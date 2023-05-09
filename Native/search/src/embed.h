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



Dictionary vector_dict;
bool isInitialized = false;

class term_frequency{
    private:
        std::unordered_map<std::string, int> term_map;      //Mapping of word to frequency
        int n_terms;                                        //Number of total term
        int outlier;

    public:
        term_frequency(const std::string& text);
        ~term_frequency();
       float get_weight(const std::string& word); // get term frequency weight of word
};

//pre process string for embedding
std::string pre_process(std::string sentence);

//add vector to binary file
int storeVector(std::string path, Vector vec);

/**
 * get vector (embedding) for text
 * @param text          text to be embedded
 * @param dvec          vector representation of text
 */
void vectorize(std::string text, Vector &dvec);

//Python wrappers
extern "C"{
    //load fasttex model
    void load_model(char* path);

    //get dimensions of vectors used for given model
    int get_vector_size();

    /**
     * Python wrapper to get vector of text
     * @param text          text to be embedded
     * @param output_path   optional arg for if vector needs to be saved
     * @return              return a vector if no output_path is set, return nullptr if output_path is set
     */
    float* get_vector( char* text, const char* output_path);

    //free getVector return
    void free_ptr(void* ptr);
    
}
