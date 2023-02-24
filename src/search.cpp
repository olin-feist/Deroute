#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <map>
#include <algorithm>
#include <faiss/IndexFlat.h>
#include <faiss/impl/AuxIndexStructures.h>

#include <vec_tools.h>
//search database_path urls_path query_path
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


    
    faiss::IndexFlatIP index(d); // call constructor
    index.add(nb, database); // add vectors to the index


    { // search queries

        faiss::RangeSearchResult result(nq);
        float range_val=0.0;
        index.range_search(nq, queries,range_val, &result);
        
        

        idx_t* I=result.labels; //indexs
        float* D=result.distances; //distances
        
        int k=result.lims[1]; // k
        std::vector<std::pair<float,idx_t>> search_results; //put results into a pair

        for(int i=0;i<k;i++)
            search_results.push_back(std::pair<float, idx_t>(D[i],I[i]));
        
        std::sort(search_results.begin(), search_results.end(),std::greater<>()); //sort

        
        //find elbow
        float prev=0.0;
        int keep_indexes=0;
        for (int i=1;i<k;i++){          
            float difference=(search_results[i-1].first)-(search_results[i].first);
            if(difference>=prev){
                keep_indexes++;
                prev=difference;
            }else{
                break;
            }
        }
        
        std::cout<<keep_indexes<<std::endl;
        // print results
        file.open(args[2],std::ios::binary);
        if(!file) {
            std::cerr << "Cannot open file!" << std::endl;
            return 1;
        } 
        
        char* url= new char[300];
        for(int i=0;i<keep_indexes;i++){
            file.seekg(300* search_results[i].second, std::ios_base::beg);
            file.read(url,300);
            std::cout<<url<<std::endl;
        }
        file.close();

        for(int i=0;i<keep_indexes;i++){
            printf("%f\n", search_results[i].first);
            fflush(stdout);
        }

        delete[] url;
    }

    delete[] database;
    delete[] queries;

    
    return 0;
}
