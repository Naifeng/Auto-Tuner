# GenMAT

GenMAT is a portable tool for identifying the best variant of any application specified as a meta-program with exposed tunable parameters on any hardware. 

GenMAT automatically profiles the application by varying the exposed tunable parameters to generate a small set of profiling data. Then, GenMAT trains a compact machine learning model that is used to quickly predict the runtimes of a large number of possible parameter settings to identify the best variant.

For more details, please view the full paper [here](https://naifeng.github.io/assets/pdf/PEHC_GenMAT_2021.pdf).

---

*GenMAT is still being actively developed for new features. Please do not hesitate to contact me if you find any bug or have any questions.*

## Dependencies

GenMAT is developed on macOS 10.15.7 with the following dependencies. 

for **auto-tuner bash script**:

* bash = 3.2.57
* python = 3.6.5
* clang++ = 12.0.0
* md5sum (GNU coreutils) = 8.32
* gdate (GNU coreutils) = 8.32

for **performance prediction**:

* tensorflow = 1.8.0
* numpy = 1.16.1
* pandas = 0.23.0
* scikit-learn = 0.22.2
* scipy = 0.16.0

## Usage

Details can be found at Section III-A and III-B of paper "GenMAT: A General-Purpose Machine Learning-Driven Auto-Tuner for Heterogeneous Platforms."

[comment]: <> (add link)

### Program to Meta-Program

1. specify the tunable parameters
2. read the command line arguments and assign them to tunable parameters accordingly
3. (Optional) enforce constraints on parameters

### Configuration File

Change the parameters within `config.txt` according to the current setting of the task. 

### Running GenMAT

Start GenMAT by:
```
bash genmat.sh
```

### Running on Linux
Change `gdate` to `date` in `genmat.sh` when running on Linux. 

## Code Walkthrough

The code walthrough is now comments in `genmat.sh`. 
