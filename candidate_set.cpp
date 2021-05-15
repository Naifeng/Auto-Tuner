// On linux, compile with: g++ -g -Wall -std=c++11 candidate_set.cpp -o candidate_set
// On linux, run with ./candidate_set

#include "candidate_set.h"

// enum parallel
// {
//     split,
//     vectorize,
//     tile
// };

// TODO: move tree structure to another file
// TODO: write a simple makefile

Node* newNode(int constraint) {
  Node* node = new Node;    
  node->constraint = constraint;
  return node;
}

// Prints the n-ary tree level wise 
void inorder_traverse(Node * root) 
{ 
    if (root==NULL) 
        return; 
   
    // Standard level order traversal code 
    // using queue 
    queue<Node *> q;  // Create a queue 
    q.push(root); // Enqueue root  
    while (!q.empty()) 
    { 
        int n = q.size(); 
  
        // If this node has children 
        while (n > 0) 
        { 
            // Dequeue an item from queue and print it 
            Node * p = q.front(); 
            q.pop(); 
            cout << p->dimension << "-" << p->constraint << " "; 
   
            // Enqueue all children of the dequeued item 
            for (long unsigned int i=0; i<p->children.size(); i++) 
                q.push(p->children[i]); 
            n--; 
        } 
   
        cout << endl; // Print new line between two levels 
    } 
} 

// Function to traverse tree without recursion 
void preorder_traverse(Node* root) 
{ 
    // Stack to store the nodes 
    stack<Node*> nodes; 
  
    // push the current node onto the stack 
    nodes.push(root); 
  
    // loop while the stack is not empty 
    while (!nodes.empty()) { 
  
        // store the current node and pop it from the stack 
        Node* curr = nodes.top(); 
        nodes.pop(); 
  
        // current node has been travarsed 
        if(curr != NULL) 
            { 
            cout << curr->dimension << "-" << curr->constraint << " "; 
      
            // store all the childrent of current node from 
            // right to left. 
            vector<Node*>::iterator it = curr->children.end(); 
      
            while (it != curr->children.begin()) { 
                it--; 
                nodes.push(*it); 
            } 
        } 
    } 

    cout << endl;
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


std::string find_number(std::string const & str)
    {
      char const* digits = "0123456789";
      std::size_t const n = str.find_first_of(digits);
      if (n != std::string::npos)
      {
        std::size_t const m = str.find_first_not_of(digits, n);
        return str.substr(n, m != std::string::npos ? m-n : m);
      }
      return std::string();
    }

// extract the pipeline and generate a tree structure accordingly
// meanwhile, help generate the generator
Node* extract_pipeline(string &str){

    string str_copy = str;

    std::string delimiter = ".";
    // TODO: use a set/vector 
    // std::string par_keyword = "split"; 
    std::vector<std::string> par_keywords = {"vectorize", "parallel"}; 

    size_t pos = 0;
    std::string token;

    // add one return node
    // currently, set all constraint to 0
    struct Node* root = newNode(0);
    root->dimension = "root";

    // dimension set
    set <string> dimension_set;
    int c = 1; // for var
    while ((pos = str_copy.find(delimiter)) != std::string::npos) {
        // seperated by "."
        token = str_copy.substr(0, pos);
        // cout << "token: " << token << endl;

        // work with each token that contains keyword
        size_t pos_found = token.find(par_keywords[0]);
        size_t pos_found_ = token.find(par_keywords[1]);
        // cout << "par: " << par_keywords[0] << endl;
        if (pos_found != string::npos || pos_found_ != string::npos){
        // if (pos_found != string::npos){

            // trim
            size_t pos_end = token.find(";");
            token = token.substr(0, pos_end);
            string token_copy = token;

            // cout << "token: " << token << endl;
            // modify token to insert argv
            // string param_found = find_number(token);
            
            // TODO: change today if possible
            string param_found = "vectorsize";
            // string param_found = "blocksize";
            if (pos_found_ != string::npos) param_found = "blocksize";
            // cout << find_number(token) << endl;

            size_t param_pos = token.find(param_found);
            if (param_pos != string::npos){

                // string argv = "atoi(argv[" + to_string(c) + "])";
                // token.replace(param_pos, param_found.length(), argv);

                // string var = "var[" + to_string(c) +"]";
                // token.replace(param_pos, param_found.length(), var);
                c++;

                std::cout << "identified: " << token;
                // Assume that split() in each compute sequence is unique

                // add token back to the input string
                size_t token_pos = str.find(token_copy);
                str.replace(token_pos, token_copy.length(), token);
                
                // check dimension
                unsigned start = token.find("(") + 1; // 1 is the length of "(", hard-coded now
                unsigned end = token.find_first_of(",", start);
                string dimension = str_copy.substr(start,end-start);

                std::cout << " upon dimension " << dimension << std::endl;

                // create a new node
                struct Node* compute_node = newNode(0);
                compute_node->dimension = dimension;

                // check if new dimension contains added dimension
                bool flag = false;
                struct Node* start_node;
                for (long unsigned int i = 0; i < root->children.size(); ++i){
                    if (dimension.find(root->children[i]->dimension) != std::string::npos){
                        
                        //std::cout << "investigate: " << root->children[i]->dimension << std::endl;
                        //std::cout << "investigate dim: " << dimension << std::endl;
                        
                        start_node = root->children[i];
                        flag = true;
                        break;
                    }
                }

                // dimension already added
                if (flag){
                    // traverse to the end
                    // hard-coded
                    start_node->children.push_back(compute_node);
                }
                else{
                    root->children.push_back(compute_node);
                }

            }
            
        }

        str_copy.erase(0, pos + delimiter.length());

    }
    
    // last piece
    // std::cout << str << std::endl;
    std::cout << std::endl;

    // tree constructed!
    return root;
}



// output a csv
void generate_candidate_set(string str, int t, int range, Node* root){
    
    // TODO: multiple root nodes
    // TODO: a nicer graphic illustration of tree structure

    // TODO: allow specifying range for each parameter
    // std::vector<int> ranges {1024, 1024, 1024, 1024};
    // std::vector<int> ranges {512, 512, 512, 512, 512, 512};


    // both work since parallelization size should match ranges size
    // int n_params = parallelization.size();
    // std::vector<int> ranges{};
    // int n_params = parallelization.size();
    // for (int i = 0; i < n_params; ++i){
    //     ranges.push_back(range);
    // }
    
    // TODO: n_params could be the tree cardinality
    // int base = compute_base(n_params, ranges, t);
    int base = 4;
    // cout << "Proposed base: " << base << endl;
    // cout << endl;


    // TODO: should also generate the cons

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    
    std::string filename = "candidate_set.csv"; 
    std::ofstream ofile;
    ofile.open(filename);


    // without hashing: repetition may exist
    for (int i = 0; i < t; ++i){

        // queue to store the nodes 
        queue<Node*> nodes; 
      
        // push the current node onto the queue
        nodes.push(root); 

        int p_log_range = intlog(base,range);
        // constraint is the max log value
        root->constraint = p_log_range;
        bool first = true;
        
        // loop while the queue is not empty 
        while (!nodes.empty()) { 
      
            // store the current node and pop it from the queue 
            Node* curr = nodes.front(); 
            nodes.pop(); 
          
            // current node has been travarsed 
            if(curr != NULL) { 
                // cout << curr->dimension << " "; 

                // output to .csv file
                int min = 1;

                std::uniform_int_distribution<int> uni(min,curr->constraint); 
                int p_log_random = uni(rng);
                
                // TODO: overflow?
                int p = pow(base,p_log_random);

                // successfully generated!
                if (curr == root){
                    // if (root->dimension == "root") ofile << 8;
                    // else ofile << p;
                    p_log_random = p_log_range;
                    // ofile <<;
                }
                else{
                    if (first){
                        ofile << p;
                        first = false; 
                    }
                    else ofile << "," << p;
                }
                // cout << curr->dimension << ",";

                // store all the children of current node from 
                // right to left
                vector<Node*>::iterator it = curr->children.begin(); 
          
                for (; it != curr->children.end(); ++it) { 

                    // set constraint for children
                    (*it)->constraint = p_log_random;
                    
                    nodes.push(*it); 
                } 
            } 
        }
        // cout << endl;
        ofile << "\n";
    
    } 

    // cout << "Candidate set is successfully generated!" << endl;
}

// // argument #1: targeted number of candidates
// // argument #2: range for all parameters 
// int main(int argc, char **argv) {

//     int t = atoi(argv[1]);
//     int range = atoi(argv[2]);

    
    
//     // std::string str = "blur_x"
//     //                 ".compute_at(blur_y, x_o)"
//     //                 ".split(x, x_vo, x_vi, v1)"
//     //                 ".vectorize(x_vi);"
//     //                 "blur_y"
//     //                 ".compute_root()"
//     //                 ".split(x, x_o, x_i, v2)"
//     //                 ".split(y, y_o, y_i, v3)" 
//     //                 ".reorder(x_i, y_i, x_o, y_o)"
//     //                 ".split(x_i, x_i_vo, x_i_vi, v4)"
//     //                 ".vectorize(x_i_vi)"
//     //                 ".parallel(y_o)"
//     //                 ".parallel(x_o);";

//     std::string str = "blur_y"
//                     ".compute_root()"
//                     ".split(x, x_o, x_i, v2)"
//                     ".split(y, y_o, y_i, v3)" 
//                     ".reorder(x_i, y_i, x_o, y_o)"
//                     ".split(x_i, x_i_vo, x_i_vi, v4)"
//                     ".vectorize(x_i_vi)"
//                     ".parallel(y_o)"
//                     ".parallel(x_o);";
    
//     cout << "Input schedule: " << endl;
//     std::cout<< str << std::endl;
//     std::cout << endl;


//     Node* root = extract_pipeline(str);

//     generate_candidate_set(str, t, range, root);
    
//     std::cout<< "Example: " << std::endl;
//     preorder_traverse(root);
//     // inorder_traverse(root); 
    


//     return 0;
// }