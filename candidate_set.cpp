// On linux, compile with: g++ -g -Wall -std=c++11 candidate_set.cpp -o candidate_set
// On linux, run with ./candidate_set

#include "candidate_set.h"

int intlog(double base, double x) {
    return (int)(log(x) / log(base));
}


// TODO: add a param: boolean/int to indicate strictness
int compute_base(int t, std::vector<int> ranges){
    int n_params = ranges.size();
    float root = std::pow(t, 1.0/n_params);
    int r_max = *max_element(std::begin(ranges), std::end(ranges));
    float base = std::pow(r_max, 1.0/root);

    // raise/lower to the next power of 2
    return pow(2, ceil(log(base)/log(2)));

}

// output a csv
// [t] is the desired number of candidates
void generate_candidate_set(int t, std::vector<int> ranges){
    
    int base = compute_base(t, ranges);

    cout << "Proposed base: " << base << endl;
    cout << endl;
    
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    
    std::string filename = "candidate_set.csv"; 
    std::ofstream ofile;
    ofile.open(filename);

    // without hashing: repetition may exist
    for (int i = 0; i < t; ++i){

        for (long unsigned int j = 0; j < ranges.size(); ++j)
        {
            // TODO: different varying techniques for different parameter?
            // e.g., log for one param and linear for another
            int p_log_range = intlog(base,ranges[j]);
            int p_log_min = 1;

            std::uniform_int_distribution<int> uni(p_log_min,p_log_range); 
            int p_log_random = uni(rng); 
            
            int p = pow(base,p_log_random);
            
            // output to .csv file
            if (j == 0) ofile << p;
            else ofile << "," << p;
        }

        // cout << endl;
        ofile << "\n";
    
    } 

    // cout << "Candidate set is successfully generated!" << endl;
}

// argument #1: targeted number of candidates
// argument #2: a vector of range corresponding to each parameter
int main(int argc, char **argv) {

    int t = atoi(argv[1]);
    std::vector<int> ranges; 

    for (int i = 2; i < argc; ++i)
    {
        ranges.push_back(atoi(argv[i]));
    }

    // for (int i = 0; i < ranges.size(); ++i)
    // {
    //     std::cout << ranges[i] << std::endl; 
    // }
    
    generate_candidate_set(t, ranges);

    return 0;
}