#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <sstream>

#include "dictionary.h"
#include "vector.h"


Dictionary vector_dict;
bool isInitialized = false;

class term_frequency{
    private:
        std::unordered_map<std::string, int> term_map;      //Mapping of word to frequency
        int n_terms;                                        //Number of total term

    public:
        term_frequency(const std::string& text);
        ~term_frequency();
       void TF_Vector(Vector& svec,const std::string& text); //Get average embeddings with weights applied
};

extern "C"{
    //load fasttex model
    void load_model(char* path);

    //get dimensions of vectors used for given model
    int get_vector_size();

    //get sentence vectors
    float* getVector(char* output, char* sentence);

    //free getVector return
    void free_ptr(void* ptr);
    
}

//pre process string for embedding
std::string pre_process(std::string sentence);

//add vector to binary file
int storeVector(std::string path, Vector vec);