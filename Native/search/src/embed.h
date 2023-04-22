#include <fastText.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <sstream>

using namespace fasttext;

FastText fastText_model;
bool isFastTextInitialized = false;

class term_frequency{
    private:
        std::unordered_map<std::string, int> term_map;
        int n_terms;

    public:
        term_frequency(const std::string& text);
        ~term_frequency();
       void TF_Vector(fasttext::Vector& svec,const std::string& text);
};

extern "C"{
    //load fasttex model
    void load_model(char* path);

    //get dimensions of vectors used for given model
    int get_vector_size();

    //get sentence vectors
    float* getVector(char* output, char* sentence);
}

//pre process string for embedding
std::string pre_process(std::string sentence);

//add vector to binary file
int storeVector(std::string path, Vector vec);