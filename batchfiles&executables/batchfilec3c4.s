#!/bin/sh
#
#SBATCH --verbose
#SBATCH --job-name=test
#SBATCH --time=00:30:00
#SBATCH --partition=c29
#SBATCH --nodes=1
#SBATCH --mem=32GB
#SBATCH --cpus-per-task=08

module load python3/intel/3.6.3
module load numpy/python3.6/intel/1.14.0
python lab1-c3-c4.py
