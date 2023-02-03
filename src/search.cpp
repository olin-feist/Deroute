#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <faiss/IndexFlat.h>
//search database_path k_neighbors query_path
// 64-bit int
using idx_t = faiss::Index::idx_t;


int main(int argc, char* args[]) {

    auto start = std::chrono::high_resolution_clock::now();

    int d;     // dimension
    int nb;    // database size
    int nq;    // number of queries
    
    std::ifstream file;

    //---------------------------- database file ----------------------------

    file.open(args[1],std::ios::binary);
    if(!file) {
        std::cerr << "Cannot open file!" << std::endl;
        return 1;
    } 

    file.read((char*) &d, sizeof(int)); // dimension
    file.read((char*) &nb, sizeof(int)); // database size
    float* database = new float[d * nb];

    //get database
    for(int i=0;i<nb;i++){
        file.read((char*) &database[d*i], sizeof(float)*d);
    }
    
    file.close(); 

    //---------------------------- query file ----------------------------
    int d_q;
    float* queries;
    if(argc==4){
        
        file.open(args[3],std::ios::binary); //open file
        if(!file) {
            std::cerr << "Cannot open file!" << std::endl;
            return 1;
        } 

        file.read((char*) &d_q, sizeof(int));
        if(d_q!=d){
            std::cerr << "Query vectors and Database vectors different dimensions " <<d<<"!="<<d_q<< std::endl;
            return 1;
        }

        file.read((char*) &nq, sizeof(int)); // number of queries

        queries = new float[d * nq];

        //get queries
        for(int i=0;i<nq;i++){
            file.read((char*) &queries[d*i], sizeof(float)*d);
        }
        
        file.close(); 
    }else{
        setmode(fileno(stdin), O_BINARY);
        //check error for differing dimensions
        
        std::cin.read((char*) &d_q, sizeof(int));
        if(d_q!=d){
            std::cerr << "Query vectors and Database vectors different dimensions " <<d<<" != "<<d_q<< std::endl;
            return 1;
        }

        std::cin.read((char*) &nq, sizeof(int)); // number of queries

        queries = new float[d * nq];

        //get queries
        for(int i=0;i<nq;i++){
            std::cin.read((char*) &queries[d*i], sizeof(float)*d);
            
        }
    }   
    
    //-------------------------------------------------------------------


    
    faiss::IndexFlatL2 index(d); // call constructor
    index.add(nb, database); // add vectors to the index

    int k = std::stoi(args[2]);


    { // search queries

        idx_t* I = new idx_t[k * nq]; //indexs
        float* D = new float[k * nq]; //distances

        index.search(nq, queries, k, D, I);

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        //time
        std::cout <<std::fixed<< "Time taken by function: "<< duration.count()*0.000001 << " seconds" << std::endl;

        // print results
        printf("Index = ");
        for(int i=0;i<k;i++){
            printf("%5zd ", I[i]);
        }
        printf("\n");

        printf("Distances = ");
        for(int i=0;i<k;i++){
            printf("%f ", D[i]);
        }
        //printf("\n");

        delete[] I;
        delete[] D;
    }

    delete[] database;
    delete[] queries;

    return 0;
}
