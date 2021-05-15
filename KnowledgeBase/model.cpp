#include <numeric>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "model.h"
#include "global.h"
#include "include/Model.h" 
#include "include/Tensor.h" 
// added to read .csv files
#include <boost/tokenizer.hpp>

Linear_1d_t::Linear_1d_t()
{
}

Linear_1d_t::Linear_1d_t(double k, double b)
{
	this->k = k;
	this->b = b;
}

void Linear_1d_t::set(double k, double b)
{
	this->k = k;
	this->b = b;
}

performance_t Linear_1d_t::eval(metadata_t input)
{
	struct performance_t ans;
	ans.exec_time = input.OPS*this->k + this->b;
	ans.latency = this->b;
	return ans;
}

Performance_model_t::Performance_model_t()
{
}

performance_t Performance_model_t::eval(metadata_t input)
{
	return performance_t();
}

MLP_t::MLP_t()
{
}

MLP_t::MLP_t(char *pb_file, char *chkpt_file)
{
    pb_path=strdup(pb_file);
    chkpt_path=strdup(chkpt_file);
}

performance_t MLP_t::eval(struct metadata_t in)
{
    float dimension = in.dimension;
    std::vector<float> schedule = in.schedule;
    // append the dimension to the data vector
    std::vector<float> data;
    data.push_back(dimension);
    // copy the schedule vector into the data vector
    data.insert(data.end(), schedule.begin(), schedule.end());
    // append the constant to the data vector
    float cons = dimension*dimension;
    data.push_back(cons);

    // PATH to the saved graph from the pretrained model
    Model model(pb_path); 

    // PATH to the saved check point from thee pretrained model
    model.restore(chkpt_path); 

    Tensor input{model, "input"};
    Tensor output{model, "output"};

    // uncomment for validation purpose
    // std::vector<float> data = {16384, 1024, 2, 2, 2, 268435456};
    // std::vector<float> data = {32768, 16, 64, 32, 16, 1073741824};
    // std::vector<float> data = {32768, 1024, 4, 2, 2, 1073741824};

    // scale input
    // from halide_I7, hard-coded
    //std::vector<float> mean = {9.9123200e+03, 2.2196000e+02, 1.8346400e+02, 4.2440000e+01, 1.3720000e+01, 2.0013541e+08};
    //std::vector<float> var = {1.00936278e+04, 3.31257408e+02, 2.89093785e+02, 1.30071051e+02, 4.16538306e+01, 3.42590390e+08};
    

    // from *_hpc.csv, hard-coded
    // std::vector<float> mean = {1.06414080e+04, 1.99136000e+02, 1.98936000e+02, 4.52720000e+01, 9.70400000e+00, 2.34562257e+08};
    // std::vector<float> var = {1.10146581e+04, 3.21347397e+02, 3.18602756e+02, 1.44071864e+02, 2.28761969e+01, 3.82664360e+08};

    // read .csv files
    // hard-coded filename
    std::string parameters("../../preprocessing_parameters.csv");
    std::ifstream infile(parameters.c_str());
    if (!infile.is_open()) return {0};
    
    typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;
    std::vector<std::string > vec;
    std::string line;

    std::vector<float> mean;
    std::vector<float> var;

    int index = 0;
    while (getline(infile,line))
    {   
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        
        if (index == 0){
            for(int i = 0; i != vec.size(); ++i) {
                if(!vec[i].empty()) mean.push_back(std::stof(vec[i]));
            }  
        }
        if (index == 1){
            for(int i = 0; i != vec.size(); ++i) {
                if(!vec[i].empty()) var.push_back(std::stof(vec[i]));
            }  
        }

        index++;
        // uncomment the following lines to check the candidate
        // copy(mean.begin(), mean.end(), std::ostream_iterator<float>(std::cout, "|"));
        // std::cout << std::endl;
    }

    // scaling
    for(int i = 0; i != data.size(); ++i) {
        data[i] = (data[i] - mean[i])/var[i];
    }   
    
    // uncomment to show the scaled input 
    // for (float f : data) {
    //     std::cout << f << " ";
    // }
    // std::cout << std::endl;

    input.set_data(data);
    
    // inference
    model.run({&input}, output);

    struct performance_t ans;
	ans.exec_time = output.get_data<float>()[0];

	return ans;
}