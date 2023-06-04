#include "utils.h"
#include "search.h"
#include "embed.h"



namespace deroute{
    Model model = Model();                           // Search index of websites
    SearchIndex database = SearchIndex();            // Database of Websites
}


//Python wrappers
extern "C"{

    /**
     * Python wrapper to load Deroute Model
     * @param path          file path of model
     */
    int load_model(char* path);

    //get dimensions of vectors used for given model
    int get_vector_size();

    /**
     * Python wrapper to get vector of text
     * @param text          text to be embedded
     * @param output_path   optional arg for if vector needs to be saved
     * @return              return a vector if no output_path is set, return nullptr if output_path is set
     */
    float* get_vector(char* text, const char* label,const char* output_path);

    //free getVector return
    void free_ptr(float* ptr);
    
    /**
     * Update the current database in memory with new content from local file
     * @return              1 success -1 Error
     */
    int update_index();

    /**
     * Load vectors and their corresponding labels (URLS) into memory
     * @param database_p     path to .bin of labels and vectors
     * @return              1 success -1 Error
     */
    int load_search_index(char* database_p);

    /**
     * Perform similarity search
     * @param queries       vector of single query
     * @return struct containing results and distances
     */
    deroute::search_ret* search(float* queries);

    //for python wrappers to dealloc pointers
    void free_mem(void* ptr);

    //for python wrappers to dealloc list of pointers
    void free_list(void** ptr,int k);

    //for python wrappers to dealloc return results
    void delete_struct(deroute::search_ret* ptr);
}