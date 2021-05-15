#!/bin/bash
# run with: bash autotuner.sh

# echo -e "Please enter the filename:"
# read filename # e.g., CMakeLists.txt
# echo -e "Please enter the environment:"
# read env # e.g., Halide-General
# echo -e "Please enter the image input size:"
# read img_dim # e.g., 1024

echo -e "Reading configuration file..."
file=$(cat config.txt | sed -n '1p')
env=$(cat config.txt | sed -n '2p')
reuse=$(cat config.txt | sed -n '3p')
echo -e "File containing tunable parameters=${file}"
echo -e "Environment=${env}"
echo -e "Using pre-trained models=${reuse}"

g++ -g -Wall -std=c++11 autotuner.cpp candidate_set.cpp -o autotuner

# Since candidate set may vary, the following allows reproducing results
# allow user to switch candidate set also
echo -e "Candidate set for this version of the task exists"
if test -f "candidate_set.csv"; then
    echo -e "Would you like to regenerate the candidate set? (y/n)"
    read regen_choice
    if [[ $regen_choice == y ]]; then
        echo -e "Generating candidate set..."
        ./autotuner ../../$filename $env
        echo -e "Candidate set successfully generated"
    fi
fi

./autotuner ../../$filename $env

train_size=7

# Check if previous profiling results already exists
if test -f "${env}_profiling_result.csv"; then
    echo -e "Performance model for this version of the task exists"
    # echo -e "Would you like to reuse it? (y/n)"
    # read reuse_choice
    if [[ $reuse == y ]]; then
        echo "Reusing the pre-trained model..."
        train_size=7
        # Connect to KB
    else 
        g++ -g -Wall -std=c++11 replace.cpp -o replace
        # Not reusing
        # delete profiling_result.csv
        rm ${env}_profiling_result.csv
        echo -e "Profiling..."
        while IFS=, read -r p1 p2
        do
            # generate CMakeLists.txt
            ./replace ../../CMakeLists.txt HalideSAR $p1 $p2
            cd .. # build folder
            make
            ts=$(date +%s%N)
            # ./sarbp -p ../data/Sandia/npy -o Sandia.png -d -45.0 -D 0.0 -t 30 -u 2
            ./sarbp -p ../data/AFRL/pass1/HH_npy -o AFRL.png -d -30.0 -D 0.0 -t 17 -u 2
            tt=$((($(date +%s%N) - $ts)/1000000))
            echo "Time taken: $tt milliseconds"
            # 4096, 16777216 are tentative values
            echo "$tt,4096,$p1,$p2,16777216" >> autotuner/${env}_profiling_result.csv
            cd autotuner/
        done < training_set.csv

        echo -e "Training performance prediction model..."
    fi
fi

# do variant selection inside NN+C
python perf_model.py $train_size 

# knowledge base (under dev)
cd KnowledgeBase/build/
./save
./load Halide