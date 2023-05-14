#include "dictionary.h"
#include <cmath>

//Fowler–Noll–Vo hash function
uint32_t Dictionary::hash(const std::string& str) const {
  uint32_t h = 2166136261;
  for (size_t i = 0; i < str.size(); i++) {
    h = h ^ uint32_t(int8_t(str[i]));
    h = h * 16777619;
  }
  return h;
}

Dictionary::Dictionary()
:dimensions(0),
entrys(0),
BUCKETS(0)
{}

Dictionary::~Dictionary(){}

int Dictionary::load(const std::string& path){
    std::fstream in(path, std::ios::in | std::ios::binary);
    if(!in) {
        std::cerr << "Cannot open file!" << std::endl;
        return 1;
    }

    in.read((char*) &entrys, sizeof(int));
    in.read((char*) &dimensions, sizeof(int));
    
    BUCKETS= std::ceil(entrys / LOAD_FACTOR);
    
    word2indx.assign(BUCKETS, -1);
    
    words.resize(entrys);
    dense_vectors.resize(dimensions*entrys);
    
    for(int i=0;i<entrys;i++){
        
        std::string word;
        //get word
        char c;
        while((c = in.get()) != 0) {
            word.push_back(c);
        }
        //load in vector
        int32_t id = get_id(word);
        words[i]=word;
        word2indx[id]=i;
        in.read((char*) &dense_vectors[dimensions*i], sizeof(real)*dimensions);
    }

    in.close();
    return 0;
}

int32_t Dictionary::get_id(const std::string& word) const{
    //get hash of word to find dense vector
    uint32_t h=hash(word);
    
    int32_t id = h % BUCKETS;
    int32_t t=h % BUCKETS;
    //look through hash table
    while (word2indx[id] != -1 && words[word2indx[id]] != word) {
        id = (id + 1) %  BUCKETS;
    }
    return id;
}

void Dictionary::get_vector(Vector& vec, const std::string& word) const{
    
    vec.zero();
    int32_t id = get_id(word); // get word id

    //not a word in the dictionary
    if(word2indx[id]==-1)
        return;

    //get vector
    for(int i=0;i<vec.size();i++){
        vec[i]+=(float) dense_vectors[(word2indx[id]*dimensions)+i];
    }
    
}

int Dictionary::get_dimensions() const{
    return dimensions;
}
