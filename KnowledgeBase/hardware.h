#pragma once
#include "global.h"

class Hardware_t
{
public:
	std::string type;
	int id;
	Hardware_t();
	virtual double get_FLOPS();
	virtual void print(std::ostream &os) {};
};

class CPU_t: public Hardware_t
{
private:
	int total_cores;
	double total_FLOPS;
public:
	CPU_t();
	CPU_t(int total_cores, double FLOPS);
	double get_FLOPS();
	void print(std::ostream &os)
	{
		os << "Hardware_t " << this->type << " " << this->id << " " << this->total_cores << " " << this->total_FLOPS << std::endl;
	}
};

class GPU_t: public Hardware_t
{
private:
	double total_FLOPS;
	double input_bus_bandwith;
public:
	GPU_t();
	GPU_t(double total_FLOPS, double input_bus_bandwidth);
	double get_FLOPS();
	void print(std::ostream &os)
	{
		os << "Hardware_t " << this->type << " " << this->id << " " << this->total_FLOPS << " " << this->input_bus_bandwith << std::endl;
	}
};