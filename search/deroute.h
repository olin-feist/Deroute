#include <vec_tools.h>
namespace deroute{
    namespace search{ 
        /**
         * Return type for search
         * @param k          number of results found
         * @param distances  pointer to list of distance
         * @param urls       pointer to list of 300 charecter labels
         */
        struct search_ret { 
            int k;
            float* distances;
            char* urls;
        };
        
        /**
         * Update index space
         * @param vector vector to be added to index space
         */
        void update_index(float* vector);

        /**
         * Perform similarity search
         * @param database_path path to .bin of vectors
         * @param labels_path   path to .bin of labels
         * @param queries       vector of single query
         * @return struct containing results and distances
         */
        search_ret* search(char* database_path, char* labels_path, float* queries);
    }

    namespace embed{
        /**
         * Get a dense vector for a givin sentence
         * @param output     optional param to select where resulting vector goes
         * @param sentence   string to be embeded
         * @return           if output is empty string, return will be the resulting dense vector
         */
        float* getVector(char* output, char* sentence);

        /**
         * Get size of vectors being used
         * @return size of vectors
         */
        int get_vector_size();

        /**
         * Load fastText model to used
         * @param path path to model
         */
        void load_model(char* path);
    }

    namespace lsh{
        /**
         * Get a 128 bit hash of a vector database
         * @param database_path  path to .bin that contains vectors
         * @return 128 bit value
         */
        __uint128_t* local_hash(char* database_path);
    }
}