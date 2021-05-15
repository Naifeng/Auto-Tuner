#pragma once
#include <vector>
#include "global.h"

class Performance_model_t
{
public:
	std::string type;
	int id;
	int src_id;
	int dst_id;
	Performance_model_t();
	virtual struct performance_t eval(struct metadata_t input);
	virtual void print(std::ostream &os) {};
};

class Linear_1d_t: public Performance_model_t
{
private:
	double k;
	double b;
public:
	Linear_1d_t();
	Linear_1d_t(double k,double b);
	void set(double k, double b);
	struct performance_t eval(struct metadata_t input);
	void print(std::ostream &os)
	{
		os << "Performance_model_t " << this->type << " " << this->id << " " << this->k << " " << this->b << std::endl;
	}
};

class MLP_t: public Performance_model_t
{
	char *pb_path;
	char *chkpt_path;
public:
	MLP_t();
	MLP_t(char *pb_file, char *chkpt_file);
	struct performance_t eval(struct metadata_t input);
	void print(std::ostream &os)
	{
		os << "Performance_model_t " << this->type << " " << this->src_id << " " << this->dst_id << " ";
		os << this->id << " " << this->pb_path << " " << this->chkpt_path << std::endl;
	}
};