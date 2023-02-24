#include <fastText.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <regex>
using namespace fasttext;
//.\bin\embed.exe bin/wiki.en.bin data/website_plain.txt data/vectors.bin append


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


//get sentence vectors
void getVector(std::string model_path,std::string output){
    FastText fasttext;
    fasttext.loadModel(model_path); //load model

    Vector svec(fasttext.getDimension());
   
    //get sentences
    while (std::cin.peek() != EOF) {
        
        std::string sentence;
        std::getline(std::cin, sentence);
        
        sentence=pre_process(sentence);
    
        std::stringstream fstring_string(sentence);
        fasttext.getSentenceVector(fstring_string, svec); // compute first sentence vector
        
        //normalize
        float norm  = svec.norm();
        if (norm > 0) {
            svec.mul(1.0 / norm);
        }
        
        //if output path specified
        if(!output.empty()){
            storeVector(output,svec);
            std::cout<<"Done"<<std::endl;
        //write to stdout
        }else{
            setmode(fileno(stdout), O_BINARY);
            int dimensions=svec.size();
            int n=1;
            std::cout.write((char*) &dimensions, sizeof(int));
            std::cout.write((char*) &n, sizeof(int));
            for(int i=0;i<dimensions;i++){
                std::cout.write((char*) &svec[i], sizeof(float));
            }
        }

    }

}


int main(int argc, char* argv[]){
    
    if(argc>3){
        std::cerr<< "Invalid Number of Args" << std::endl;
        std::cerr<< "usage: embed <model_path> <optional_output_path>" << std::endl;
        return 1;
    }
    
    //if output path specified
    if(argc==3){
        getVector(argv[1],argv[2]);

    //write to stdout
    }else{
        getVector(argv[1],"");
    }
    
    return 0;
}
