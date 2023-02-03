#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <faiss/IndexFlat.h>
#include <vec_tools.h>
//search database_path k_neighbors query_path
// 64-bit int
using idx_t = faiss::Index::idx_t;


int main(int argc, char* args[]) {


    int d;     // dimension
    int nb;    // database size
    int nq;    // number of queries
    
    std::ifstream file;

    //---------------------------- database file ----------------------------
 
    float* database =  vectools::read_vectors(args[1], &d, &nb);
    if(database==NULL)
        return 1;
    //---------------------------- query file ----------------------------
    int d_q;
    float* queries;
    //if input path is defined
    if(argc==4){
        float* queries =  vectools::read_vectors(args[3], &d_q, &nq);
        if(queries==NULL)
            return 1;

        if(d_q!=d){
            std::cerr << "Query vectors and Database vectors different dimensions " <<d<<" != "<<d_q<< std::endl;
            return 1;
        }

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


        std::cout<<k<<"\n";

        // print results
        file.open("data/urls.bin",std::ios::binary);
        char* url= new char[300];
        for(int i=0;i<k;i++){
            file.seekg(300* I[i], std::ios_base::beg);
            file.read(url,300);
            std::cout<<url<<"\n";
        }
        file.close();

        for(int i=0;i<k;i++){
            printf("%f ", D[i]);
            std::cout<<"\n";
        }

        delete[] url;
        delete[] I;
        delete[] D;
    }

    delete[] database;
    delete[] queries;

    return 0;
}
