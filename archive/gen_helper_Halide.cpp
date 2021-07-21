// TODO: eventually use Cog to generate this

#include "Halide.h"
#include "clock.h"

using namespace Halide;

#include <stdio.h>
#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>


int main(int argc, char **argv) {

    // subject to change: the output file name
    std::string FILENAME = "autotuner/profiling_result.csv"; 
    std::ofstream ofile;
    ofile.open(FILENAME);

    std::ifstream ifile ( "autotuner/training_set.csv" );
    // how many data instances to generate
    int NUM = 50; // change
    // input size ranges from 2^10 to 2^13, can generalize to 10 - 15
    for (int k = 0; k <= 3; k++){ //change
        int p_input = 10 + k;
        // int p_input = 10; // for faster testing
        std::cout << "Input dimension: " << pow(2,p_input) << std::endl;
        
        for (int i = 1; i < NUM+1; i++) {

            if (i % 10 == 0) std::cout << i << " instances generated" << std::endl;
            
            std::string line;
            std::getline(ifile, line);

            std::istringstream ss(line);
            std::string token;
            std::vector <int> var;
            // TODO: use a vector
            while(std::getline(ss, token, ',')) {
                // std::cout << token << '\n';
                var.push_back(std::stoi(token));
            }
        
            // write to the console
            // std::cout << var[0] << "," << var[1] << "," << var[2] << "," << var[3] << std::endl;

            // vvv kernel starts vvv

            // ^^^ kernel ends ^^^


            double time = 0;
            int rounds = 5; 
            // for each data instance, run 5 times and take the average
            for (int j = 0; j < rounds; j++){
                // run
                double t1 = current_time();
                // vvv computation starts vvv

                // ^^^ computation ends ^^^
                double t2 = current_time();
                
                time += (t2 - t1)/1000;
            }

            double avgtime = time/rounds; 
            // std::cout << avgtime << std::endl;
            // define C
            // for blur operation, C = n^2, where n is the input dimension
            unsigned long int cons = pow(2,p_input)*pow(2,p_input);
            // write to the output file
            ofile << avgtime << "," << pow(2,p_input) << "," << 8 << "," << var[1] << "," << var[2] << "," << var[3] << ",";
            ofile << cons << '\n';
        }
    }
    ofile.close();

    printf("Profiling finished!\n");

    return 0;
}
