#include "hardware.h"
#include "global.h"

CPU_t::CPU_t()
{
	this->total_cores = 0;
	this->total_FLOPS = 0;
}

CPU_t::CPU_t(int total_cores, double FLOPS)
{
	this->total_cores = total_cores;
	this->total_FLOPS = FLOPS;
}

double CPU_t::get_FLOPS()
{
	return this->total_FLOPS;
}

GPU_t::GPU_t()
{
	this->input_bus_bandwith = 0;
	this->total_FLOPS = 0;
}

GPU_t::GPU_t(double total_FLOPS, double input_bus_bandwidth)
{
	this->input_bus_bandwith = input_bus_bandwidth;
	this->total_FLOPS = total_FLOPS;
}

double GPU_t::get_FLOPS()
{
	return this->total_FLOPS;
}

Hardware_t::Hardware_t()
{
}

double Hardware_t::get_FLOPS()
{
	return 0.0;
}
