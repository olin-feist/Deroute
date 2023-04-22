#include <iostream>
#include <fstream>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>


class Dictionary{
    private:
        
        std::vector<int32_t> word2indx;
        std::vector<std::string> words;
        const float* dense_vectors;
        int dimensions;
        int entrys;
        int bucket_size=200000;
        
        uint32_t Dictionary::hash(const std::string& str) const;
        int32_t Dictionary::get_id(const std::string& word);

    public:
        Dictionary(const std::string& path);
        ~Dictionary();
        const float* get_vector(const std::string& word);
        inline int get_dimensions();
};