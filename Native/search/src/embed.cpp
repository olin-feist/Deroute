#include "embed.h"


term_frequency::term_frequency(const std::string& text){
    std::istringstream text_stream (text);
    
    std::string sub_s;
    //generate term frequency map
    n_terms=0;
    while (text_stream >> sub_s) {
        
        
        auto ret = term_map.emplace(sub_s, 1);
        
        // Collision occurred
        if (!ret.second) {
            ret.first->second++;
        }
        n_terms++;
    }

}
term_frequency::~term_frequency(){}

void term_frequency::TF_Vector(fasttext::Vector& svec,const std::string& text){
    svec.zero();
    std::istringstream text_stream (text);
    const Args args_ = fastText_model.getArgs();
    std::shared_ptr<const Dictionary> dict_ = fastText_model.getDictionary();

  
        Vector vec(args_.dim);
        std::string sentence;
        std::getline(text_stream, sentence);
        std::istringstream iss(sentence);
        std::string word;
        int32_t count = 0;
        std::cerr<<"NEW"<<std::endl;
        std::cerr<<term_map.size()<<" "<<n_terms<<std::endl;
        while (iss >> word) {
            
            fastText_model.getWordVector(vec, word);
            float freq_w = (float) term_map.at(word)/n_terms;
            std::cerr<<word<<" "<<term_map.at(word)<<" "<<freq_w<<std::endl;
           
            real norm = vec.norm();
            if (norm > 0) {
                vec.mul(1.0 / norm);
                vec.mul(freq_w);
                svec.addVector(vec);
                count++;
            }
        }
        if (count > 0) {
            svec.mul(1.0 / count);
        }
    
}


void load_model(char* path){
    fastText_model.loadModel(path);
    isFastTextInitialized=true;
}

int get_vector_size(){
    if(!isFastTextInitialized){
        std::cerr<<"Error: fastText model is not loaded"<<std::endl;
        std::cerr<<"Call load_model(char* path) to fix"<<std::endl;
        return -1;
    }
    return fastText_model.getDimension();
}

std::string pre_process(std::string sentence){


    int pos;
    while ((pos = sentence.find("=")) != std::string::npos)
        sentence.replace(pos, 1, "equals");                 // replace = with equals

    while ((pos = sentence.find("+")) != std::string::npos)
        sentence.replace(pos, 1, "plus");                   // replace + with plus


    std::regex non_ascii(R"([^\w])"); // remove non ascii characters
    sentence = std::regex_replace(sentence, non_ascii, " ");

    std::transform(sentence.begin(), sentence.end(), sentence.begin(), // set lowercase
    [](unsigned char c){ return std::tolower(c); });

    std::regex non_letter(R"([^a-zA-Z])"); // remove non letter
    sentence = std::regex_replace(sentence, non_letter, " ");
    
    std::regex single_words(R"(\b[b-hj-z]\b)"); // remove single words
    sentence = std::regex_replace(sentence, single_words, "");

    //stop words
    std::string stopwords[]={
    "a","about","above","after","again","against","all","am","an","and","any","are","arent","as","at","be","because","been","before","being","below","between","both","but","by","cant",
    "cannot","could","couldnt","did","didnt","do","does","doesnt","doing","dont","down","during","each","few","for","from","further","had","hadnt","has","hasnt","have","havent",
    "having","he","hed","hes","her","here","heres","hers","herself","him","himself","his","how","hows","i","id","ill","im","ive","if","in","into","is","isnt","it","its","itself",
    "lets","me","more","most","mustnt","my","myself","no","nor","not","of","off","on","once","only","or","other","ought","our","ours","ourselves","out","over","own","same","shant",
    "she","shed","shell","shes","should","shouldnt","so","some","such","than","that","thats","the","their","theirs","them","themselves","then","there","theres","these","they",
    "theyd","theyll","theyre","theyve","this","those","through","to","too","under","until","up","very","was","wasnt","we","wed","well","were","weve","were","werent","what",
    "whats","when","whens","where","wheres","which","while","who","whos","whom","why","whys","with","wont","would","wouldnt","you","youd","youll","youre","youve","your","yours",
    "yourself","yourselves"
    };

    //remove stop words
    for(std::string word:stopwords){
        std::regex stop_words(R"(\b)" + word + R"(\b)");
        sentence = std::regex_replace(sentence, stop_words, "");
    }

    std::regex extra_spaces(R"(' +')"); // remove extra spaces
    sentence = std::regex_replace(sentence, non_letter, " ");

    sentence = std::regex_replace(sentence, std::regex("^ +| +$|( ) +"), "$1"); //remove trailing and leading whitespaces
    
    return sentence;

}

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
        
        int dimension=vec.size();
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

float* getVector(char* output, char* sentence){

    if(!isFastTextInitialized){
        std::cerr<<"Error: fastText model is not loaded"<<std::endl;
        std::cerr<<"Call load_model(char* path) to fix"<<std::endl;
        return NULL;
    }

    int dimensions=fastText_model.getDimension();
    Vector svec(dimensions);
   
    std::string sentence_string(sentence);
    sentence_string=pre_process(sentence_string);

    term_frequency tf(sentence_string);
    tf.TF_Vector(svec,sentence_string);
    //std::stringstream fstring_string(sentence_string);
    //fastText_model.getSentenceVector(fstring_string, svec); // compute first sentence vector
    
    //normalize
    float norm  = svec.norm();
    if (norm > 0) {
        svec.mul(1.0 / norm);
    }
    
    //if output path specified
    if(output[0]!='\0'){
        storeVector(output,svec);
        return nullptr;
    //write to stdout
    }else{
        float* ret = (float*) malloc(dimensions*sizeof(float));
        memcpy(ret, svec.data(), dimensions*sizeof(float));
        return ret;
    }

}



