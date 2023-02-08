#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <algorithm>
#include <faiss/IndexFlat.h>
#include <faiss/impl/AuxIndexStructures.h>

#include <vec_tools.h>
//search database_path query_path
// 64-bit int
using idx_t = faiss::Index::idx_t;


bool compare(const int &a, const int &b, float *distances) {
  return distances[a] > distances[b];
}



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
    if(argc==3){
        float* queries =  vectools::read_vectors(args[2], &d_q, &nq);
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


    
    faiss::IndexFlatIP index(d); // call constructor
    index.add(nb, database); // add vectors to the index


    { // search queries

        faiss::RangeSearchResult result(nq);
        float range_val=0.0;
        index.range_search(nq, queries,range_val, &result);
        
        

        idx_t* I=result.labels;
        float* D=result.distances;
        
        int k=result.lims[1];
        
        //sort indexes based on float array
        std::sort(I, I + k,[&](const int &a, const int &b) {
            return compare(a, b, D);
        });

        //sort distances
        std::sort(D, D + k, std::greater<float>());
        
        
        //find elbow
        float prev=0.0;
        int keep_indexes=0;
        for(int i=1;i<k;i++){
            if((D[i-1]-D[i])>=prev){
                keep_indexes++;
                prev=D[i-1]-D[i];
            }else{
                keep_indexes++; //inculsive (works in some cases)
                break;
            }
        }
        
        std::cout<<keep_indexes<<std::endl;
        // print results
        file.open("data/urls.bin",std::ios::binary);
        char* url= new char[d];
        for(int i=0;i<keep_indexes;i++){
            file.seekg(d* I[i], std::ios_base::beg);
            file.read(url,d);
            std::cout<<url<<std::endl;
        }
        file.close();

        for(int i=0;i<keep_indexes;i++){
            printf("%f\n", D[i]);
            fflush(stdout);
        }

        delete[] url;
    }

    delete[] database;
    delete[] queries;

    
    return 0;
}
