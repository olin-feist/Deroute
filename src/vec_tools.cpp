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

int vectools::average_vectors(float* data){
    return 0;
}


int main(){
    vectools::print_vectors("data/vectors.bin");
}