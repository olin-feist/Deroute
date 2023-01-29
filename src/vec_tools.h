#include <iostream>
#include <fstream>
#include <string.h>

namespace vectools{
    int print_vectors(std::string path); 
    int add_vectors(float* v1, float* v2,int size);
    int mul_vector(float* v1, float mult, int size);
    float * read_vectors(std::string path,int *d, int *n);
    
}