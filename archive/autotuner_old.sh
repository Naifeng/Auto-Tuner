#!/bin/bash
# run with: bash autotuner.sh

g++ -g -Wall -std=c++11 autotuner.cpp candidate_set.cpp -o autotuner
echo -e "Please enter the filename:"
read filename # e.g., CMakeLists.txt
echo -e "Please enter the environment:"
read env # e.g., Halide-General
# echo -e "Please enter the image input size:"
# read img_dim # e.g., 1024
echo -e "Please enter the target hardware:"
read hardware # e.g., cpu, gpu
echo -e "Please enter the input size(s):"
read sizes # e.g., 1024, 2048, 4096, 8192, 16384, 32768

# env='HalideSAR'
# TODO: concat md5 string

# Since candidate set may vary, the following allows reproducing results
# allow user to switch candidate set also
# echo -e "Candidate set for this version of the task exists"
# if test -f "candidate_set.csv"; then
#     echo -e "Would you like to regenerate the candidate set? (y/n)"
#     read regen_choice
#     if [[ $regen_choice == y ]]; then
#         echo -e "Generating candidate set..."
#         ./autotuner ../../$filename $env
#         echo -e "Candidate set successfully generated"
#     fi
# fi

# ./autotuner ../../$filename $env

# train_size=100
# # set profile number
# # echo -e "Would you like to regenerate the candidate set? (y/n)"
# head -n $train_size candidate_set.csv > training_set.csv

# # Check if previous profiling results already exists
# if test -f "${env}_profiling_result.csv"; then
#     echo -e "Performance model for this version of the task exists"
#     echo -e "Would you like to reuse it? (y/n)"
#     read reuse_choice
#     if [[ $reuse_choice == y ]]; then
#         echo "Reusing the pre-trained model"
#         train_size=10
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


# python3 shuffle.py ${env}_profiling_result.csv # back
# do variant selection inside NN+C
# img_dim will be passed to it
# echo $train_size
# python3 perf_model.py $train_size # back
echo -e "Available hardware:"
echo -e "CPU-I5, CPU-I7, GPU-Tesla, GPU-Quadro"
echo -e "Profiling, training omitted for demo purposes"
sleep 5
python platform_select.py


# echo -e "Would you like to store the model? (y/n)"
# read store_choice
# if [[ $store_choice == y ]]; then
#     echo "Model stored"
#     # Connect to KB
#     # TODO: either way, will overwrite?
#     # can store in "${env}_profiling_result.csv" format
# fi