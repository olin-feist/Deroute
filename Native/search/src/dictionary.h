#include <iostream>
#include <fstream>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>

#include "vector.h"

// Class to hold and manage the embedding dictionary
class Dictionary{
    private:
        
        std::vector<int32_t> word2indx;                         //Holds index of word in dense vector matrix
        std::vector<std::string> words;                         //List of words in dictionary (1-1 to dense_vectors)
        std::vector<float> dense_vectors;                       //List of dense vectors in dictionary
        int dimensions;                                         //Dimensions of vectors
        int entrys;                                             //Number of vectors
        int32_t bucket_size=3000000;                            //Buckets for hash table
        
        uint32_t hash(const std::string& str) const;            //Hash function for lookup table
        int32_t get_id(const std::string& word);                //Given a word will generate index for word2indx

    public:
        Dictionary();
        ~Dictionary();
        void load(const std::string& path);                     //Load model dictionary
        void get_vector(Vector& vec, const std::string& word);  //Get embedding for word
        int get_dimensions();                                   //Get dimensions
};