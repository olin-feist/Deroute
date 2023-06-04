#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <map>
#include <algorithm>
#include <faiss/IndexFlat.h>
#include <faiss/impl/AuxIndexStructures.h>
#include <shared_mutex>

#include "utils.h"


using idx_t = faiss::Index::idx_t;  // FAISS 64-bit int

namespace deroute{


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
 * The SearchIndex class contains all the data needed for Query - > Vectors - > Websites 
 * Function calls info is the same as their corresponding python wrapper calls,
 * i.e update_index() <-> update() have the same documentation
 */
class SearchIndex{
    private:
        faiss::IndexFlatIP search_index;    // FAISS index that contains dense vectors
        std::vector<std::string> urls;      // List of urls that label the FAISS index
        int d;                              // Dimension
        int nb;                             // Entries
        int nq;                             // Number of queries
        char* database_path;                // Path to dense vectors
        bool isLoaded;                      // Status of database
        mutable std::shared_mutex rw_lock;  // Shared mutex lock to prevent reads during writes

    
    public:
        SearchIndex();
        ~SearchIndex();
        int load(char* database_p);                               // Load vectors and labels (URLS)
        int update();                                             // Update database in memory
        search_ret* search(float* queries, float range) const;    // Perform search on database
        
};

}