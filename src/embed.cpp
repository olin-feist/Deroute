#include <fastText.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace fasttext;
//.\bin\embed.exe bin/wiki.en.bin data/website_plain.txt data/vectors.bin append

//get sentence vectors
Vector getVector(std::string model_path){
    FastText fasttext;
    fasttext.loadModel(model_path); //load model

    Vector svec(fasttext.getDimension());
    fasttext.getSentenceVector(std::cin, svec); // compute first sentence vector

    return svec;
}

//add vector to binary file
int storeVector(std::string path, Vector vec){

    std::fstream append_f(path, std::ios::out | std::ios::in | std::ios::binary);
    
    //if appending new vector to existing file
    if(append_f.good()){
        if(!append_f) {
            std::cerr << "Cannot open file!" << std::endl;
            return 1;
        }
        
        //get dimensions the re write
        int dimensions;
        append_f.read((char*) &dimensions, 4);
        append_f.seekp(0, std::ios_base::beg);
        append_f.write((char*) &dimensions, 4);

        //get elements, edit then re write
        int elements;
        append_f.read((char*) &elements, 4);
        append_f.seekp(sizeof(int), std::ios_base::beg);
        elements++;
        append_f.write((char*) &elements, 4);

        //append new vector
        append_f.seekp(8+sizeof(float)*((elements-1)*dimensions), std::ios_base::beg);
        for(int i=0;i<dimensions;i++){
            append_f.write((char*) &vec[i], sizeof(float));
        }

        append_f.close();

    //new file
    }else{
        std::ofstream write_f(path, std::ios::out | std::ios::binary);
        if(!write_f) {
            std::cerr << "Cannot open file!" << std::endl;
            return 1;
        }

        std::string str; 
        FastText fasttext;
        int dimension=300;
        write_f.write((char*) &dimension, sizeof(int));

        int elements=1;
        write_f.write((char*) &elements, sizeof(int));

        for(int i=0;i<dimension;i++){
            write_f.write((char*) &vec[i], sizeof(float));
        }
        write_f.close();
       
    }

    append_f.close();
    return 0;
}


int main(int argc, char* argv[]){

    if(argc>3){
        std::cerr<< "Invalid Number of Args" << std::endl;
        std::cerr<< "usage: embed <model_path> <optional_output_path>" << std::endl;
        return 1;
    }

    //get sentence vector
    Vector svec=getVector(argv[1]);
    //if output path specified
    if(argc==3){
        storeVector(argv[2],svec);

    //write to stdout
    }else{
        for(int i=0;i<svec.size();i++){
             std::cout.write((char*) &svec[i], sizeof(float));
        }
    }

    return 0;
}