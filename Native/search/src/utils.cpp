#include "utils.h"

constexpr int32_t DEROUTE_VERSION = 2;                      // Deroute file format version
constexpr int32_t DEROUTE_FILEFORMAT_MAGIC_ID = 53274414;   // File format id
constexpr int32_t LABEL_BUFFER_SIZE = 300;                  // String buffer size of labels


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
            std::cout<<vec[(i*d)+j];
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
    int entries;
    file.read((char*) &dimensions, sizeof(int)); // dimensions
    file.read((char*) &entries, sizeof(int)); // database size

    vec = new float[dimensions*entries];

    //load in vectors and labels
    char* label= new char[LABEL_BUFFER_SIZE];
    for(int i=0;i<entries;i++){
        //get label
        file.read(label,LABEL_BUFFER_SIZE);
        labels.push_back(label);

        //get vector
        file.read((char*) &vec[dimensions*i], sizeof(float)*dimensions);
        
    }
    delete[] label;
    file.close();
    *d=dimensions;
    *n=entries;

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

    int entries;
    database_file.read((char*) &entries, 4); 

    int new_idx=(entries-idx); // new elements that have been added

    *n=new_idx;
    if(new_idx==0){
        std::cerr << "Warning: no new vectors to add" << std::endl;
        return 0;
    }
    
    vec = new float[new_idx*dimensions];
    char* label= new char[LABEL_BUFFER_SIZE];

    database_file.seekp((sizeof(float)*((idx)*dimensions))+(LABEL_BUFFER_SIZE*idx), std::ios_base::cur); //skip to new entries

    //get new vectors
    for(int i=0;i<new_idx;i++){
        
        //get label and add it
        database_file.read(label,LABEL_BUFFER_SIZE);
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

        if(dimensions!=vec.size()){
            append_f.close();
            std::cerr << "Error: attempting to add vectors of different dimensions" << std::endl;
            return 1;
        }

        //get entries and increment
        int entries;
        append_f.read((char*) &entries, 4);
        entries++;

        char* c_label= new char[LABEL_BUFFER_SIZE];

        //remove carriage returns and newlines
        label.erase(std::remove(label.begin(), label.end(), '\r'), label.end());
        label.erase(std::remove(label.begin(), label.end(), '\n'), label.end());

        //go through all entries
        for(int i=0;i<entries-1;i++){
            //get label and convert to string
            append_f.read(c_label,LABEL_BUFFER_SIZE);
            std::string s = c_label;

            //check duplicates
            if(!s.compare(label)){
                std::cerr << "Error: Duplicate Entry" << std::endl;
                append_f.close();
                return 1;
            }

            append_f.seekp(sizeof(float)*dimensions, std::ios_base::cur); // skip vector
        }

        strcpy(c_label, label.c_str()); // set c string to label

        memset(c_label+label.size(), 0, LABEL_BUFFER_SIZE-label.size()); // add trailing 0's to fill buffer

        append_f.write(c_label, LABEL_BUFFER_SIZE); // write label

        //write vector
        for(int i=0;i<dimensions;i++){
            append_f.write((char*) &vec[i], sizeof(float));
        }

        //update entries
        append_f.seekp(sizeof(int)+8, std::ios_base::beg);
        append_f.write((char*) &entries, 4);

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

        //set entries
        int entries=1;
        write_f.write((char*) &entries, sizeof(int));
        
        char* c_label= new char[LABEL_BUFFER_SIZE]; // cstring to hold label

        strcpy(c_label, label.c_str()); // set c string to label

        memset(c_label+label.size()-1, 0, LABEL_BUFFER_SIZE-label.size()); // add trailing 0's to fill buffer
       
        
        write_f.write(c_label, LABEL_BUFFER_SIZE); // write label

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