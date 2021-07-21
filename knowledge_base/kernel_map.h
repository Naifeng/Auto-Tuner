#pragma once
#include "global.h"

class Kernel_map_t
{
public:
	std::string type;
	int id;
	Kernel_map_t();
	virtual bool eval(struct metadata_t input);
	virtual void print(std::ostream &os) {};
};

class Always_on_t : public Kernel_map_t
{
public:
	Always_on_t();
	bool eval(struct metadata_t input);
	void print(std::ostream &os)
	{
		os << "Kernel_map_t " << this->type << " " << this->id << " " << std::endl;
	}
};

class If_node_less_than_t : public Kernel_map_t
{
private:
	int thres;
public:
	If_node_less_than_t();
	If_node_less_than_t(int thres);
	bool eval(struct metadata_t input);
	void print(std::ostream &os)
	{
		os << "Kernel_map_t " << this->type << " " << this->id << " " << this->thres << std::endl;
	}
};

class If_node_more_than_t : public Kernel_map_t
{
private:
	int thres;
public:
	If_node_more_than_t();
	If_node_more_than_t(int thres);
	bool eval(struct metadata_t input);
	void print(std::ostream &os)
	{
		os << "Kernel_map_t " << this->type << " " << this->id << " " << this->thres << std::endl;
	}
};