#! /bin/bash
#
#PBS -l walltime=00:10:00
#PBS -l nodes=1
#PBS -W group_list=newriver
#PBS -q open_q
#PBS -j oe

cd $PBS_O_WORKDIR

module purge
module load gcc
module load openmpi

make

mpiexec -n $1 main;
mpiexec -n $2 main;
mpiexec -n $4 main;
mpiexec -n $8 main;
mpiexec -n $12 main;
mpiexec -n $16 main;
mpiexec -n $20 main;
