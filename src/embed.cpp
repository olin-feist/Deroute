#include <fastText.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace fasttext;
//.\bin\embed.exe bin/wiki.en.bin data/website_plain.txt data/vectors.bin append

//get sentence vectors
Vector getVector(std::string model_path, std::string input_path){
    FastText fasttext;
    fasttext.loadModel(model_path); //load model

    std::fstream file;
    file.open(input_path,std::ios::in); //get website

    Vector svec(fasttext.getDimension());
    fasttext.getSentenceVector(file, svec); // compute first sentance vector

    return svec;
}

//add vector to binary file
int storeVector(int append, std::string path, Vector vec){


    //if appending new vector
    if(append){
        std::fstream append_f(path, std::ios::out | std::ios::in | std::ios::binary);
        
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

        int start=0;
        int increment=0;
        for(int i=0;i<dimension;i++){
            write_f.write((char*) &vec[i], sizeof(float));
        }

        write_f.close();
    }
    
    return 0;
}


int main(int argc, char* argv[]){

    if(argc!=5){
        std::cerr<< "Invalid Number of Args" << std::endl;
        std::cerr<< "usage: embed <model_path> <input_path> <output_path> <apppend | new>" << std::endl;
        return 1;
    }

    //get sentence vector
    Vector svec=getVector(argv[1],argv[2]);
    
    //storing vector
    if(strcmp(argv[4], "append")==0){
        storeVector(1,argv[3],svec);
    }else if(strcmp(argv[4], "new")==0){
        storeVector(0,argv[3],svec);
    }else{
        std::cerr<< "Invalid write operation arg" << std::endl;
        std::cerr<< "Expected append or new" << std::endl;
        return 1;
    }

    //std::ifstream rf(argv[3], std::ios::binary);
    /*FastText fasttext;
    fasttext.loadModel("bin/wiki.en.bin"); //load model
    Args a= Args();
    a.input="bin/wiki.en.bin";
    a.output="bin/quantize.bin";
    fasttext.quantize(a);
    fasttext.saveModel("bin/quantize.bin");*/
    return 0;
}