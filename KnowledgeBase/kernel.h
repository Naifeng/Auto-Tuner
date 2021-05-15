#pragma once
#include "model.h"
#include "hardware.h"
#include "global.h"

class Kernel_t
{
public:
	std::string type;
	int id;
	Kernel_t();
	virtual void print(std::ostream &os) {};
};

class GEMM_t : public Kernel_t
{
public:
	GEMM_t();
	void print(std::ostream &os)
	{
		os << "Kernel_t " << this->type << " " << this->id << " " << std::endl;
	}
};

class Matrix_pointwise_t : public Kernel_t
{
public:
	Matrix_pointwise_t();
	void print(std::ostream &os)
	{
		os << "Kernel_t " << this->type << " " << this->id << " " << std::endl;
	}
};

class BFS_tree_t : public Kernel_t
{
public:
	BFS_tree_t();
	void print(std::ostream &os)
	{
		os << "Kernel_t " << this->type << " " << this->id << " " << std::endl;
	}
};

class KD_tree_t : public Kernel_t
{
public:
	KD_tree_t();
	void print(std::ostream &os)
	{
		os << "Kernel_t " << this->type << " " << this->id << " " << std::endl;
	}
};

class Ball_tree_t : public Kernel_t
{
public:
	Ball_tree_t();
	void print(std::ostream &os)
	{
		os << "Kernel_t " << this->type << " " << this->id << " " << std::endl;
	}
};