#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <shared_mutex>
#include <unordered_map>
#include <mutex> 
#include <algorithm>

#include "Vector.h"

namespace utils{
    /**
     * Print content of deroute database file
     * @param path          path to database file
     * @return              0 success 1 Error
     */
    int print_database(std::string path);
    
    /**
     * Add v2 to v1 over size 
     * @param v1            vector to add to
     * @param v2            add vector
     * @param size          number of dimensions to add to
     * @return              0 success 1 Error
     */                                                                     
    int add_vectors(float* v1, float* v2,int size);   
    
    /**
     * Multiply v1 by v2 over size 
     * @param v1            vector to multiply into
     * @param mult          scalar value
     * @param size          number of dimensions to mutliply to
     * @return              0 success 1 Error
     */                                                           
    int mul_vector(float* v1, float mult, int size);
    
    /**
     * Read vectors and labels from database file 
     * @param path          path to database file
     * @param d             set to dimensions of vectors
     * @param n             set to number of entries
     * @param vec           set to vectors in database (float* [dimensions*entries])
     * @param labels        set to labels in database (1<->1 with vec)
     * @return              0 success 1 Error
     */  
    int read_database(std::string path,int *d, int *n,float * &vec,std::vector<std::string> &labels); 

    /**
     * Read vectors and labels from database file at specific data entry index
     * @param path          path to database file
     * @param d             set to dimensions of vectors
     * @param n             set to number of entries
     * @param vec           set to vectors in database (float* [dimensions*entries])
     * @param labels        set to labels in database (1<->1 with vec)
     * @param idx           index of where to read from
     * @return              0 success 1 Error
     */  
    int read_database(std::string path,int *d, int *n,float * &vec,std::vector<std::string> &labels, int idx);

    /**
     * Write vector and label to end of deroute database
     * @param path          path to database file
     * @param vec           Vector to be written to database
     * @param label         Label of vector
     * @return              0 success 1 Error
     */ 
    int write_database(std::string path, const Vector &vec, std::string label);    

    /**
     * Check file is valid
     * @param in           fstream to binary file
     * @return             True success False Error
     */ 
    bool check_file(std::fstream& in);

    /**
     * Sign file
     * @param out          ostream to binary file
     * @return             True success False Error
     */ 
    void sign_file(std::ostream& out);                           
}