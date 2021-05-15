# General-Purpose Auto-Tuner for HPC Applications

---

## Dependencies

for **performance prediction**:
* python >= 3.6.5
* tensorflow >= 1.8.0
* numpy >= 1.16.1
* pandas >= 0.23.0
* scikit-learn >= 0.22.2
* scipy >= 0.16.0

for **knowledge base**:
* [Boost](https://www.boost.org/) >= 1.71.0

## Building

No action needed from the users.

## Configuration

Change the parameters within the configuration file `config.txt` according to the current setting of the task. Parameters within `config.txt` are:

### 1. Environment

The auto-tuner can currently operates in the following environments:

#### 1.1 `Halide-General`

Halide kernels whose algorithm and schedule are written in a single `.cpp` file. For example, the Blur kernel written [here](https://github.com/Naifeng/Variant-Selection/blob/master/Halide/Blur/blur_cpu/experiments/blur_cpu.cpp). 

#### 1.2 `Halide-SAR`

Halide SAR kernel written [here](https://github.com/ISI-apex/halide-sar-app). 

#### 1.3 `cpplib`

Kernels written in c++ using libraries such as Eigen and Boost. For example, the Matrix-Matrix Multiplication kernel written [here](https://github.com/Naifeng/Augmented-Neural-Network/blob/master/data_generation/cpu/boost/MM_C_boost.cpp). 

#### 1.4 `cuda` 

Kernels written in CUDA. For example, the Matrix-Matrix Multiplication kernel written [here](https://github.com/Naifeng/Augmented-Neural-Network/blob/master/data_generation/cpu/boost/MM_C_boost.cpp). 

### 2. File containing tunable parameters

The relative path of the file containing tunable parameters. For example, `CMakeLists.txt`. 

The auto-tuner needs write access to this file for automatic profiling. 

### 3. Using pre-trained models (y/n)

If the auto-tuner detects that there exists a pre-trained model for the task:

`y`: the auto-tuner uses the pre-trained model and therefore skips the profiling and training process. 

`n`: the auto-tuner profiles and trains a new model. 

### 4. Target hardware & input sizes (*under development*) 

For each input size, the auto-tuner first determines which target hardware is most suitable for running the task and then outputs the variant-selection result.  

For example, target hardware=`cpu, gpu`, input sizes=`1024,2048,4096`. 

## Usage

Start the auto-tuner by:
```
bash autotuner.sh
```

## Code Walkthrough

**Note that the following code is under development and is subject to change.** 

Read from the configuration file `config.txt`.
```
echo -e "Reading configuration file..."
file=$(cat config.txt | sed -n '1p')
env=$(cat config.txt | sed -n '2p')
reuse=$(cat config.txt | sed -n '3p')
echo -e "File containing tunable parameters=${file}"
echo -e "Environment=${env}"
echo -e "Using pre-trained models=${reuse}"
```

Build the autotuner.
```
g++ -g -Wall -std=c++11 autotuner.cpp candidate_set.cpp -o autotuner
```

Run the autotuner. Candidate set will be generated in this step. 
```
./autotuner ../../$filename $env
```

If the pre-trained model exists and the user choose to reuse it, skip the next step. 
```
if [[ $reuse == y ]]; then
    echo "Reusing the pre-trained model..."
```

The auto-tuner automatically profiles by iteratively changing the tunable parameters in `$file` according to the candidate set. 
```
else 
    g++ -g -Wall -std=c++11 replace.cpp -o replace
    # delete profiling_result.csv
    rm ${env}_profiling_result.csv
    echo -e "Profiling..."
    while IFS=, read -r p1 p2
    do
        # generate tunable file 
        ./replace ../../$file $env $p1 $p2
        cd .. 
        make
        ts=$(date +%s%N)
        # tentative for $env=Halide-SAR
        ./sarbp -p ../data/AFRL/pass1/HH_npy -o AFRL.png -d -30.0 -D 0.0 -t 17 -u 2
        tt=$((($(date +%s%N) - $ts)/1000000))
        echo "Time taken: $tt milliseconds"
        # 4096, 16777216 are tentative values
        # if parameter#4 is specified, those values will be read from config.txt
        echo "$tt,4096,$p1,$p2,16777216" >> autotuner/${env}_profiling_result.csv
        cd autotuner/
    done < training_set.csv
    echo -e "Training performance prediction model..."
fi
```

Train performance prediction models.
```
python perf_model.py $train_size
```

Save the trained models to the knowledge base
```
cd KnowledgeBase/build/
./save
```

Conduct variant-selection.
```
./load $env
```

## Acknowledgments

* This work is supported by the Defense Advanced Research Projects Agency (DARPA), under the Performant Automation of Parallel Program Assembly (PAPPA) project, at the University of Southern California.