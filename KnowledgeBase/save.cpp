#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/config.hpp>
#include <boost/graph/graphml.hpp>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <fstream>
#include "global.h"
#include "hardware.h"
#include "kernel.h"
#include "model.h"
#include "step.h"
#include "kernel_map.h"

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

void save_graph(graph_t &g, std::string filename)
{
	boost::dynamic_properties dp;
	dp.property("vectex_properties_is_step", get(&vertex_properties_t::is_step , g));
	dp.property("vectex_properties_is_kernel", get(&vertex_properties_t::is_kernel, g));
	dp.property("vectex_properties_is_hardware", get(&vertex_properties_t::is_hardware, g));
	dp.property("vectex_properties_step", get(&vertex_properties_t::id, g));
	dp.property("edge_properties", get(&edge_properties_t::is_kernel_map, g));
	dp.property("edge_properties", get(&edge_properties_t::is_performance_model, g));
	dp.property("edge_properties", get(&edge_properties_t::id, g));
	std::ofstream out(filename+"_sturcture.xml", std::ios::out);
	write_graphml(out, g, dp, true);
	out.close();
	out.open(filename + "_content.xml", std::ios::out);
	std::pair<vertex_iter, vertex_iter> vp;
	for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first)
		if (g[*vp.first].is_hardware)
			g[*vp.first].hardware->print(out);
		else if (g[*vp.first].is_kernel)
			g[*vp.first].kernel->print(out);
		else if (g[*vp.first].is_step)
			g[*vp.first].step->print(out);
	std::pair<edge_iter, edge_iter> ep;
	edge_iter ei, ei_end;
	for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
		if (g[*ei].is_kernel_map)
			g[*ei].kernel_map->print(out);
		else if (g[*ei].is_performance_model)
			g[*ei].performance_model->print(out);
	out.close();
	return;
}

int main()
{
	graph_t KB;
    
    // add hardware
    vertex_descriptor_t hardware0 = boost::add_vertex(KB);
	CPU_t i7_cpu(4, 50000000000);
    i7_cpu.type = "CPU_t";
    i7_cpu.id = 0;
	KB[hardware0].is_hardware = true;
	KB[hardware0].hardware = &i7_cpu;
	KB[hardware0].id = i7_cpu.id;

    // add step
    vertex_descriptor_t step0 = boost::add_vertex(KB);
    Blur_t Blur;
    Blur.type = "Blur_t";
    Blur.id = 1;
    KB[step0].is_step = true;
	KB[step0].step = &Blur;
	KB[step0].id = Blur.id;

    // add performance model
    std::pair<edge_descriptror_t, bool> h0s0 = boost::add_edge(hardware0, step0, KB);
    MLP_t halide_i7("../tf_models/model.pb","../tf_models/my-model.ckpt");
    halide_i7.type = "MLP_t";
    halide_i7.id = 2;
	halide_i7.src_id = KB[hardware0].id;
	halide_i7.dst_id = KB[step0].id;
	KB[h0s0.first].is_performance_model = true;
	KB[h0s0.first].performance_model = &halide_i7;

    // query
    // float dimension = 32768;
    // std::vector<float> schedule = {1024, 4, 2, 2};
    // std::cout << halide_i7.eval(dimension, schedule).exec_time << std::endl;

    // save graph
    std::string filename = "KB";
	save_graph(KB, filename);

    return 0;
}