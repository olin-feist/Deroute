#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <regex>
#include <unordered_map>
#include <sstream>
#include <optional>
#include <cmath>

#include "dictionary.h"
#include "vector.h"
#include "utils.h"

namespace deroute {

class term_frequency{
    private:
        std::unordered_map<std::string, int> term_map;      // Mapping of word to frequency
        int n_terms;                                        // Number of total terms
        int outlier;                                        // Outlier value for frequency

    public:
        term_frequency(const std::string& text);
        ~term_frequency();
       float get_weight(const std::string& word);                    // Get term frequency weight of word
       float get_weight(int freq);                                   // Get term frequency weight from frequency
       inline const std::unordered_map<std::string, int>& get_map(){ // Get term frequency map
            return term_map;
       }
};

//class to hold needed methods to create and save vectors
class Model{
    private:
        Dictionary vector_dict;             // Hash map of all dense vectors
        bool isInitialized;                 // Status of Model, i.e. loaded or unloaded

        //pre process string for embedding
        std::string pre_process(std::string sentence) const;

    public:
        Model();
        ~Model();
        //load in model, currently just loads in the dictionary
        int load(const std::string& path);

        //add vector to binary file
        int store_vector(std::string path, const Vector &vec, const std::string label) const;

        /**
         * get vector (embedding) for text
         * @param text          text to be embedded
         * @param dvec          vector representation of text
         */
        void vectorize(std::string text, Vector &dvec) const;

        bool is_loaded() const;

        int get_dimensions() const;


};



}

