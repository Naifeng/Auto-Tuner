// On linux, compile with: g++ -g -Wall -std=c++11 candidate_set_simple_constraint.cpp -o candidate_set
// On linux, run with ./candidate_set

#include <iostream>
#include <string> 
#include <map> 
#include <cmath>
#include <random>
#include <fstream>

using namespace std;

// enum parallel
// {
//     split,
//     vectorize,
//     tile
// };

// TODO: move tree structure to another file
// TODO: write a simple makefile

struct node
{
   int data;
   string axis;
   std::vector<node> children;
};

struct node* new_node(int data) {
  struct node* node = new(struct node);    
  node->data = data;
  node->left = NULL;
  node->right = NULL;

  return(node);
}

struct node* insert(struct node* node, int data) {
  // 1. If the tree is empty, return a new, single node
  if (node == NULL) {
    return(newNode(data));
  }
  else {
    // 2. Otherwise, recur down the tree
    if (data <= node->data) node->left = insert(node->left, data);
    else node->right = insert(node->right, data);

    return(node); // return the (unchanged) node pointer
  }
}



int intlog(double base, double x) {
    return (int)(log(x) / log(base));
}


// TODO: add a param: boolean/int to indicate strictness
int compute_base(int n_params, std::vector<int> ranges, int t){
    float root = std::pow(t, 1.0/n_params);
    int r_max = *max_element(std::begin(ranges), std::end(ranges));
    float base = std::pow(r_max, 1.0/root);

    // cout << "Calculated base: " << base << endl;
    
    // raise/lower to the next power of 2
    return pow(2, ceil(log(base)/log(2)));

}




// output a csv
void generate_candidate_set(string str, int t, int range){
    map<int, std::string> parallelization;

   
    // extract a tree structure


    // TODO: v2 and v3 should not have constraints
    // TODO: a set of keyword strings
    // for keyword in keyword_set
    //  do while
    
    std::string keyword_split = "split"; 
  
    size_t pos_found = str.find(keyword_split); 
    while (pos_found != string::npos)
    {  
        parallelization.insert({pos_found, keyword_split});
        pos_found = str.find(keyword_split, pos_found+1); 
    }

    cout << parallelization.size() << " keyword(s) are identified: " << endl;
    // prints the elements 
    cout << "Position\tKeyword\n"; 
    for (auto itr = parallelization.begin(); itr != parallelization.end(); ++itr) { 
        cout << itr->first 
             << "\t\t" << itr->second << '\n'; 
    } 


    // TODO: tree structure
    // sampling to generate the training set
    // given
    
    
    // TODO: allow specifying range for each parameter
    // std::vector<int> ranges {1024, 1024, 1024, 1024};
    // std::vector<int> ranges {512, 512, 512, 512, 512, 512};



    // both work since parallelization size should match ranges size
    // int n_params = parallelization.size();
    std::vector<int> ranges{};
    int n_params = parallelization.size();
    for (int i = 0; i < n_params; ++i){
        ranges.push_back(range);
    }
    

    int base = compute_base(n_params, ranges, t);
    cout << "Proposed base: " << base << endl;

    // generate the data set 
    // should consider constraint
    // randomly


    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    
    std::string filename = "halide_blur_cpu_candidate_set.csv"; 
    std::ofstream ofile;
    ofile.open(filename);


    // without hashing: repetition may exist
    // TODO: implement constraints
    for (int i = 0; i < t; ++i){

        std::vector<int> candidate_params{};
        
        for (int j = 0; j < ranges.size(); ++j){

            int p_log_range = intlog(base,ranges[j]);

            int min = 1;
            int max; 
            
            if (j == 0) {
                max = p_log_range;
            }
            else {
                // This is the implementation of one depends on previous, there are many kinds of constraints
                // cout << "p_log_range " << p_log_range << " previous " << candidate_params[j-1] << endl;
                int previous_p_log_range = intlog(base,candidate_params[j-1]);
                max = std::min(p_log_range,previous_p_log_range);
                
                // no constraints
                // max = p_log_range;
            }

            std::uniform_int_distribution<int> uni(min,max); 

            int p_log_random = uni(rng);
            
            // TODO: overflow?
            int p = pow(base,p_log_random);
            

            //cout << p << endl;
            ofile << p <<  ",";

            // constraints
            candidate_params.push_back(p);

        }

        ofile << "\n";
        
    }

    ofile.close();

    cout << "Candidate set is successfully generated!" << endl;
}





// argument #1: targeted number of candidates
// argument #2: range for all parameters 
int main(int argc, char **argv) {

    int t = atoi(argv[1]);
    int range = atoi(argv[2]);

    cout << "Input schedule: " << endl;
    
    std::string str = "blur_x"
                    ".compute_at(blur_y, x_o)"
                    ".split(x, x_vo, x_vi, v1)"
                    ".vectorize(x_vi);"
                    "blur_y"
                    ".compute_root()"
                    ".split(x, x_o, x_i, v2)"
                    ".split(y, y_o, y_i, v3)" 
                    ".reorder(x_i, y_i, x_o, y_o)"
                    ".split(x_i, x_i_vo, x_i_vi, v4)"
                    ".vectorize(x_i_vi)"
                    ".parallel(y_o)"
                    ".parallel(x_o);";
    
    std::cout<< str << std::endl;

    generate_candidate_set(str, t, range);


    return 0;
}