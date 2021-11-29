#!/bin/bash

# run with: bash genmat.sh

# shopt -s expand_aliases
# source ~/.bashrc

# a join function that is used later to parse inputs
join () {
  local IFS="$1"
  shift
  echo "$*"
}

echo " "
echo "Reading configuration file..."
echo " "

# count the number of lines in config.txt
lines=$(grep -c ".*" config.txt)

# calculate the number of Execution Blocks
blocks=$((($lines - 1) / 7))

# read Decisions
IFS="=" read entry decisions <<< "$(cat config.txt | sed -n '1p')" 
echo "decisions=${decisions}" 

# split the 'decision' string and store as an array named 'decision'
IFS=', ' read -r -a decision <<< "$decisions"

# declare variables and lists
declare number_of_input_dim
declare -a md5_list
declare -a functional_list

# for each Execution Block
for (( block = 0; block < $blocks; block++ ))
do

echo " "
echo "Block $block. Decision: ${decision[$block]}"
echo " "

# read config.txt
# $(cat config.txt | sed -n '1p') is the first line of the config file
# e.g., entry="path", value="../Blur.cpp" 

IFS="=" read entry input_metadata <<< "$(cat config.txt | sed -n "$(( $block*7 + 2 )) p")" 
IFS="=" read entry numeric <<< "$(cat config.txt | sed -n "$(( $block*7 + 3 )) p")" 
# if functional param is specified, read it; if not, ignore
IFS="=" read entry functional <<< "$(cat config.txt | sed -n "$(( $block*7 + 4 )) p")"

IFS="=" read entry path <<< "$(cat config.txt | sed -n "$(( $block*7 + 5 )) p")" 
IFS="=" read entry compile_cmd <<< "$(cat config.txt | sed -n "$(( $block*7 + 6 )) p")"
IFS="=" read entry run_cmd <<< "$(cat config.txt | sed -n "$(( $block*7 + 7 )) p")"

IFS="=" read entry reuse_model <<< "$(cat config.txt | sed -n "$(( $block*7 + 8 )) p")"

# FUTURE: fully automated pre-existing env
# # if the env is specified, read it; if not, ignore
# # check if the env is one of the pre-defined environments in GenMAT
# IFS="=" read entry env <<< "$(cat config.txt | sed -n '9p')" 

# print the config info
echo "input_metadata parameter=${input_metadata}"
echo "numeric parameter=${numeric}"
echo "using pre-trained models=${reuse_model}"

echo "meta-program path=${path}" # both absolute and relative path works
echo "compile command=${compile_cmd}"
echo "run command=${run_cmd}"

# if the functional parameter is specified
isFunctional=true

# if isFunctional, use the specified formula to calculate the additional constant("C" in NN+C)
# if not, use 1 (which will be normalized by NN+C)

if ! [ -z "$functional" ] 
then
    echo "functional parameter=${functional}"
    # e.g., transform 1*2 to 'let functional_param=${all_params[0]}*${all_params[1]}'
    functional_calculation=$(python3 functional.py $functional)
    functional_list[$block]=$functional
else 
    isFunctional=false
    functional_list[$block]=1
fi

if ! [ -z "$env" ] 
then
    echo "environment=${env}"
fi

echo " "

# find the current path 
autotuner_path=$(pwd)

# generate candidate set
# FUTURE: detect the difference between [2,1024] and {2,1024}
clang++ -g -Wall -std=c++11 candidate_set.cpp -o candidate_set
# default candidate set number = 1000
# default training set number = 250
./candidate_set 1250 $numeric 

# default profiling size is 250
profiling_size=250
# default training size is 100
training_size=100

# training_size should be (2x) less than profiling_size to see MAPE and rho
# testing_size=succeeded profiling results - training_size
# training_set.csv is used for profiling and training/testing the model

head -n $profiling_size candidate_set.csv > training_set.csv

# to create the condition for storing the performance prediction model, 
# hash the input file and name the folder under model_config that stores model.pb file using the hash value
# so that each input file links to one model.pb
# model_config serves as a lightweight knowledge base
md5=($(md5sum ${path}))
mkdir -p model_config/${md5}

# add to the list
md5_list[$block]=$md5

# split the 'input-data' string
IFS=', ' read -r -a number_of_input_dim <<< "$input_metadata"

# if the file was not seen before or the user choose not to reuse models
if [ ! -d model_config/${md5} -o ! $reuse_model == y ]; then

    # start profiling for unknown env by running the provided meta-program
    rm -f profiling_result.csv

    # trim the '/meta-program' part at the end
    dir_path=${path%/*}
    cd $dir_path
    echo "Currently at: $(pwd)"

    # compile once
    # feed in multiple parameter sets using the command line arguments
    $compile_cmd

    # Profiling

    echo "Profiling"

    while IFS=, read -r array
    do

        # randomly select input dimension given constraints ('input-metadata') from config.txt
        # given: 1024, 1024, 1024
        # output example: 2, 128, 256

        # split the 'input-data' string
        IFS=', ' read -r -a input_range <<< "$input_metadata"
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

        IFS=', ' read -r -a numerics <<< "${array[@]}"

        # echo "input=${input_dim[@]}"
        # echo "array=${numerics[@]}"

        all_params=("${input_dim[@]}" "${numerics[@]}")

        # echo ${all_params[@]}
        # echo $isFunctional

        if [ "$isFunctional" = true ] ; then
            # example: 
            # functional_calculation='let functional_param=${all_params[0]}*${all_params[1]}'

            # pay extra caution to this line as 'eval $cmd' might be malicious
            eval $functional_calculation
        else 
            # append 1 when the user does not specify functional parameters
            functional_param=1
        fi

        # echo $functional_param

        # 'date' for Linux; 'gdate' for MacOS
        start=`gdate +%s.%N`

        # Current mode is for users without zero understanding of the program.
        # Therefore, the user does not need to know the constraints for each tuning knob-some execution will fail because of this

        # To accelerate the profiling process, more specifically, to increase the execution success rate, some DSL knowledge is required from the user

        # pay extra caution to this line as 'eval $cmd' might be malicious
        if error_msg=$(eval $run_cmd ${input_dim[@]} $array_unjoined 2>&1) ; then 
            # echo "Command succeeded"
            echo -n "."
            
            # 'date' for Linux; 'gdate' for MacOS
            end=`gdate +%s.%N`

            runtime=$( echo "$end - $start" | bc -l ) # in second 

            input_dim_joined=$(join , "${input_dim[@]}") # joined refers to using comma to 'join' each element
            # echo "input=$input_dim_joined"
            # echo "array=${array[@]}"

            echo "$runtime,$input_dim_joined,${array[@]},$functional_param" >> $autotuner_path/profiling_result.csv 

        # else
            # echo "Command failed"
            # echo $error_msg
        fi

    done < $autotuner_path/training_set.csv

    cd $autotuner_path

    echo " "
    echo "Currently at: $(pwd)"

    echo "md5: $md5"

    # Performance Prediction

    python3 perf_model.py profiling_result.csv $training_size $md5 ${decision[$block]}

fi

done

# echo ${functional_list[@]}

echo "--------------Query Interface--------------"
echo "type 'quit' or use CTRL-C to exit"
echo " "

# echo ${#number_of_input_dim[@]}

input_size=0
while [ "$input_size" != quit ] 
do 
    echo "type the input size you would like to query:"
    IFS= read -r input_size

    if [ "$input_size" == quit ]; then continue
    fi

    IFS=', ' read -r -a inputs <<< "$input_size"
    
    if [ ${#inputs[@]} != ${#number_of_input_dim[@]} ]; then
        echo "number of input dimensions should be ${#number_of_input_dim[@]}"
        continue
    fi
    
    # need a new csv file to store the best variant for each decision
    rm -f best_candidates.csv

    # for each queried input
    # iterate through all md5-decision combinations and use the pre-trained model there to inference
    for (( block = 0; block < $blocks; block++ ))
    do  
        # load stored model and inference
        # read from model_config/$md5/$decision and inference on candidate_set.csv
        python3 inference.py candidate_set.csv ${md5_list[$block]} ${decision[$block]} $input_size ${functional_list[$block]}
    done

    # choose between decisions (e.g., cpu and gpu) - decision.py
    python3 decision.py $input_size
done 
