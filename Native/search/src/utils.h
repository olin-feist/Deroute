#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include "Vector.h"

namespace utils{
    int print_vectors(std::string path);                                                                        // Print vectors in file
    int add_vectors(float* v1, float* v2,int size);                                                             // Add v2 to v1
    int mul_vector(float* v1, float mult, int size);                                                            // Multiply v1 by scalar mult
    int read_database(std::string path,int *d, int *n,float * &vec,std::vector<std::string> &labels);           // Read vectors and labels from database file 
    int read_database(std::string path,int *d, int *n,float * &vec,std::vector<std::string> &labels, int idx);  // Read vectors and labels from specific idx
    int write_database(std::string path, const Vector &vec, const std::string label);                           // write a vector and label to database file
}