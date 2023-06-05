#include "utils.h"

constexpr int32_t DEROUTE_VERSION = 2;                      // Deroute file format version
constexpr int32_t DEROUTE_FILEFORMAT_MAGIC_ID = 53274414;  // File format id


int utils::print_database(std::string path){
    std::fstream file(path , std::ios::in | std::ios::binary);
    if(!file) {
        std::cerr << "Cannot open file!" << std::endl;
        return 1;
    }
    int d;
    int n;
    float * vec;
    std::vector<std::string> labels;

    if(utils::read_database(path,&d, &n,vec,labels)){
        std::cerr << "Error: Reading Database" << std::endl;
        return 1;
    }

    for(int i=0;i<n;i++){
        std::cout<<"Label: "<<labels[i]<<"Vector: ";
        for(int j=0;j<(d/10);j++){
            std::cout<<vec[j*i];
        }
        std::cout<<std::endl;
    }
    delete[] vec;
    return 0;
        
}

int utils::mul_vector(float* v1, float mult, int size){
    for (int i = 0; i < size; i++) {
        v1[i] *= mult;
    }
    return 0;
}

int utils::add_vectors(float* v1, float* v2, int size){
    for (int i = 0; i < size; i++) {
        v1[i] += v2[i];
    }
    return 0;
}

int utils::read_database(std::string path,int *d, int *n,float * &vec,std::vector<std::string> &labels){
    std::fstream file;
    file.open(path, std::ios::in |std::ios::binary);
    if(!file) {
        std::cerr << "Error: Cannot open file!" << std::endl;
        return 1;
    } 
    if(!check_file(file)){
        return 1;
    }
    int dimensions;
    int entrys;
    file.read((char*) &dimensions, sizeof(int)); // dimensions
    file.read((char*) &entrys, sizeof(int)); // database size

    vec = new float[dimensions*entrys];

    //load in vectors and labels
    char* label= new char[300];
    for(int i=0;i<entrys;i++){
        //get label
        file.read(label,300);
        labels.push_back(label);

        //get vector
        file.read((char*) &vec[dimensions*i], sizeof(float)*dimensions);
        
    }
    delete[] label;
    file.close();
    *d=dimensions;
    *n=entrys;

    return 0;
}

int utils::read_database(std::string path,int *d, int *n,float * &vec,std::vector<std::string> &labels, int idx){    
    //get database files and check if they exist
    std::fstream database_file(path , std::ios::in | std::ios::binary);
    if(!database_file) {
        std::cerr << "Error: could not open database file" << std::endl;
        return 1;
    }
    if(!check_file(database_file)){
        return 1;
    }
    int dimensions;
    database_file.read((char*) &dimensions, 4);

    *d=dimensions; //set dimensions

    int entrys;
    database_file.read((char*) &entrys, 4); 

    int new_idx=(entrys-idx); // new elements that have been added

    *n=new_idx;
    if(new_idx==0){
        std::cerr << "Warning: no new vectors to add" << std::endl;
        return 0;
    }
    
    vec = new float[new_idx*dimensions];
    char* label= new char[300];

    database_file.seekp(8+(sizeof(float)*((idx)*dimensions))+(300*idx), std::ios_base::beg); //skip to new entries

    //get new vectors
    for(int i=0;i<new_idx;i++){
        
        //get label and add it
        database_file.read(label,300);
        labels.push_back(label);

        //get vector
        database_file.read((char*) &vec[dimensions*i], sizeof(float)*dimensions);

    }

    database_file.close();

    delete[] label;

    return 0;
}

int utils::write_database(std::string path, const Vector &vec, std::string label){
    std::fstream append_f(path, std::ios::out | std::ios::in | std::ios::binary);
    
    //if appending new vector to existing file
    if(append_f.good()){
        if(!append_f) {
            std::cerr << "Cannot open file!" << std::endl;
            return 1;
        }

        if(!check_file(append_f)){
            return 1;
        }

        //get dimensions
        int dimensions;
        append_f.read((char*) &dimensions, 4);


        //get entrys and increment
        int entrys;
        append_f.read((char*) &entrys, 4);
        entrys++;

        char* c_label= new char[300];

        //remove carriage returns and newlines
        label.erase(std::remove(label.begin(), label.end(), '\r'), label.end());
        label.erase(std::remove(label.begin(), label.end(), '\n'), label.end());

        //go through all entrys
        for(int i=0;i<entrys-1;i++){
            //get label and convert to string
            append_f.read(c_label,300);
            std::string s = c_label;

            //check duplicates
            if(!s.compare(label)){
                std::cerr << "Error: Duplicate Entry" << std::endl;
                return 1;
            }

            append_f.seekp(sizeof(float)*dimensions, std::ios_base::cur); // skip vector
        }

        strcpy(c_label, label.c_str()); // set c string to label

        memset(c_label+label.size(), 0, 300-label.size()); // add trailing 0's to fill buffer

        append_f.write(c_label, 300); // write label

        //wirte vector
        for(int i=0;i<dimensions;i++){
            append_f.write((char*) &vec[i], sizeof(float));
        }

        //update entrys
        append_f.seekp(sizeof(int)+8, std::ios_base::beg);
        append_f.write((char*) &entrys, 4);

        append_f.close();

        delete[] c_label;

    //new file
    }else{

        std::ofstream write_f(path, std::ios::out | std::ios::binary);
        if(!write_f) {
            std::cerr << "Cannot open file!" << std::endl;
            return 1;
        }
        
        sign_file(write_f); // sign file
        
        //set dimensions
        int dimensions=vec.size(); 
        write_f.write((char*) &dimensions, sizeof(int));

        //set entrys
        int entrys=1;
        write_f.write((char*) &entrys, sizeof(int));
        
        char* c_label= new char[300]; // cstring to hold label

        strcpy(c_label, label.c_str()); // set c string to label

        memset(c_label+label.size()-1, 0, 300-label.size()); // add trailing 0's to fill buffer
       
        
        write_f.write(c_label, 300); // write label

        //write vector
        for(int i=0;i<dimensions;i++){
            write_f.write((char*) &vec[i], sizeof(float));
        }

        
        write_f.close();
        delete[] c_label;
    }
    append_f.close();
    return 0;
}

bool utils::check_file(std::fstream& in) {
    //get magic id
    int32_t magic;
    in.read((char*)&(magic), sizeof(int32_t));
    if (magic != DEROUTE_FILEFORMAT_MAGIC_ID) {
        std::cerr << "Error: file format ID does not match" << std::endl;
        return false;
    }
    //get file version
    int32_t version;
    in.read((char*)&(version), sizeof(int32_t));
    if (version != DEROUTE_VERSION) {
        std::cerr << "Error: Incorrect version" << std::endl;
        return false;
    }
    return true;
}

void utils::sign_file(std::ostream& out) {
    const int32_t magic = DEROUTE_FILEFORMAT_MAGIC_ID;
    const int32_t version = DEROUTE_VERSION;
    out.write((char*)&(magic), sizeof(int32_t));
    out.write((char*)&(version), sizeof(int32_t));
}