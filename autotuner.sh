#!/bin/bash
# run with: bash autotuner.sh

echo " "
echo "Reading configuration file..."
echo " "

# $(cat config.txt | sed -n '1p') is the first line of the config file
# e.g., entry="path", value="../Blur.cpp" 
IFS="=" read entry path <<< "$(cat config.txt | sed -n '1p')" 

# note that decisions are not limited to choosing from using cpu or gpu - any kind of decision can be made
IFS="=" read entry decision <<< "$(cat config.txt | sed -n '2p')" 

# Currently: default is starting from 2
IFS="=" read entry numeric <<< "$(cat config.txt | sed -n '3p')" 
IFS="=" read entry input_data <<< "$(cat config.txt | sed -n '4p')" 

IFS="=" read entry reuse_model <<< "$(cat config.txt | sed -n '5p')"
# if functional param is specified, read it; if not, ignore
IFS="=" read entry functional <<< "$(cat config.txt | sed -n '6p')"

# if the env is specified, read it; if not, ignore
# TODO: check if the env is one of the pre-defined environments in AT
IFS="=" read entry env <<< "$(cat config.txt | sed -n '7p')" 

# print the config info
echo "meta-program path=${path}"
echo "decision parameter=${decision}"
echo "numeric parameter=${numeric}"
echo "input_data parameter=${input_data}"
echo "using pre-trained models=${reuse_model}"
if ! [ -z "$functional" ] 
then
    echo "functional parameter=${functional}"
fi
if ! [ -z "$env" ] 
then
    echo echo "environment=${env}"
fi

# generate candidate set
# in the future: can detect the difference between [2,1024] and {2,1024}
g++ -g -Wall -std=c++11 candidate_set.cpp -o candidate_set
# default candidate set number = 1000
# default training set number = 250
./candidate_set 1250 $numeric

train_size=250
head -n $train_size candidate_set.csv > training_set.csv

# profiling process for unknown env by running the provided meta-program
# rm -f profiling_result.csv
# while IFS=, read -r array
# do
#     ts=$(date +%s%N)
#     ./$path ${array[@]} # TODO: add more inputs and check if that works
#     runtime=$((($(date +%s%N) - $ts)/1000000))
#     echo "Time taken: $runtime milliseconds"
#     echo "$runtime,$TODO_input,${array[@]},$TODO_cons" >> profiling_result.csv 
# done < training_set.csv

# TODO
# # Since candidate set may vary, the following allows reproducing results
# # allow user to switch candidate set also
# # echo -e "Candidate set for this version of the task exists"
# # if test -f "candidate_set.csv"; then
# #     echo -e "Would you like to regenerate the candidate set? (y/n)"
# #     read regen_choice
# #     if [[ $regen_choice == y ]]; then
# #         echo -e "Generating candidate set..."
# #         ./autotuner ../../$filename $env
# #         echo -e "Candidate set successfully generated"
# #     fi
# # fi

# g++ -g -Wall -std=c++11 autotuner.cpp candidate_set.cpp -o autotuner
# ./autotuner ../../$filename $env # autotuner.cpp should change name to env_cheking.cpp

# # For HalideSAR
# if test -f "${env}_profiling_result.csv"; then
#     echo -e "Performance model for this version of the task exists"
#     # echo -e "Would you like to reuse it? (y/n)"
#     # read reuse_choice
#     if [[ $reuse == y ]]; then
#         echo "Reusing the pre-trained model..."
#         train_size=7
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

# To store the model, hash the input file and name the folder that stores model.pb file
# so that each input file links to one model.pb
md5=($(md5sum ${path}))
mkdir model_config/${md5}

# TODO: copy to a folder named 'current' under the model_config file
# KB will automatically read from the 'current' file

# if reuse, 1. search for the md5 2. copy to the 'current' file
rm -rf model_config/current
cp -r model_config/${md5} model_config/current
# TODO: add current to the path in KB and verify it.

# python perf_model.py profiling_result.csv $train_size $md5
python perf_model.py ../halide_dataset/halide_fft_cpu_4000_points_I5.csv $train_size $md5

# # echo -e "Would you like to store the model? (y/n)"
# # read store_choice
# # if [[ $store_choice == y ]]; then
# #     echo "Model stored"
# #     # Connect to KB
# #     # TODO: either way, will overwrite?
# #     # can store in "${env}_profiling_result.csv" format
# # fi