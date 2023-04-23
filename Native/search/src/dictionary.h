#include <iostream>
#include <fstream>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>

#include "vector.h"

class Dictionary{
    private:
        
        std::vector<int32_t> word2indx;
        std::vector<std::string> words;
        std::vector<float> dense_vectors;
        int dimensions;
        int entrys;
        int32_t bucket_size=3000000;
        
        uint32_t hash(const std::string& str) const;
        int32_t get_id(const std::string& word);

    public:
        Dictionary();
        ~Dictionary();
        void load(const std::string& path);
        void get_vector(Vector& vec, const std::string& word);
        int get_dimensions();
};