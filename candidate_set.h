#ifndef CANDIDATE_SET_H
#define CANDIDATE_SET_H

#include <iostream>
#include <string> 
#include <map> 
#include <cmath>
#include <random>
#include <fstream>
#include <queue>
#include <set> 
#include <stack> 
#include <sstream>
#include <algorithm>

using namespace std;

struct Node;

struct Node
{
   int constraint;
   string dimension;
   std::vector<Node*> children;
};

Node* newNode(int constraint);
void generate_candidate_set(string str, int t, int range, Node* root);
Node* extract_pipeline(string &str);
void preorder_traverse(Node* root);
std::string find_number(std::string const & str);

#endif