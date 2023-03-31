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

// 64-bit int
using idx_t = faiss::Index::idx_t;

struct search_ret { 
    int k;
    float* distances;
    char* urls;
};


faiss::IndexFlatIP search_index;
bool isLoaded = false;
int d;     // dimension
int nb;    // entrys
int nq=1;  // number of queries
char* vectors_path;
char* urls_path;

extern "C"
int update_index(){
    if(isLoaded){
        std::fstream file(vectors_path , std::ios::in | std::ios::binary);
        if(!file) {
            std::cerr << "Error: could not open vectors file" << std::endl;
            return -1;
        }

        int dimensions;
        file.read((char*) &dimensions, 4);

        int elements;
        file.read((char*) &elements, 4); 
        if((elements-nb)==0){
            std::cerr << "Warning: no new vectors to add" << std::endl;
            return 1;
        }
        float *vec = new float[(elements-nb)*dimensions];

        file.seekp(8+sizeof(float)*((nb)*dimensions), std::ios_base::beg); //skip to new vectors

        //get new vectors
        for(int i=0;i<(elements-nb);i++){
            file.read((char*) &vec[dimensions*i], sizeof(float)*dimensions);
        }


        file.close();
        search_index.add((elements-nb), vec); //add new vectors
        delete[] vec;

        return 1;

    }else{

        std::cerr<<"Error: Database not loaded"<<std::endl;
        return -1;
    }
}

extern "C"
int load_data(char* vectors_p,char* urls_p){
    
    
    if(isLoaded){
        std::cerr<<"Error: Data already loaded"<<std::endl;
        return -1;
    }
    vectors_path = (char*)malloc(strlen(vectors_p) + 1);
    urls_path = (char*)malloc(strlen(urls_p) + 1);
    strcpy(vectors_path,vectors_p);
    strcpy(urls_path,urls_p);

    float* vectors =  vectools::read_vectors(vectors_path, &d, &nb);

    if(vectors==NULL){
        std::cerr<<"Error: No vectors file found"<<std::endl;
        return -1;
    }

    search_index= faiss::IndexFlatIP(d); // call constructor
    search_index.add(nb, vectors); // add vectors to the index

    delete[] vectors;
    isLoaded = true;
    return 1;
}

extern "C"
search_ret* search(float* queries){

    if(!isLoaded){
        std::cerr<<"Error: Indexes not loaded"<<std::endl;
        return NULL;
    }
    std::ifstream file;
    
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
        file.open(urls_path,std::ios::binary);
        
        if(!file) {
            std::cerr<<"Error: No URLS file found"<<std::endl;    
            return NULL;
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
