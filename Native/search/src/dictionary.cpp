#include "dictionary.h"

uint32_t Dictionary::hash(const std::string& str) const {
  uint32_t h = 2166136261;
  for (size_t i = 0; i < str.size(); i++) {
    h = h ^ uint32_t(int8_t(str[i]));
    h = h * 16777619;
  }
  return h;
}

Dictionary::Dictionary(const std::string& path)
:word2indx(bucket_size, -1),
words(bucket_size)
{
    std::fstream in(path, std::ios::in | std::ios::binary);
    if(!in) {
        std::cerr << "Cannot open file!" << std::endl;
        return;
    } 
    in.read((char*) &entrys, sizeof(int));
    in.read((char*) &dimensions, sizeof(int));

    dense_vectors= new float[entrys*dimensions];
    
    for(int i=0;i<entrys;i++){
        std::string word;
        char c;
        while((c = in.get()) != 0) {
            word.push_back(c);
        }
        int32_t id = get_id(word);
        words[id]=word;
        word2indx[id]=i;
        in.read((char*) &dense_vectors[dimensions*i], sizeof(float)*dimensions);
    }

    in.close();

    
}

Dictionary::~Dictionary(){
    delete[] dense_vectors;
}
 
int32_t Dictionary::get_id(const std::string& word){
    //get hash of word to find dense vector
    uint32_t h=hash(word);
    int32_t word2indxsize = word2indx.size();
    int32_t id = (int32_t) h % word2indxsize;
    while (word2indx[id] != -1 && words[word2indx[id]] != word) {
        id = (id + 1) %  word2indxsize;
    }
    return id;
}

const float* Dictionary::get_vector(const std::string& word){

    int32_t id = get_id(word);
    
    return &dense_vectors[(id*dimensions)];
}
