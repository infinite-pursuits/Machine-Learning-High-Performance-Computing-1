#!/bin/sh
#
#SBATCH --verbose
#SBATCH --job-name=test
#SBATCH --time=00:30:00
#SBATCH --partition=c29
#SBATCH --nodes=1
#SBATCH --mem=32GB
#SBATCH --cpus-per-task=08

module load gcc/6.3.0
gcc -W -Wall -O3 ./lab1-c1-c2.c -o lab1-c1-c2 && ./lab1-c1-c2
