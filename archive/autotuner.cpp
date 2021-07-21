// On os x, compile with: g++ -g -Wall -std=c++11 autotuner.cpp candidate_set.cpp -o autotuner
// On os x, run with ./autotuner ../CMakeLists.txt HalideSAR

#include "candidate_set.h"

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

void autotune(string &str, int t, int range){

    Node* root = extract_pipeline(str);
    generate_candidate_set(str, t, range, root);

    // verify
    std::cout<< "Example: " << std::endl;
    preorder_traverse(root);

    cout << "After extracting: " << endl;
    cout << str << endl;

}

// TODO: write a new function to generate candidate by varying within several ranges
// which should be similar to the 'main' function here
// candidate.cpp is used to automatically identify Halide tree structure

int main(int argc, char **argv) {


    // TODO: now automatically set
    // argument #1: targeted number of candidates = 1000
    // argument #2: range for all parameters = 1024

    // int t = atoi(argv[1]);
    // int range = atoi(argv[2]);

    int t = 1000;
    int range = 256; 
    int base = 4;

    // TODO: check by 1. in the algorithm 2. in the schedule and followed by a .compute
    
    string key_str = "";

    // ifstream file; 
    string word, filename; 
    string env;
    string parallel_keyword = "";
    string compute_keyword = "";
    string vectorsize;
    string blocksize;

  
    // filename of the file 
    // filename = "blur_cpu.cpp";
    filename = argv[1];
    env = argv[2];

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
        // vectorsize = "vectorsize";
        // blocksize = "blocksize";


        std::string filename = "candidate_set.csv"; 
        std::ofstream ofile;
        ofile.open(filename);

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(1, int(range/base)); // define the range

        // without hashing: repetition may exist
        for (int i = 0; i < t; ++i){
            // std::cout << distr(gen) << ' '; // generate numbers
            ofile << distr(gen)*4 << "," << distr(gen)*4 << "\n";
        }

        ofile.close();
    }

    return 0;
}