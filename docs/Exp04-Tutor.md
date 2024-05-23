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

### 2.2. BLIS - AMD CPU

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

# [Prompt] >>> Do you want to set LP64 or ILP64 libraries as default libraries? (Enter 1 for LP64 / 2 for ILP64 / Default option: 1)
# <Enter 1>
```

Add following lines to the end of file `/etc/bash.bashrc` (Replace `<x.x.x>` with your BLIS version number; Use command `ls /usr/local/aocl` if you are not sure):

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

### 2.4. OpenBLAS - Whatever CPU

You can simply install OpenBLAS for any CPU.

Download the latest stable source code from the release page [[link](https://github.com/OpenMathLib/OpenBLAS/releases)].

For example, if you have downloaded `OpenBLAS-0.3.27.tar.gz`, extract it and you will get a directory called `OpenBLAS-0.3.27`. Change to it's directory and build the libraray:

```bash
tar -xf ./OpenBLAS-0.3.27.tar.gz

FC=gfortran make -j $(nproc)

sudo make PREFIX=/usr/local/openblas install
```

Add following lines to the end of file `/etc/bash.bashrc`:

```bash
# >>> OpenBLAS >>>
export OPENBLAS_HOME="/usr/local/openblas"
alias OPENBLAS_INIT="export PATH=$OPENBLAS_HOME/bin:$PATH && export LD_LIBRARY_PATH=$OPENBLAS_HOME/lib:$LD_LIBRARY_PATH"
# <<< OpenBLAS <<<
```

Now, every time you open a new terminal, you can initialize the environment of OpenBLAS with the following command:

```bash
OPENBLAS_INIT
```

## 3. Install HPL

### 3.1. Download HPL

Download the source code of HPL from the official website [[link](https://www.netlib.org/benchmark/hpl/)].

You should download the the latest release of hpl which named `hpl-2.3.tar.gz`.

Extract the file, and change to the project directory:

```bash
tar -xvf hpl-2.3.tar.gz && cd ./hpl-2.3
```

**Note**:
- You should check "README" for the detailed information. Here we will just provide some examples.

### 3.2. Configure HPL

#### 3.2.1. Configure for AMD CPU (with BLIS and OPENMPI)

Copy the template file we provide: [Make.Linux64_BLIS](../exp05-hpl/Make.Linux64_BLIS) into the project directory `hpl-2.3`:

Change following lines in the file to match your system configuration:
- Line 70, absolute path to hpl project directory (which is named `hpl-2.3`).
- Line 84, absolute path to your installed OpenMPI.
- Line 95, absolute path to your installed BLIS.

Build HPL:

```bash
make arch=Linux64_BLIS
```

#### 3.2.2. Configure for Intel CPU (with MKL and OneAPI)

To do.

#### 3.2.3. Configure for Any CPU (with OpenBLAS and OpenMPI)

Copy the template file we provide: [Make.Linux64_OpenBLAS](../exp05-hpl/Make.Linux64_OpenBLAS) into the project directory `hpl-2.3`:

Change following lines in the file to match your system configuration:
- Line 70, absolute path to hpl project directory (which is named `hpl-2.3`).
- Line 84, absolute path to your installed OpenMPI.
- Line 95, absolute path to your installed OpenBLAS.

Build HPL:

```bash
make arch=Linux64_OpenBLAS
```

### 3.3. Test for Installation

```bash
cd bin/Linux64_AMD

# AMD CPU >>>>>>
OPENMPI_INIT
BLIS_INIT
# AMD CPU <<<<<<

# Intel CPU >>>>>>
# To do.
# Intel CPU <<<<<<

mpirun -np 4 ./xhpl
```

## 4. Tuning HPL

Change to the binary directory of HPL which should be inside its project directory.

```bash
cd /path/to/hpl-2.3/bin/<your-configured-arch>
```

You should adjust the relevant parameters in file `HPL.dat` **according to your hardware configuration**.

- **Before Linpack**

  - **Turn off Hyper-Threading**  
  For machines where the processor's brand is Intel , **turning off Hyper-Threading** improves HPL performance. This may occur because the core’s resources, such as cache, execution engine, and functional units, are shared between the two threads in a Hyper-Threaded core.

  - **Disabling Intel AVX-512 Instructions**.  
  After Ice Lake CPUs using AVX-512, CPU's power and temperature will reach the limit, which will result to the fact that the maximum core frequency will be reduced. Meanwhile, while using Intel oneAPI Math Kernel Library, the default settings of Intel MKL also use AVX-512 instruction set.

- **Line 5 - 6**

  ```shell
  1 # of problems sizes (N)
  143600 Ns 
  ```

  - N denotes the number and size of matrices to be solved. The larger the matrix size N, the larger the percentage of effective computation, and the higher the system's floating-point processing performance. However, the larger matrix size will result in more memory consumption, and if the system's actual memory space is insufficient, the use of cache will be significantly reduced the performance. 

  - It is optimal for the matrix to occupy about **80% of the total system memory**, i.e., N × N × 8 = total system memory × 80% (**where total memory is in bytes**).

  - To check system memory in bytes: `free -b`.


- **Line 7 - 8**

  ```shell
  1 # of NBs
  384 NBs 
  ```

  NBs indicate the size of the matrix chunks in the matrix solving process. The chunk size has a great impact on the performance, and the selection of NB is closely related to many factors of hardware and software.The selection of NB value is mainly based on the optimal value through actual testing, and generally follows the following rules:

  - NB can not be too large or too small, **generally less than 384.**
  - **NB × 8** must be a multiple of the **cache line**.
  - The size of the NB is related to the communication method, matrix size, network, processor speed, etc..

  Generally, a few better NB values can be obtained by single node or single CPU testing, but when the system size increases and the problem size becomes larger, the performance obtained from some NB values will decrease. **Therefore, it is recommended to select 3 NB values with good performance in small-scale testing, and then test these choices through large-scale testing.**

- **Line 10 - 12**

  ```shell
  1 # of process grids (P x Q)
  1 Ps  
  1 Qs 
  ```

  P denotes the number of processors in the horizontal direction and Q denotes the number of processors in the vertical direction. P x Q denotes a two-dimensional processor grid. P x Q = number of processes. Generally one process corresponds to one CPU to get the best performance. The values of P and Q generally follow the following pattern:

  - **P x Q = Number of Pocesses.**
  - **P ≤ Q**. In general P takes a smaller value than Q because the amount of columnar communication (the number of communications and the amount of data communicated) is much larger than the horizontal communication.
  - **P is recommended to be chosen as a power of 2**. Horizontal communication in HPL uses the Binary Exchange method, and the performance is optimal when the number of processors in the horizontal direction is a power of two.
