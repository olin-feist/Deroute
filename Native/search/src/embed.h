#include <fastText.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <regex>

using namespace fasttext;

FastText fastText;
bool isFastTextInitialized = false;

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