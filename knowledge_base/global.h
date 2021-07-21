#pragma once
#include <string>
#include <iostream>
#include "hardware.h"
#include "kernel.h"
#include "model.h"

enum datatype_t {int32, int64, float32, float64};

struct performance_t
{
	double exec_time;
	double latency;
};

struct metadata_t
{
	double OPS;
	int num_nodes;
	datatype_t datatype;
	float dimension;
	std::vector<float> schedule;
};