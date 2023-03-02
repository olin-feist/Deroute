#include <vec_tools.h>
#include <faiss/IndexLSH.h>
#include <bitset>

#define HASH_SIZE 128

using idx_t = faiss::Index::idx_t;

int local_hash(float* vec, int dimensions, int entrys){
    
    faiss::IndexLSH*  LSH =  new faiss::IndexLSH (dimensions,HASH_SIZE);
    
    //compute average
    float *avg_vec= new float[dimensions];
    std::fill(avg_vec, avg_vec + dimensions, 0.0);
    for(int i=0;i<entrys;i++){
        vectools::add_vectors(avg_vec,&vec[i*dimensions],dimensions);
    }
    float f= 1.0/entrys;
    vectools::mul_vector(avg_vec,f,dimensions);


    //get encoding
    uint8_t *encoding=new uint8_t[(HASH_SIZE/8)];
    LSH->sa_encode(1,avg_vec,encoding);
    
    //print encoding
    for(int i = 0;i<HASH_SIZE/8;i++){
        std::bitset<8> b(encoding[i]);
        std::cout << b;
    }
    std::cout<<std::endl;


    delete(avg_vec);
    delete(encoding);
    delete(LSH);

    return 0;
}

int main(int argc, char **args){
    int dimensions;
    int entrys;
    
    //get vectors
    float *vec = vectools::read_vectors(args[1],&dimensions,&entrys);
    //hash vectors
    local_hash(vec,dimensions,entrys);
    
    delete(vec);
    
}

