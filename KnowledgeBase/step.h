#pragma once
#include "global.h"

class Step_t 
{
public:
	std::string type;
	int id;
	Step_t();
	virtual void print(std::ostream &os) {};
};

class DBScan_t : public Step_t
{
public:
	DBScan_t();
	void print(std::ostream &os)
	{
		os << "Step_t " << this->type << " " << this->id << " " << std::endl;
	}
};

class Node_classification_sample_t : public Step_t
{
public:
	Node_classification_sample_t();
	void print(std::ostream &os)
	{
		os << "Step_t " << this->type << " " << this->id << " " << std::endl;
	}
};

class Blur_t: public Step_t
{
public:
	Blur_t();
	void print(std::ostream &os)
	{
		os << "Step_t" << " " << this->type << " " << this->id << " " << std::endl;
	}
};