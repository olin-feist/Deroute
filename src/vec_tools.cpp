#include <vec_tools.h>

int vectools::print_vectors(std::string path){
    std::fstream file(path , std::ios::in | std::ios::binary);
    if(!file) {
        std::cerr << "Cannot open file!" << std::endl;
        return 1;
    }

    std::string str; 
    int dimension;
    file.read((char*) &dimension, sizeof(int));
    std::cout<<dimension<<" ";
    int elements;
    
    file.read((char*) &elements, sizeof(int));
    std::cout<<elements<<std::endl;
    float* vec= new float[dimension];
    for(int i=0;i<elements;i++){
        file.read((char*) vec, sizeof(float)*dimension);
        for(int i=0;i<15;i++)
            std::cout<<vec[i]<<", ";
        std::cout<<"..."<<std::endl;
    }
    delete(vec);

    return 0;
        
}

int vectools::mul_vector(float* v1, float mult, int size){
    for (int i = 0; i < size; i++) {
        v1[i] *= mult;
    }
    return 0;
}

int vectools::add_vectors(float* v1, float* v2, int size){
    for (int i = 0; i < size; i++) {
        v1[i] += v2[i];
    }
    return 0;
}

float* vectools::read_vectors(std::string path, int *d, int *n){
    std::ifstream file;

    file.open(path,std::ios::binary);
    if(!file) {
        std::cerr << "Cannot open file!" << std::endl;
        return NULL;
    } 
    int dimensions;
    int entrys;
    file.read((char*) &dimensions, sizeof(int)); // dimension
    file.read((char*) &entrys, sizeof(int)); // database size
    float *vec = new float[dimensions*entrys];
    
    //get database
    for(int i=0;i<entrys;i++){
        file.read((char*) &vec[dimensions*i], sizeof(float)*dimensions);
    }

    file.close();
    *d=dimensions;
    *n=entrys;
    return vec;
}
