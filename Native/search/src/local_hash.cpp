#include <faiss/IndexLSH.h>
#include <bitset>

#include "utils.h"

#define HASH_SIZE 128

using idx_t = faiss::Index::idx_t;

extern "C"
__uint128_t* local_hash(char* database_path){
    
    int dimensions;     // dimension
    int entrys;    // database size

    //get database
    float* database =  utils::read_vectors(database_path, &dimensions, &entrys);
    if(database==NULL)
        exit(1);

    faiss::IndexLSH*  LSH =  new faiss::IndexLSH (dimensions,HASH_SIZE);
    
    //compute average
    float *avg_vec= new float[dimensions];
    std::fill(avg_vec, avg_vec + dimensions, 0.0);
    for(int i=0;i<entrys;i++){
        utils::add_vectors(avg_vec,&database[i*dimensions],dimensions);
    }
    float f= 1.0/entrys;
    utils::mul_vector(avg_vec,f,dimensions);


    //get encoding
    uint8_t *encoding=new uint8_t[(HASH_SIZE/8)];
    LSH->sa_encode(1,avg_vec,encoding);
    
    //print encoding
    for(int i = 0;i<HASH_SIZE/8;i++){
        std::bitset<8> b(encoding[i]);
        std::cout << b;
    }
    std::cout<<std::endl;
    
    __uint128_t* ret = new __uint128_t;
    *ret=0;
    // copy the bytes into the lower 16 bytes of the 128-bit integer
    memcpy(ret, encoding, sizeof(encoding));

    delete(avg_vec);
    delete(encoding);
    delete(LSH);

    return ret;
}

//for python wrappers to dealloc pointers
extern "C"
void free_mem(void* ptr){
    free(ptr);
}

