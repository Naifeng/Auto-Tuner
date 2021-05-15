// TODO: eventually use Cog to generate this

#include <ctime>
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
    int NUM = 300; // change

    for (int i = 0; i < NUM; i++) {

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

        // vvv kernel starts vvv

        // ^^^ kernel ends ^^^

        double time = 0;
        int rounds = 1; // here

        for (int j = 0; j < rounds; j++){

            std::clock_t t1 = std::clock();
            // vvv computation starts vvv


            // ^^^ computation ends ^^^
            std::clock_t t2 = std::clock();

            time += ( t2 - t1 ) / (double) CLOCKS_PER_SEC; // unit: s

        }

        double avgtime = time/rounds; 

        unsigned long int cons = var[0]*var[1];
        ofile << avgtime << "," << var[0] << "," << var[1] << "," << cons << '\n';
    }


    ofile.close();

    printf("Profiling finished!\n");

    return 0;
}