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

Kernel_t dummy_kernel;
Hardware_t dummy_hardware;
Step_t dummy_step;
Performance_model_t dummy_performance_model;
Kernel_map_t dummy_kernel_map;

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

	// add hardwares
	vertex_descriptor_t hardware0 = boost::add_vertex(KB);
	vertex_descriptor_t hardware1 = boost::add_vertex(KB);
	CPU_t xeon_cpu(18, 500000000000); xeon_cpu.type = "CPU_t"; xeon_cpu.id = 0;
	GPU_t titan_gpu(8000000000000, 10000000000); titan_gpu.type = "GPU_t"; titan_gpu.id = 1;
	KB[hardware0].is_hardware = true;
	KB[hardware0].hardware = &xeon_cpu;
	KB[hardware0].id = xeon_cpu.id;
	KB[hardware1].is_hardware = true;
	KB[hardware1].hardware = &titan_gpu;
	KB[hardware1].id = titan_gpu.id;

	// add kernels
	vertex_descriptor_t kernel0 = boost::add_vertex(KB);
	vertex_descriptor_t kernel1 = boost::add_vertex(KB);
	vertex_descriptor_t kernel2 = boost::add_vertex(KB);
	vertex_descriptor_t kernel3 = boost::add_vertex(KB);
	vertex_descriptor_t kernel4 = boost::add_vertex(KB);
	GEMM_t gemm_kernel; gemm_kernel.type = "GEMM_t"; gemm_kernel.id = 2;
	Matrix_pointwise_t matrix_pointwise_kernel; matrix_pointwise_kernel.type = "Matrix_pointwise_t"; matrix_pointwise_kernel.id = 3;
	BFS_tree_t BFS_tree_kernel; BFS_tree_kernel.type = "BFS_tree_t"; BFS_tree_kernel.id = 4;
	KD_tree_t KD_tree_kernel; KD_tree_kernel.type = "KD_tree_t"; KD_tree_kernel.id = 5;
	Ball_tree_t Ball_tree_kernel; Ball_tree_kernel.type = "Ball_tree_t"; Ball_tree_kernel.id = 6;
	KB[kernel0].is_kernel = true;
	KB[kernel0].kernel = &gemm_kernel;
	KB[kernel0].id = gemm_kernel.id;
	KB[kernel1].is_kernel = true;
	KB[kernel1].kernel = &matrix_pointwise_kernel;
	KB[kernel1].id = matrix_pointwise_kernel.id;
	KB[kernel2].is_kernel = true;
	KB[kernel2].kernel = &BFS_tree_kernel;
	KB[kernel2].id = BFS_tree_kernel.id;
	KB[kernel3].is_kernel = true;
	KB[kernel3].kernel = &KD_tree_kernel;
	KB[kernel3].id = KD_tree_kernel.id;
	KB[kernel4].is_kernel = true;
	KB[kernel4].kernel = &Ball_tree_kernel;
	KB[kernel4].id = Ball_tree_kernel.id;

	// link kernels and hardwares with performance_models
	std::pair<edge_descriptror_t, bool> ek0h0 = boost::add_edge(hardware0, kernel0, KB);
	std::pair<edge_descriptror_t, bool> ek0h1 = boost::add_edge(hardware1, kernel0, KB);
	Linear_1d_t performance_k0h0(1 / xeon_cpu.get_FLOPS(), 0); performance_k0h0.type = "Linear_1d_t"; performance_k0h0.id = 7;
	Linear_1d_t performance_k0h1(1 / titan_gpu.get_FLOPS(), 0); performance_k0h1.type = "Linear_1d_t"; performance_k0h0.id = 8;
	KB[ek0h0.first].is_performance_model = true;
	KB[ek0h0.first].performance_model = &performance_k0h0;
	KB[ek0h0.first].id = performance_k0h0.id;
	KB[ek0h1.first].is_performance_model = true;
	KB[ek0h1.first].performance_model = &performance_k0h1;
	KB[ek0h1.first].id = performance_k0h1.id;

	std::pair<edge_descriptror_t, bool> ek1h0 = boost::add_edge(hardware0, kernel1, KB);
	std::pair<edge_descriptror_t, bool> ek1h1 = boost::add_edge(hardware1, kernel1, KB);
	Linear_1d_t performance_k1h0(1 / xeon_cpu.get_FLOPS(), 0); performance_k1h0.type = "Linear_1d_t"; performance_k1h0.id = 9;
	Linear_1d_t performance_k1h1(1 / titan_gpu.get_FLOPS(), 0); performance_k1h1.type = "Linear_1d_t"; performance_k1h1.id = 10;
	KB[ek1h0.first].is_performance_model = true;
	KB[ek1h0.first].performance_model = &performance_k1h0;
	KB[ek1h0.first].id = performance_k1h0.id;
	KB[ek1h1.first].is_performance_model = true;
	KB[ek1h1.first].performance_model = &performance_k1h1;
	KB[ek1h1.first].id = performance_k1h1.id;

	// add steps
	vertex_descriptor_t step0 = boost::add_vertex(KB);
	vertex_descriptor_t step1 = boost::add_vertex(KB);
	DBScan_t DBScan; DBScan.type = "DBScan_t"; DBScan.id = 11;
	Node_classification_sample_t Node_classification_sample; Node_classification_sample.type = "Node_classification_sample_t"; Node_classification_sample.id = 12;
	KB[step0].is_step = true;
	KB[step0].step = &DBScan;
	KB[step0].id = DBScan.id;
	KB[step1].is_step = true;
	KB[step1].step = &Node_classification_sample;
	KB[step1].id = Node_classification_sample.id;

	// link steps with kernels
	std::pair<edge_descriptror_t, bool> es1k2 = boost::add_edge(step1, kernel2, KB);
	std::pair<edge_descriptror_t, bool> es0k3 = boost::add_edge(step0, kernel3, KB);
	std::pair<edge_descriptror_t, bool> es0k4 = boost::add_edge(step0, kernel4, KB);
	Always_on_t Always_on; Always_on.type = "Always_on_t"; Always_on.id = 13;
	If_node_less_than_t If_node_less_than(1000); If_node_less_than.type = "If_node_less_than_t"; If_node_less_than.id = 14;
	If_node_more_than_t If_node_more_than(1000); If_node_more_than.type = "If_node_more_than_t"; If_node_more_than.id = 15;
	KB[es1k2.first].is_kernel_map = true;
	KB[es1k2.first].kernel_map = &Always_on;
	KB[es1k2.first].id = Always_on.id;
	KB[es0k3.first].is_kernel_map = true;
	KB[es0k3.first].kernel_map = &If_node_less_than;
	KB[es0k3.first].id = If_node_less_than.id;
	KB[es0k4.first].is_kernel_map = true;
	KB[es0k4.first].kernel_map = &If_node_more_than;
	KB[es0k4.first].id = If_node_more_than.id;

	std::string filename = "KB";
	save_graph(KB, filename);

	// sample query
	struct metadata_t metadata;
	metadata.OPS = 2 * 5000 * 5000 * 5000;
	metadata.datatype = int32;
	struct performance_t performance;
	performance = KB[ek0h0.first].performance_model->eval(metadata);
	std::cout << performance.exec_time << std::endl;

}