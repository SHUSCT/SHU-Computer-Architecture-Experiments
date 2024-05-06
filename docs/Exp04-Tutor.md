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
You should adjust the relevant parameters in the HPL.dat file **according to the hardware configuration of the node**.

- **Before Linpack**

  * For machines where the processor's brand is Intel , **turning off Hyper-Threading** improves HPL performance. This may occur because the core’s resources, such as cache, execution engine, and functional units, are shared between the two threads in a Hyper-Threaded core.
  * **Disabling Intel AVX-512 Instructions**. After Ice Lake CPUs using AVX-512, CPU's power and temperature will reach the limit, which will result to the fact that the maximum core frequency will be reduced. Meanwhile, while using Intel oneAPI Math Kernel Library, the default settings of Intel MKL also use AVX-512 instruction set.

- **Lines 5 to 6**

  ```shell
  1 # of problems sizes (N)
  143600 Ns 
  ```

  * N denotes the number and size of matrices to be solved. The larger the matrix size N, the larger the percentage of effective computation, and the higher the system's floating-point processing performance. However, the larger matrix size will result in more memory consumption, and if the system's actual memory space is insufficient, the use of cache will be significantly reduced the performance. 

  * It is optimal for the matrix to occupy about **80% of the total system memory**, i.e., N × N × 8 = total system memory × 80% (**where total memory is in bytes**).

- **Lines 7 to 8**

  ```shell
  1 # of NBs
  384 NBs 
  ```

  NBs indicate the size of the matrix chunks in the matrix solving process. The chunk size has a great impact on the performance, and the selection of NB is closely related to many factors of hardware and software.The selection of NB value is mainly based on the optimal value through actual testing, and generally follows the following rules:

  - NB can not be too large or too small, **generally less than 384.**
  - **NB × 8** must be a multiple of the **cache line**.
  - The size of the NB is related to the communication method, matrix size, network, processor speed, etc..

  Generally, a few better NB values can be obtained by single node or single CPU testing, but when the system size increases and the problem size becomes larger, the performance obtained from some NB values will decrease. **Therefore, it is recommended to select 3 NB values with good performance in small-scale testing, and then test these choices through large-scale testing.**

- **Lines 10 to 12**

  ```shell
  1 # of process grids (P x Q)
  1 Ps  
  1 Qs 
  ```

  P denotes the number of processors in the horizontal direction and Q denotes the number of processors in the vertical direction. P x Q denotes a two-dimensional processor grid. P x Q = number of processes. Generally one process corresponds to one CPU to get the best performance. The values of P and Q generally follow the following pattern:

  - **P x Q = Number of Pocesses.**
  - **P ≤ Q**. In general P takes a smaller value than Q because the amount of columnar communication (the number of communications and the amount of data communicated) is much larger than the horizontal communication.
  - **P is recommended to be chosen as a power of 2**. Horizontal communication in HPL uses the Binary Exchange method, and the performance is optimal when the number of processors in the horizontal direction is a power of two.
