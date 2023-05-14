#include <iostream>
#include <fstream>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>

#include "vector.h"
typedef _Float16 real;

// Class to hold and manage the embedding dictionary
class Dictionary{
    private:
        
        std::vector<int32_t> word2indx;                         // Holds index of word in dense vector matrix
        std::vector<std::string> words;                         // List of words in dictionary (1-1 to dense_vectors)
        std::vector<real> dense_vectors;                        // List of dense vectors in dictionary
        int dimensions;                                         // Dimensions of vectors
        int entrys;                                             // Number of vectors
        int32_t BUCKETS;                                        // Number of buckets in hash map
        float LOAD_FACTOR=0.70;                                 // Load factor of hash map
        
        uint32_t hash(const std::string& str) const;            // Hash function for lookup table
        int32_t get_id(const std::string& word) const;          // Given a word will generate index for word2indx

    public:
        Dictionary();
        ~Dictionary();
        int load(const std::string& path);                            // Load model dictionary
        void get_vector(Vector& vec, const std::string& word) const;  // Get embedding for word
        int get_dimensions() const;                                   // Get dimensions
};