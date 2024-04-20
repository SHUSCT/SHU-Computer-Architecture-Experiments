# Env Setup Guide for Linux

This is guide is only for Ubuntu. If you are using other Linux distributions, please check the corresponding package manager to install the required softwares.

## 1. Install Build Essentials

Install build essentials:

```bash
sudo apt-get install build-essential
```

Check gcc version:

```bash
gcc -v  # or g++ -v
```

If you are using ubuntu 23, the outputs would imply the installed gcc version is "13.x.x".

If the output version is lower than 13, please check [this article](https://github.com/jamesnulliu/My_Notes/blob/main/Linux/Use_Cxx20_in_Ubuntu22.md).


## 2. Install CMake

```bash
sudo apt-get install cmake
```

## 4. Install MPI

### 4.1. OpenMPI

Download the source code in [OpenMPI Official Website](https://www.open-mpi.org/software/ompi/v5.0/).

Supposed that you have downloaded **openmpi-5.0.3.tar.bz2**.  

Open your terminal and change directory to the path of the compressed file.

```bash
# Extact the file, and you will get a folder named "openmpi-5.0.3"
tar -xf openmpi-5.0.3.tar.bz2

cd ./openmpi-5.0.3

./configure --prefix=/opt/openmpi-5.0.3

make -j $(nproc) all

sudo make install
```

Edit "/etc/bash.bashrc" with command `sudo vim /etc/bash.bashrc`, add following lines to the end of that file:

```bash
# >>> openmpi >>>
export OPENMPI_HOME="/opt/openmpi-5.0.3"
alias OPENMPI_INIT="export PATH=$OPENMPI_HOME/bin:$PATH && export LD_LIBRARY_PATH=$OPENMPI_HOME/lib:$LD_LIBRARY_PATH"
# <<< openmpi <<<
```

Now, every time you open a new terminal, you can initialize the environment of OpenMPI with the following command: 

```bash
OPENMPI_INIT
```

You can also check the path of mpi executables after initialized the environment:

```bash
# Check where is mpicxx
which mpicxx
# Check where is mpirun
which mpirun
```