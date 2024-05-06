# Exp04 Tutor: HPL Benchmark

## 1. Introduction to HPL

HPL (High Performance Linpack) is a software package that solves a (random) dense linear system in double precision (64 bits) arithmetic on distributed-memory computers. It can be used to measure the performance of a computer system.

## 2. Install BLAS

### 2.1. Introduction to BLAS

BLAS (Basic Linear Algebra Subprograms) is a specification that prescribes a set of low-level routines for performing common linear algebra operations such as vector addition, scalar multiplication, dot products, linear combinations, and matrix multiplication. BLAS is a library that provides a set of efficient routines for performing basic linear algebra operations.

### 2.2. CPU Information

Before installing BLAS and LAPACK, you need to know your cpu. You can use the following command to check your CPU information:

```bash
lscpu
```

### 2.2. AMD CPU

If you have an AMD CPU, you should install **BLIS** for BLAS.

Go to the official website of BLIS [[link](https://www.amd.com/en/developer/aocl.html)], download "**AOCL X.X binary packages compiled with GCC Y.Y**", and you will get a file named "**aocl-linux-gcc-X.X.X.tar.gz**".

Extract the file, and you will get a folder named "**aocl-linux-gcc-X.X.X**":

```bash
tar -xf aocl-linux-gcc-X.X.X.tar.gz

cd ./aocl-linux-gcc-X.X.X
```

Install BLIS:

```bash
sudo bash ./install.sh -t /usr/local/aocl
```

Add following lines to the end of **/etc/bash.bashrc** (Replace `<x.x.x>` with your BLIS version number; Use command `ls /usr/local/aocl` if you are not sure):

```bash
# >>> BLIS >>>
export BLIS_HOME="/usr/local/aocl/<x.x.x>/gcc"
alias BLIS_INIT="source $BLIS_HOME/amd-libs.cfg"
# <<< BLIS <<<
```

Now, every time you open a new terminal, you can initialize the environment of BLIS with the following command:

```bash
BLIS_INIT
```

### 2.3. Intel CPU

To do.

## 3. Install HPL

### 3.1. Download HPL

Download the source code of HPL from the official website [[link](https://www.netlib.org/benchmark/hpl/)].

You should download the file named "**hpl-X.X.tar.gz**".

Extract the file, and you will get a folder named "**hpl-X.X**":

```bash
tar -xvf hpl-X.X.tar.gz

cd hpl-X.X
```

**Note**:
- You should check "README" for the detailed information. Here we will just provide some examples.

### 3.2. Configure HPL

#### 3.2.1. Configure for AMD CPU

Copy the template file [Make.Linux64_AMD](../scripts/Make.Linux64_AMD) into the directory "**hpl-X.X**":

Change following lines in the file to match your system configuration:
- Line 84, path to OpenMPI.
- Line 95, path to BLAS(BLIS).

Build HPL:

```bash
make arch=Linux64_AMD
```

#### 3.2.2. Configure for Intel CPU

To do.

### 3.3. Test for Installation

```bash
cd bin/Linux64_AMD

# For AMD CPU >>>>>>
OPENMPI_INIT
BLIS_INIT
# For AMD CPU <<<<<<

# For Intel CPU >>>>>>
# To do.
# For Intel CPU <<<<<<

mpirun -np 4 ./xhpl
```

## 4. Tuning HPL
To do.