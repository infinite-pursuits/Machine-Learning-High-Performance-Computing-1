#!/bin/sh
#
#SBATCH --verbose
#SBATCH --job-name=test
#SBATCH --time=00:30:00
#SBATCH --partition=c28
#SBATCH --nodes=1
#SBATCH --mem=32GB
#SBATCH --cpus-per-task=08

module load gcc/6.3.0
module load intel/17.0.1

gcc -W -Wall -O2 -I /share/apps/intel/17.0.1/mkl/include -D_GNU_SOURCE lab1-c5-c6.c -o lab1-c5-c6 -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lm -L/share/apps/intel/17.0.1/mkl/lib/intel64  && ./lab1-c5-c6
