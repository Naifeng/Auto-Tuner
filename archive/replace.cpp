// On os x, compile with: g++ -g -Wall -std=c++11 replace.cpp -o replace
// On os x, run with ./replace ../CMakeLists.txt HalideSAR 8 8


#include <iostream>
#include <string> 
#include <map> 
#include <cmath>
#include <random>
#include <fstream>
#include <queue>
#include <set> 
#include <stack> 

using namespace std;

// void autotune(string &str, int t, int range){

//     Node* root = extract_pipeline(str);
//     generate_candidate_set(str, t, range, root);

//     // verify
//     std::cout<< "Example: " << std::endl;
//     preorder_traverse(root);

//     cout << "After extracting: " << endl;
//     cout << str << endl;

// }



int main(int argc, char **argv) {


    // TODO: now automatically set
    // argument #1: targeted number of candidates = 1000
    // argument #2: range for all parameters = 1024

    // int t = atoi(argv[1]);
    // int range = atoi(argv[2]);

    // int t = 500;
    // int range = 256; 

    // TODO: check by 1. in the algorithm 2. in the schedule and followed by a .compute
    
    string key_str = "";

    // ifstream file; 
    string word, filename; 
    string vector_param, block_param;
    string env;
    string parallel_keyword = "";
    string compute_keyword = "";
    string vectorsize;
    string blocksize;

  
    // filename of the file 
    // filename = "blur_cpu.cpp";
    filename = argv[1];
    env = argv[2];
    vector_param = argv[3];
    block_param = argv[4];

    // TODO: use switch statements
    if (env == "cpp"){
        parallel_keyword = "N/A";
        compute_keyword = "// blur";
    }

  
    if (env == "Halide"){
        parallel_keyword = ".compute_root()";
        compute_keyword = "realize";
    }

    if (env == "HalideSAR"){
        vectorsize = "vectorsize";
        blocksize = "blocksize";
    }

    std::fstream file("../../CMakeLists.txt", std::ios::in);


    if(file.is_open()) {
        std::string replace = vectorsize;
        std::string replace_with = vectorsize + "=" + vector_param;
        
        std::string replace_2 = blocksize;
        std::string replace_with_2 = blocksize + "=" + block_param;

        std::string line;
        std::vector<std::string> lines;

        // flag
        int flag = 0; 

        while(std::getline(file, line)) {
            // std::cout << line << std::endl;

            std::string::size_type pos = 0;

            while ((pos = line.find(replace, pos)) != std::string::npos){
                line.replace(pos, line.size(), replace_with);
                pos += replace_with.size();
            }

            pos = 0;

            while ((pos = line.find(replace_2, pos)) != std::string::npos){
                line.replace(pos, line.size(), replace_with_2);
                pos += replace_with_2.size();
            }

            lines.push_back(line);
        }

        file.close();
        file.open("../../CMakeLists.txt", std::ios::out | std::ios::trunc);

        for(const auto& i : lines) {
            file << i << std::endl;
        }
    
    }

    // opening file 
    // file.open(filename.c_str()); 
    // // word by word, maybe string is better?
    // // extracting words from the file 
    // bool clip = false; 
    // while (file >> word) 
    // {   
    //     cout << word << endl;

    //     if (word.find(vectorsize) != string::npos){
            
    //     }

    //     if (clip){
    //         // abandon using key_str
    //         // directly changing the original file
    //         key_str += word;
    //     }

    //     // if (word.find(parallel_keyword) != std::string::npos){
    //     //     clip = false;
        
    //     // }
    //     // cout << word << endl; 
    // } 
    // file.close();


    // cout << "Input schedule: " << endl;
    // std::cout<< key_str << std::endl;
    // std::cout << endl;

    // if (env == "Halide") autotune(key_str, t, range);

    // TODO: separate as the generator
    // write to generator

    // in 
    // ifstream taskfile (filename);
    // // ifstream helper ("gen_helper_" + env + ".cpp");
    // // out 
    // // ofstream ofile (filename +  "_gen.cpp");  
    // ofstream ofile ("../CMakeLists.txt");  
    // string line;
    // // bool pass = false;
    // while (getline(taskfile, line))
    // {   

    //     ofile << line;
    //     ofile << endl;

    //     if (line.find("int blocksize = 64;") != std::string::npos){
    //         ofile << key_str;
    //         ofile << endl;
    //         break;
    //     }

    // } 

    // // helper.close();
    // taskfile.close();
    // ofile.close();

    return 0;
}