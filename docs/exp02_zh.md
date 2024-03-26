# Exp02 

## 1. 实验环境

| OS | Compiler | Build System Generator| Build Tool |
|:--:|:--------:|:---------------------:|:----------:|
| Ubuntu | g++-13 | cmake | make |
| Windows | msvc-19.3 | cmake | ninja |

## 2. 实验 1 多环境 OpenMP 程序的编译和运行
### 2.1. 实验目的
- 掌握 OpenMP 并行编程基础.
- 掌握在 Linux 上编译运行 OpenMP 程序.
- 掌握在 Windows 平台上编译运行 OpenMP 程序.

### 2.2. 实验步骤
#### 2.2.1. 项目搭建

项目主要结构如下:

```makefile
OpenMP/
├── CMakeLists.txt
├── bin/    # 这个目录由 cmake + make / ninjia 自动生成
├── build/  # 这个目录由 cmake 自动生成
├── scripts/
│   ├── build.ps1  # Windows 下使用 powershell 执行 build 脚本
│   └── build.sh   # Linux 下使用 shell 执行 build 脚本
├── include/
│   ├── TimeCounter.hpp
│   └── xRandom.hpp
└── exp02-openmp-src/
    ├── CMakeLists.txt
    ├── computePi_main.cpp
    ├── matmul_main.cpp
    └── hellomp_main.cpp
```

由于 CMake 和 shell 语言不是本次实验重点, 这里就不做过多的介绍.

#### 2.2.2. TimeCounter 和 xRandom

在 "./include/" 目录下有两个头文件, 其中分别定义了 `TimeCounter` 类和 `xRandom` 类. 他们的功能是提供欧尼较为便捷的计时功能和随机数生成功能. 由于 C++ 语言不是本次实验重点, 这里就省去了具体的实现方式介绍; 详情可以查看源码.

#### 2.2.3. Linux 下 build 可执行文件


