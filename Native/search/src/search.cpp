#include "search.h"



//deconstructor for search return
search_ret::~search_ret() {
    for(int i=0;i<k;i++){
        free(urls[i]);
    }
    free(urls);
    free(distances);
}


URLVectorIndex::URLVectorIndex():nq(1), isLoaded(false){}   //constructor
URLVectorIndex::~URLVectorIndex(){
    free(vectors_path);
    free(urls_path);
}  //deconstructor

//update index
int URLVectorIndex::update(){
    //check if database is loaded
    if(isLoaded){

        //get database files and check if they exist
        std::fstream file_vector(vectors_path , std::ios::in | std::ios::binary);
        std::fstream file_url(urls_path , std::ios::in | std::ios::binary);

        if(!file_vector) {
            std::cerr << "Error: could not open vectors file" << std::endl;
            return -1;
        }

        if(!file_vector) {
            std::cerr << "Error: could not open URLS file" << std::endl;
            return -1;
        }
        
        int dimensions;
        file_vector.read((char*) &dimensions, 4);

        int elements;
        file_vector.read((char*) &elements, 4); 

        int new_idx=(elements-nb); // new elements that have been added

        if(new_idx==0){
            std::cerr << "Warning: no new vectors to add" << std::endl;
            return 1;
        }
        
        float *vec = new float[new_idx*dimensions];

        file_vector.seekp(8+sizeof(float)*((nb)*dimensions), std::ios_base::beg); //skip to new vectors

        //get new vectors
        for(int i=0;i<new_idx;i++){
            file_vector.read((char*) &vec[dimensions*i], sizeof(float)*dimensions);
        }
        file_vector.close();

        search_index.add(new_idx, vec); //add new vectors

        file_url.seekp(300*nb, std::ios_base::beg); //skip to new urls

        //update urls
        char* url= new char[300];
        for(int i=0;i<new_idx;i++){
            file_url.read(url,300);
            urls.push_back(url);
        }

        nb+=new_idx; // update count of entries

        delete[] url;
        delete[] vec;

        return 1;

    }else{
        std::cerr<<"Error: Database is not loaded"<<std::endl;
        return -1;
    }
}

//load data
int URLVectorIndex::load(char* vectors_p, char* urls_p){
    if(isLoaded){
        std::cerr<<"Error: Database already loaded"<<std::endl;
        return -1;
    }

    //save database paths
    vectors_path = (char*)malloc(strlen(vectors_p) + 1);
    urls_path = (char*)malloc(strlen(urls_p) + 1);
    strcpy(vectors_path,vectors_p);
    strcpy(urls_path,urls_p);

    //load vectors from vectors file
    float* vectors =  utils::read_vectors(vectors_path, &d, &nb);    
    if(vectors==NULL){
        std::cerr<<"Error: No vectors file found"<<std::endl;
        return -1;
    }

    //open URLS file
    std::fstream file(urls_path , std::ios::in | std::ios::binary);
    if(!file) {
        std::cerr << "Error: No URLS file found" << std::endl;
        return -1;
    }
    
    //load in URLS from saved file
    char* url= new char[300];
    for(int i=0;i<nb;i++){
        file.read(url,300);
        urls.push_back(url);
    }
    file.close();
    
    search_index= faiss::IndexFlatIP(d);
    search_index.add(nb, vectors); // add vectors to the index

    delete[] vectors;
    delete[] url;

    isLoaded = true;

    return 1;
}

//searching
search_ret* URLVectorIndex::search(float* queries){
    
    if(!isLoaded){
        std::cerr<<"Error: Database not loaded"<<std::endl;
        return NULL;
    }
    
    

    { // search queries
        
        faiss::RangeSearchResult result(nq);
        float range_val=0.65;
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
                keep_indexes++;
                break;
            }
        }

        if(k==1){
            keep_indexes=1;
        }
        //keep_indexes=k;
        //Check for too large k
        if(k > SIZE_MAX / 300) {
            std::cerr<<"Error: k is too large"<<std::endl;    
            return NULL;
        } 
        search_ret* ret = new search_ret;
        { // Build return struct
            
            ret->k=keep_indexes;
            ret->urls=(char**) malloc(k*sizeof(char*));
            
            //Add URL labels to return
            for(int i=0;i<keep_indexes;i++){   
                std::string url=urls[search_results[i].second];
                ret->urls[i]=(char*)malloc(url.size()+1);
                memcpy(ret->urls[i],url.c_str(),url.size()+1);
                
            }

            //Add distances to return
            ret->distances=(float*) malloc(k*sizeof(float));
            for(int i=0;i<keep_indexes;i++){
                ret->distances[i]=search_results[i].first;
            }
        }
        
        return ret;
    }
    
}


URLVectorIndex database= URLVectorIndex(); //create database


// -------------------------- Pyton Wrapper Calls --------------------------
int update_index(){
    return database.update();
}

int load_data(char* vectors_p, char* urls_p){
    return database.load(vectors_p, urls_p);
}

search_ret* search(float* queries){
    return database.search(queries);
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

void delete_struct(search_ret* ptr){
    delete ptr;
}
// -------------------------- Pyton Wrapper Calls --------------------------