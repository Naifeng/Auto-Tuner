#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/config.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/tokenizer.hpp>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <fstream>
#include <limits>
#include "global.h"
#include "hardware.h"
#include "kernel.h"
#include "model.h"
#include "step.h"
#include "kernel_map.h"

// checking execution time
#include <chrono> 
using namespace std::chrono; 

typedef struct vertex_properties
{
	bool is_step = false, is_kernel = false, is_hardware = false;
	int id;
	Kernel_t *kernel;
	Hardware_t *hardware;
	Step_t *step;
}vertex_properties_t;

typedef struct edge_properties
{
	bool is_performance_model = false, is_kernel_map = false;
	int id;
	Performance_model_t *performance_model;
	Kernel_map_t *kernel_map;
}edge_properties_t;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_properties_t, edge_properties_t> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor_t;
typedef graph_t::edge_descriptor edge_descriptror_t;
typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iter;
typedef boost::graph_traits<graph_t>::edge_iterator edge_iter;

std::vector<vertex_descriptor_t> load_graph(graph_t &g, std::string filename)
{
	// boost::dynamic_properties dp;
	// dp.property("vectex_properties_is_step", get(&vertex_properties_t::is_step , g));
	// dp.property("vectex_properties_is_kernel", get(&vertex_properties_t::is_kernel, g));
	// dp.property("vectex_properties_is_hardware", get(&vertex_properties_t::is_hardware, g));
	// dp.property("vectex_properties_step", get(&vertex_properties_t::id, g));
	// dp.property("edge_properties", get(&edge_properties_t::is_kernel_map, g));
	// dp.property("edge_properties", get(&edge_properties_t::is_performance_model, g));
	// dp.property("edge_properties", get(&edge_properties_t::id, g));
	// std::ifstream in(filename+"_sturcture.xml", std::ios::in);
    // boost::read_graphml(in, g, dp);
    // in.close();
    
    std::ifstream in(filename + "_content.xml", std::ios::in);
    std::vector<vertex_descriptor_t> vertices;
    while (!in.eof())
    {
        std::string parent_type, type;
        in >> parent_type >> type;
        if (parent_type == "Hardware_t")
        {
            vertex_descriptor_t v = boost::add_vertex(g);
            if (type == "CPU_t")
            {
                int id;
                int total_cores;
	            double total_FLOPS;
                in >> id >> total_cores >> total_FLOPS;
                CPU_t *cpu = new CPU_t(total_cores, total_FLOPS);
                cpu->type = "CPU_t";
                cpu->id = id;
                g[v].is_hardware = true;
                g[v].hardware = cpu;
                g[v].id = cpu->id;
            }
            vertices.push_back(v);
        }
        else if (parent_type == "Step_t")
        {
            vertex_descriptor_t v = boost::add_vertex(g);
            if (type == "Blur_t")
            {
                int id;
                in >> id;
                Blur_t *blur = new Blur_t();
                blur->type = "Blur_t";
                blur->id = id;
                g[v].is_step = true;
                g[v].step = blur;
                g[v].id = blur->id;
            }
            vertices.push_back(v);
        }
        else if (parent_type == "Performance_model_t")
        {
            int src_id, dst_id;
            in >> src_id >> dst_id;
            std::pair<edge_descriptror_t, bool> e = boost::add_edge(vertices[src_id], vertices[dst_id], g);
            if (type == "MLP_t")
            {
                int id;
                std::string pb, chkpt;
                in >> id >> pb >> chkpt;
                MLP_t *mlp = new MLP_t(const_cast<char *>(pb.c_str()), const_cast<char *>(chkpt.c_str()));
                mlp->type = "MLP_t";
                mlp->id = id;
                mlp->src_id = src_id;
                mlp->dst_id = dst_id;
                g[e.first].is_performance_model = true;
                g[e.first].performance_model = mlp;
            }
        }
    }
    return vertices;
}


// input: data dimension
// output: predicted best schedule
std::vector<float> select_variant(float input_dimension, graph_t KB, std::vector<vertex_descriptor_t> vertices){
    

    // hard-coded
    std::string data("../../candidate_set.csv");
    std::ifstream in(data.c_str());
    // if (!in.is_open()) return {0};
    
    typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;
    std::vector<std::string > vec;
    std::string line;

    float min_runtime = std::numeric_limits<float>::max();
    std::vector<float> selected_variant;
    std::vector<std::vector<float> > candidate_set;

    struct metadata_t metadata;

    while (getline(in,line))
    {   
        std::cout << line << std::endl;
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
                
        std::vector<float> candidate;

        for(int i = 0; i != vec.size(); ++i) {
            candidate.push_back(std::stof(vec[i]));
        }  
        candidate_set.push_back(candidate);

        // uncomment the following lines to check the candidate
        // copy(candidate.begin(), candidate.end(), std::ostream_iterator<float>(std::cout, "|"));
        // std::cout << std::endl;

    }
    std::cout << "Input dimension is " << input_dimension << std::endl;
    std::cout << "Selecting variant..." << std::endl;

    auto query_time = 0;
    // select variant
    for (auto candidate : candidate_set) {
        // query
        metadata.dimension = input_dimension;
        metadata.schedule = candidate;
        
        auto start_eval = high_resolution_clock::now(); 
        // takes >25000 us
        float curr_runtime = KB[boost::edge(vertices[0], vertices[1], KB).first].performance_model->eval(metadata).exec_time;
        auto stop_eval = high_resolution_clock::now();
        auto duration_eval = duration_cast<microseconds>(stop_eval - start_eval); 
        // std::cout << "query time: " << duration_eval.count() << std::endl; 
        query_time += duration_eval.count();
        

        if (curr_runtime < min_runtime){
            selected_variant = candidate;
            min_runtime = curr_runtime;
        }
        
    }

    // std::cout << "total query time: " << query_time << std::endl;

    std::cout << "Variant selected!" << std::endl; 
    // std::cout << "Predicted runtime of the selected variant: ";
    // std::cout << min_runtime << std::endl;
    return selected_variant;
}

int main(int argc, char **argv)
{   

    std::cout << "Model saved to Knowledge Base" << std::endl;

    std::string env = argv[1]; 


    if (env == "Halide"){
        graph_t KB;

        // load graph
        std::string filename = "KB";
        std::vector<vertex_descriptor_t> vertices=load_graph(KB, filename);

        // std::cout << "Here" << std::endl;

        // query
        // struct metadata_t metadata;
        // metadata.dimension = 32768;
        // // metadata.schedule = {1024, 4, 2, 2};
        // metadata.schedule = {16, 64, 32, 16};
        // // metadata.schedule = {512, 8, 2, 2};

        // std::cout << "Predicted runtime of the given schedule: ";
        // std::cout << KB[boost::edge(vertices[0], vertices[1], KB).first].performance_model->eval(metadata).exec_time << std::endl;

        auto start = high_resolution_clock::now(); 
        // select variant
        std::vector<float> variant = select_variant(30, KB, vertices);

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start); 
        // std::cout << "variant-selection time: " << duration.count() << std::endl; 
        
        std::cout << "Selected variant: ";
        copy(variant.begin(), variant.end(), std::ostream_iterator<float>(std::cout, " "));
        std::cout << std::endl;
    }

    return 0;
}