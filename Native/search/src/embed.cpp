#include "embed.h"

namespace deroute{
// -------------------------- Term Frequency --------------------------
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

    //calculate outlier ceiling
    float avg,sd=0.0;
    for(const auto& [key,freq]: term_map){
        avg+=freq;
    }
    avg=avg/term_map.size();
    
    for(const auto& [key,freq]: term_map) {
        sd += pow(freq - avg, 2);
    }
    sd=sqrt(sd / n_terms);
    
    outlier=ceil((5*sd)+avg); //max weight for words

}

term_frequency::~term_frequency(){}

float term_frequency::get_weight(const std::string& word){
    int freq=term_map.at(word);
    if(freq>outlier){
        freq=outlier;
    }
    float freq_w = (float) freq/n_terms;

    return freq_w;
    
}

float term_frequency::get_weight(int freq){
    if(freq>outlier){
        freq=outlier;
    }
    float freq_w = (float) freq/n_terms;

    return freq_w;
    
}
// -------------------------- Term Frequency --------------------------



// -------------------------- Embedding Calls --------------------------

Model::Model():
isInitialized(false)
{}

Model::~Model()
{}

std::string Model::pre_process(std::string sentence) const{

    int pos;
    while ((pos = sentence.find("=")) != std::string::npos)
        sentence.replace(pos, 1, "equals");                 // replace = with equals

    while ((pos = sentence.find("+")) != std::string::npos)
        sentence.replace(pos, 1, "plus");                   // replace + with plus


    std::regex non_ascii(R"([^\w])"); // remove non ascii characters
    sentence = std::regex_replace(sentence, non_ascii, " ");

    std::transform(sentence.begin(), sentence.end(), sentence.begin(), // set lowercase
    [](unsigned char c){ return std::tolower(c); });

    std::regex non_letter("[^a-zA-Z]+"); // remove non letter
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
    "yourself","yourselves","many"
    };

    //remove stop words
    std::string pattern = "\\b(";
    for (const std::string& word : stopwords) {
        pattern += word + "|";
    }
    pattern.pop_back();
    pattern += ")\\b";
    std::regex re(pattern);
    sentence  =  std::regex_replace(sentence, re, "");

    std::regex extra_spaces(R"(' +')"); // remove extra spaces
    sentence = std::regex_replace(sentence, non_letter, " ");
    
    //sentence= std::regex_replace(sentence, std::regex(R"(^\\s+|\\s+$)"), ""); //remove trailing and leading whitespaces

    //sentence = std::regex_replace(sentence, std::regex(R"(^\s*$)"), ""); //remove empty lines
    return sentence;

}

int Model::store_vector(std::string path, const Vector &vec, const std::string label) const{
    return utils::write_database(path,vec,label);
}

void Model::vectorize(std::string text, Vector &dvec) const{
    dvec.zero();

    text=pre_process(text);  // Pre process text
    
    term_frequency tf(text); // Build term frequency of text
    
    Vector vec(vector_dict.get_dimensions());  // Word embedding
    int32_t word_count;                        // Count of words that can be represented in text

    word_count = 0; 
    
    // For every word in term frequency map (set of unique words in text)
    for (const auto & [ word, freq ]: tf.get_map()) {
        vec.zero();
        vector_dict.get_vector(vec, word); // Get word embedding

        // Normalize and scale by weight
        float norm = vec.norm();
        if (norm > 0) {
            vec.mul(1.0 / norm);
            vec.mul(tf.get_weight(freq)); // Get term frequency weight
            vec.mul(freq);                // Scale by frequency to simulate adding number of words
            dvec.addVector(vec);
            word_count+=freq;
        }
    }

    // Average of word embeddings in text
    if (word_count > 0) {
        dvec.mul(1.0 / word_count);
    }

    // Normalize
    float norm  = dvec.norm();
    if (norm > 0) {
        dvec.mul(1.0 / norm);
    }
}

bool Model::is_loaded() const{
    return isInitialized;
}

int Model::load(const std::string& path){
    if(!vector_dict.load(path)){
       isInitialized=true;
       return 0;
    }else{
        return 1;
    }
}

int Model::get_dimensions() const{
    return vector_dict.get_dimensions();
}
// -------------------------- Embedding Calls --------------------------

}


