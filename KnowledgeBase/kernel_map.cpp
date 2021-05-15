#include "kernel_map.h"
#include "global.h"

Kernel_map_t::Kernel_map_t()
{
}

bool Kernel_map_t::eval(metadata_t input)
{
	return false;
}

Always_on_t::Always_on_t()
{
}

bool Always_on_t::eval(metadata_t input)
{
	return true;
}

If_node_less_than_t::If_node_less_than_t()
{
}

If_node_less_than_t::If_node_less_than_t(int thres)
{
	this->thres = thres;
}

bool If_node_less_than_t::eval(metadata_t input)
{
	if (input.num_nodes < this->thres)
		return true;
	return false;
}

If_node_more_than_t::If_node_more_than_t()
{
}

If_node_more_than_t::If_node_more_than_t(int thres)
{
	this->thres = thres;
}

bool If_node_more_than_t::eval(metadata_t input)
{
	if (input.num_nodes >= this->thres)
		return true;
	return false;
}
