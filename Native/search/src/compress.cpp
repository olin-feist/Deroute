#include <iostream>
#include <fstream>
#include <string.h>
#include <iostream>
#include <regex>
#include <sstream>

void compress(std::string path, std::string out_path){
    std::ifstream file;

    file.open(path);
    if(!file) {
        std::cerr << "Cannot open file!" << std::endl;
        return;
    } 
    std::string line;
    std::string word;
    int dimensions;
    int entrys;
    std::istringstream iss;

    std::getline(file, line);
    iss=std::istringstream(line);
    
    iss >> word;
    entrys= stoi(word);
    iss >> word;
    dimensions= stoi(word);

    std::vector<std::pair<std::string,float*>> saved;
    
    int count=0;
    for(int i=0;i<entrys;i++){
        std::getline(file, line);
        iss=std::istringstream(line);
        iss >> word;

        std::regex regex("^[A-Za-z]+$");

        if (std::regex_match(word, regex)) { 
            float* temp =  new float[dimensions];
            std::string float_s;
            for(int j=0;iss >> float_s;j++){
                if(j>=dimensions){
                    std::cerr<<"Error: Dimensions Mismatch"<<std::endl;
                }

                float f=(float)stof(float_s);
            
                temp[j]=f;
            }
            count++;
            saved.emplace_back(word, temp);
        } else {
            continue;
        }
    }
    file.close();

    std::fstream out(out_path, std::ios::out | std::ios::binary);
    out.write((char*) &count, sizeof(int));
    out.write((char*) &dimensions, sizeof(int));
    for(const auto& p: saved){
        out.write((char*) p.first.c_str(), p.first.length());
        out.put(0);
        out.write((char*) p.second, dimensions*sizeof(float));
        delete[] p.second;
    }
    
    std::cout<<"Saved "<<count<<" vectors of "<<dimensions<<" dimensions"<<std::endl;
    std::cout<<"Model is now "<<out.tellg()/1000000<<" MBs"<<std::endl;
    out.close();

}

int main(int argc, char* argv[]){
    if(argc>3){
        std::cerr<<"Error: expected <in> <out>"<<std::endl;   
        return 1;
    }

    compress(argv[1],argv[2]);
}