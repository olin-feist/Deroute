#include <iostream>
#include <fstream>
#include <string.h>

namespace utils{
    int print_vectors(std::string path);                    //Print vectors in file
    int add_vectors(float* v1, float* v2,int size);         //Add v2 to v1
    int mul_vector(float* v1, float mult, int size);        //Multiply v1 by scalar mult
    float * read_vectors(std::string path,int *d, int *n);  //Read vectors from .bin to 
}