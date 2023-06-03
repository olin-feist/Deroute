#include "deroute.h"


// -------------------------- Pyton Wrapper Calls --------------------------
int get_vector_size(){
    if(deroute::model.is_loaded()){
        std::cerr<<"Error: fastText model is not loaded"<<std::endl;
        std::cerr<<"Call load_model(char* path) to fix"<<std::endl;
        return -1;
    }
    return deroute::model.get_dimensions();
}

void load_model(char* path){
    deroute::model.load(path);
    
}

float* get_vector(char* text, const char* output_path = ""){

    if(!deroute::model.is_loaded()){
        std::cerr<<"Error: fastText model is not loaded"<<std::endl;
        std::cerr<<"Call load_model(char* path) to fix"<<std::endl;
        return NULL;
    }

    int dimensions=deroute::model.get_dimensions();
    Vector vec(dimensions); //Vector for document
    
    deroute::model.vectorize(text,vec);    //Get embedding for text

    //if output path specified
    if(output_path[0]!='\0'){
        deroute::model.store_vector(output_path,vec);
        return nullptr;
    //return vector
    }else{
        float* ret = (float*) malloc(dimensions*sizeof(float));
        memcpy(ret, vec.data(), dimensions*sizeof(float));
        return ret;
    }

}

void free_ptr(float* ptr){
    free(ptr);
}

int update_index(){
    return deroute::database.update();
}

int load_search_index(char* vectors_p, char* urls_p){
    return deroute::database.load(vectors_p, urls_p);
}

deroute::search_ret* search(float* queries){
    return deroute::database.search(queries,0.5);
}

void free_mem(void* ptr){
    free(ptr);
}

void free_list(void** ptr,int k){
    for(int i=0;i<k;i++){
        free(ptr[i]);
    }
    free(ptr);
}

void delete_struct(deroute::search_ret* ptr){
    delete ptr;
}

// -------------------------- Pyton Wrapper Calls --------------------------