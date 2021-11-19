#!/bin/bash

# run with: bash genmat.sh

shopt -s expand_aliases
source ~/.bashrc

join () {
  local IFS="$1"
  shift
  echo "$*"
}

echo " "
echo "Reading configuration file..."
echo " "

# $(cat config.txt | sed -n '1p') is the first line of the config file
# e.g., entry="path", value="../Blur.cpp" 
IFS="=" read entry path <<< "$(cat config.txt | sed -n '1p')" 
IFS="=" read entry compile_cmd <<< "$(cat config.txt | sed -n '2p')"
IFS="=" read entry run_cmd <<< "$(cat config.txt | sed -n '3p')"

# note that decisions are not limited to choosing from using cpu or gpu - any kind of decision can be made
IFS="=" read entry decision <<< "$(cat config.txt | sed -n '4p')" 

# Currently: default is starting from 2
IFS="=" read entry input_data <<< "$(cat config.txt | sed -n '5p')" 
IFS="=" read entry numeric <<< "$(cat config.txt | sed -n '6p')" 

IFS="=" read entry reuse_model <<< "$(cat config.txt | sed -n '7p')"
# if functional param is specified, read it; if not, ignore
IFS="=" read entry functional <<< "$(cat config.txt | sed -n '8p')"

# if the env is specified, read it; if not, ignore
# TODO: check if the env is one of the pre-defined environments in AT
IFS="=" read entry env <<< "$(cat config.txt | sed -n '9p')" 

# print the config info
echo "meta-program absolute path=${path}"
echo "compile command=${compile_cmd}"
echo "run command=${run_cmd}"
echo "decision parameter=${decision}" # TODO: cpu corresponds to profile on available cpus
echo "numeric parameter=${numeric}"
echo "input_data parameter=${input_data}"
echo "using pre-trained models=${reuse_model}"
if ! [ -z "$functional" ] 
then
    echo "functional parameter=${functional}"
fi
if ! [ -z "$env" ] 
then
    echo "environment=${env}"
fi

# TODO: maybe above should be genmat.sh
# genmat.sh should be able to call autotuner.sh several times
# below is the autotuner.sh

# find the current path 
autotuner_path=$(pwd)

# generate candidate set
# in the future: can detect the difference between [2,1024] and {2,1024}
clang++ -g -Wall -std=c++11 candidate_set.cpp -o candidate_set
# default candidate set number = 1000
# default training set number = 250
./candidate_set 1250 $numeric 

# default profiling size is 250
profiling_size=250
# default training size is 250
training_size=80 # 80 is used for testing purpose
# testing_size=succeeded profiling results - training_size

head -n $profiling_size candidate_set.csv > training_set.csv

# To create the condition for storing the performance prediction model, 
# hash the input file and name the folder that stores model.pb file
# so that each input file links to one model.pb
md5=($(md5sum ${path}))
mkdir -p model_config/${md5}

# hash the file and check if model_config has that file
# model_config serves as a lightweight knowledge base

# if the file was not seen before or the user choose not to reuse models
if [ ! -d model_config/${md5} -o ! $reuse_model == y ]; then

    # start profiling for unknown env by running the provided meta-program
    rm -f profiling_result.csv

    # Profiling
    # trim the '/meta-program' part at the end
    dir_path=${path%/*}
    cd $dir_path
    echo "Currently at: $(pwd)"

    # compile once
    # feed in multiple parameter sets using the command line arguments
    $compile_cmd

    echo "Profiling..."

    while IFS=, read -r array
    do

        # randomly select input dimension given constraints ('input-data') from config.txt
        # given: 1024, 1024, 1024
        # output example: 2, 128, 256

        # split the 'input-data' string
        IFS=', ' read -r -a input_range <<< "$input_data"
        # iteratively go through all the element/range_limit
        for index in "${!input_range[@]}"
        do
            # echo "$index ${input_range[index]}"

            # default range interval starts from 2 and increases exponentially
            range_log=$(bc -l <<< "l(${input_range[index]})/l(2)") # replace '2' if needed
            range_log=${range_log%.*}
            range_log=$(( $RANDOM % $range_log + 1 )) # 1 - range_log

            input_dim[index]=$(bc <<< "2^${range_log}")
        done
        # input_range is not changed in this process

        array_unjoined=$(tr ',' '\n' <<< ${array[@]})

        # 'date' for Linux; 'gdate' for MacOS
        start=`gdate +%s.%N`

        # TODO: write sample meta-programs 
        #   1. MMM (tuning between Eigen and Boost) 
        #   2. Halide Blur 
        #   3. meta-program template

        # Current mode is for users without zero understanding of the program.
        # Therefore, the user does not need to know the constraints for each tuning knob-some execution will fail because of this

        # To accelerate the profiling process, more specifically, to increase the execution success rate, some DSL knowledge is required from the user

        # pay extra caution to this line as 'eval $cmd' might be malicious
        if error_msg=$(eval $run_cmd ${input_dim[@]} $array_unjoined 2>&1) ; then 
            echo "Command succeeded"

            end=`gdate +%s.%N`
            runtime=$( echo "$end - $start" | bc -l ) # in second 

            input_dim_joined=$(join , "${input_dim[@]}") # joined refers to using comma to 'join' each element
            # echo "input=$input_dim_joined"
            # echo "array=${array[@]}"

            # TODO: add constant (a mathmatical function of elements in input_dim)
            echo "$runtime,$input_dim_joined,${array[@]}" >> $autotuner_path/profiling_result.csv 

        else
            echo "Command failed"
            # echo $error_msg
        fi

    done < $autotuner_path/training_set.csv

    # --------------- Existing environments -----------------

    # g++ -g -Wall -std=c++11 autotuner.cpp candidate_set.cpp -o autotuner
    # ./autotuner ../../$filename $env # autotuner.cpp should change name to env_cheking.cpp

    # # For HalideSAR
    # if test -f "${env}_profiling_result.csv"; then
    #     echo -e "Performance model for this version of the task exists"
    #     # echo -e "Would you like to reuse it? (y/n)"
    #     # read reuse_choice
    #     if [[ $reuse == y ]]; then
    #         echo "Reusing the pre-trained model..."
    #         profiling_size=7
    #         # Connect to KB
    #     else 
    #         # g++ -g -Wall -std=c++11 replace.cpp -o replace
    #         # # Not reusing
    #         # # delete profiling_result.csv
    #         # rm ${env}_profiling_result.csv
    #         # echo -e "Profiling..."
    #         # while IFS=, read -r p1 p2
    #         # do
    #         #     # generate CMakeLists.txt
    #         #     ./replace ../../CMakeLists.txt HalideSAR $p1 $p2
    #         #     cd .. # build folder
    #         #     make
    #         #     ts=$(date +%s%N)
    #         #     # ./sarbp -p ../data/Sandia/npy -o Sandia.png -d -45.0 -D 0.0 -t 30 -u 2
    #         #     ./sarbp -p ../data/AFRL/pass1/HH_npy -o AFRL.png -d -30.0 -D 0.0 -t 17 -u 2
    #         #     tt=$((($(date +%s%N) - $ts)/1000000))
    #         #     echo "Time taken: $tt milliseconds"
    #         #     # 30, 900 are tentative values
    #         #     echo "$tt,4096,$p1,$p2,16777216" >> autotuner/${env}_profiling_result.csv
    #         #     cd autotuner/
    #         # done < training_set.csv

    #         echo -e "Training performance prediction model..."
    #     fi
    # fi

    # Sometimes need to shuffle and sometimes not
    # python3 shuffle.py ${env}_profiling_result.csv 

    # ---------------------------------------------------------

    cd $autotuner_path
    echo "Currently at: $(pwd)"

    echo "md5: $md5"

    python3 perf_model.py profiling_result.csv $training_size $md5
    # python3 perf_model.py ../halide_dataset/halide_fft_cpu_4000_points_I5.csv $training_size $md5
    # python3 perf_model.py profiling_result.csv 80 0b8ffcf2d31912a1ac9a3f3d4f74debd
fi

# copy to a folder named 'current' under the 'model_config' file
# KB will automatically read from the 'current' file
rm -rf model_config/current
cp -r model_config/${md5} model_config/current

# load stored model and inference
# read from model_config/$md5 and inference on candidate_set.csv
# TODO: currently only works for single input_data
for (( t = 2; t <= input_data; t = t * 2 )) 
do  
    python3 inference.py candidate_set.csv $t $md5 
    # python3 inference.py candidate_set.csv 1024 0b8ffcf2d31912a1ac9a3f3d4f74debd
done

# TODO: choose between cpu and gpu
# always run platform-select.py (change it to decision.py)
# multiple config.txt

# Example of integrating with external KB
# # go to KB 
# # KB need to select variant using multiple input data
# cd knowledge_base/build/
# # suppress tensorflow warnings
# export TF_CPP_MIN_LOG_LEVEL=2
# # KB procedures
# # 'make' after modifying KB
# ./save
# ./load
