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