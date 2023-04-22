#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <map>
#include <algorithm>
#include <faiss/IndexFlat.h>
#include <faiss/impl/AuxIndexStructures.h>

#include "utils.h"

/**
 * Return type for search
 * @param k          number of results found
 * @param distances  pointer to list of distance
 * @param urls       pointer to list of urls
 */
struct search_ret { 
    int k;
    float* distances;
    char** urls;

    ~search_ret();
};


/**
 * The URLVectorIndex class contains all the data needed for Query - > Vectors - > Websites 
 * Function calls info is the same as their corresponding python wrapper calls,
 * i.e update_index() <-> update() have the same documentation
 */
class URLVectorIndex{
    private:
        faiss::IndexFlatIP search_index;    // FAISS index that contains dense vectors
        std::vector<std::string> urls;      // List of urls that label the FAISS index
        int d;                              // Dimension
        int nb;                             // entries
        int nq;                             // number of queries
        char* vectors_path;                 // Path to dense vectors
        char* urls_path;                    // Path to URLS
        bool isLoaded;                      // Status of database

    
    public:
        URLVectorIndex();
        ~URLVectorIndex();
        int load(char* vectors_p, char* urls_p);
        int update();
        search_ret* search(float* queries);
        
};


using idx_t = faiss::Index::idx_t;  // FAISS 64-bit int
extern URLVectorIndex database;            // Database of Websites

// Python Wrappers
extern "C" {
    /**
     * Update the current database in memory with new content from local file
     * @return              1 success -1 Error
     */
    int update_index();

    /**
     * Load vectors and their corresponding labels (URLS) into memory
     * @param vectors_p     path to .bin of vectors
     * @param urls_p        path to .bin of urls
     * @return              1 success -1 Error
     */
    int load_data(char* vectors_p, char* urls_p);

    /**
     * Perform similarity search
     * @param queries       vector of single query
     * @return struct containing results and distances
     */
    search_ret* search(float* queries);

    //for python wrappers to dealloc pointers
    void free_mem(void* ptr);

    //for python wrappers to dealloc list of pointers
    void free_list(void** ptr,int k);

    //for python wrappers to dealloc return results
    void delete_struct(search_ret* ptr);
}