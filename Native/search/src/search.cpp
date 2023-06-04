#include "search.h"


namespace deroute{


//deconstructor for search return
search_ret::~search_ret() {
    for(int i=0;i<k;i++){
        free(urls[i]);
    }
    free(urls);
    free(distances);
}


SearchIndex::SearchIndex():nq(1), isLoaded(false){}   //constructor
SearchIndex::~SearchIndex(){
    free(database_path);
}  //deconstructor

//update index
int SearchIndex::update(){
    
    //check if database is loaded
    if(isLoaded){
        std::unique_lock<std::shared_mutex> lock(rw_lock); //prevent read operations

        float *vec;
        int d_new;
        int new_entries;
        std::vector<std::string> new_labels;
        //read in new vector
        if(utils::read_database(database_path,&d_new,&new_entries,vec,new_labels,nb)){
            return 1;
        }
        if(new_entries==0){ //nothing to add
            return 0;
        }
        if(d_new!=d){ // mismatch of dimensions
            std::cerr<<"Error: Different dimensions"<<std::endl;
            return 2;
        }
        urls.insert(std::end(urls), std::begin(new_labels), std::end(new_labels)); // add new labels
        search_index.add(new_entries, vec); //add new vectors
        nb+=new_entries; // update count of entries
        delete[] vec;
        return 0;
    }else{
        std::cerr<<"Error: Database is not loaded"<<std::endl;
        return 1;
    }
}

//load data
int SearchIndex::load(char* database_p){
    if(isLoaded){
        std::cerr<<"Error: Database already loaded"<<std::endl;
        return -1;
    }

    //save database paths
    database_path = (char*)malloc(strlen(database_p) + 1);
    strcpy(database_path,database_p);


    //load vectors and labels from database file
    float* vectors;
    if(utils::read_database(database_path, &d, &nb,vectors,urls)){
        std::cerr<<"Error: Reading database failed"<<std::endl;
        return -1;
    }  
    
    search_index= faiss::IndexFlatIP(d);
    search_index.add(nb, vectors); // add vectors to the index

    delete[] vectors;

    isLoaded = true;

    return 1;
}

//searching
search_ret* SearchIndex::search(float* queries, float range=0.50) const{

    if(!isLoaded){
        std::cerr<<"Warning: Database not loaded"<<std::endl;
        return NULL;
    }
    
    std::shared_lock<std::shared_mutex> lock(rw_lock); //prevent write operations to search index

    { // search queries
        
        faiss::RangeSearchResult result(nq);
        
        search_index.range_search(nq, queries,range, &result);
        

        idx_t* I=result.labels; //indexs
        float* D=result.distances; //distances
        
        int k=result.lims[1]; // k

        
        
        std::vector<std::pair<float,idx_t>> search_results; //put results into a pair
        
        for(int i=0;i<k;i++){
            search_results.push_back(std::pair<float, idx_t>(D[i],I[i]));
        }
        
        std::sort(search_results.begin(), search_results.end(),std::greater<>()); //sort
        
        
        //find elbow
        int keep_indexes=0;

        float prev=0.0;
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
            ret->distances=(float*) malloc(keep_indexes*sizeof(float));

            for(int i=0;i<keep_indexes;i++){
                ret->distances[i]=search_results[i].first;
            }
        }

        return ret;
    }
    
}

}
