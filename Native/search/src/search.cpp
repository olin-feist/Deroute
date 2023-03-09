#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <map>
#include <algorithm>
#include <faiss/IndexFlat.h>
#include <faiss/impl/AuxIndexStructures.h>

#include <vec_tools.h>
//search database_path urls_path query_path
// 64-bit int
using idx_t = faiss::Index::idx_t;

struct search_ret { 
    int k;
    float* distances;
    char* urls;
};


faiss::IndexFlatIP search_index;


extern "C"
void update_index(float* vector){
    search_index.add(1, vector);
}


extern "C"
search_ret* search(char* database_path, char* labels_path, float* queries){

    
    int d;     // dimension
    int nb;    // database size
    int nq=1;    // number of queries
    
    std::ifstream file;
    
    //---------------------------- database file ----------------------------
    float* database =  vectools::read_vectors(database_path, &d, &nb);
    if(database==NULL)
        exit(1);
    //-----------------------------------------------------------------------


    search_index= faiss::IndexFlatIP(d); // call constructor
    search_index.add(nb, database); // add vectors to the index



    search_ret* ret = new search_ret;

    { // search queries
        
        faiss::RangeSearchResult result(nq);
        float range_val=0.0;
        search_index.range_search(nq, queries,range_val, &result);
        
        

        idx_t* I=result.labels; //indexs
        float* D=result.distances; //distances
        
        int k=result.lims[1]; // k

        
        
        std::vector<std::pair<float,idx_t>> search_results; //put results into a pair
        
        for(int i=0;i<k;i++){
            search_results.push_back(std::pair<float, idx_t>(D[i],I[i]));
        }
        
        std::sort(search_results.begin(), search_results.end(),std::greater<>()); //sort
        
        
        //find elbow
        float prev=0.0;
        int keep_indexes=0;
        for (int i=1;i<k;i++){          
            float difference=(search_results[i-1].first)-(search_results[i].first);
            if(difference>=prev){
                keep_indexes++;
                prev=difference;
            }else{
                break;
            }
        }

        
        ret->k=keep_indexes;
        // print results
        file.open(labels_path,std::ios::binary);
        
        if(!file) {
            std::cerr << "Cannot open file!" << std::endl;
            exit(1);
        } 
        ret->urls=(char*) malloc(k*300);
        
        char* url= new char[300];
        for(int i=0;i<keep_indexes;i++){
            file.seekg(300* search_results[i].second, std::ios_base::beg);
            file.read(url,300);
            memcpy(&(ret->urls[i*300]),url,300);
        }
        file.close();
        ret->distances=(float*) malloc(k*sizeof(float));
        for(int i=0;i<keep_indexes;i++){
            ret->distances[i]=search_results[i].first;
        }
        
        delete[] url;
        
    }

    delete[] database;
    return ret;

    
    
}

//for python wrappers to dealloc pointers
extern "C"
void free_mem(void* ptr){
    free(ptr);
}

//for python wrappers to dealloc return results
extern "C"
void delete_struct(search_ret* ptr){
    delete[] ptr;
}
