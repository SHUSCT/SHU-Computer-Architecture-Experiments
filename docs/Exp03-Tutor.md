# Exp03 Tutor

## 1. Install VMWare Workstation Pro

## 2. Download Ubuntu 23 Desktop iso File

Official website: [link](https://ubuntu.com/download/desktop).

Official CD mirrors: [link](https://launchpad.net/ubuntu/+cdmirrors).

Download the file named something like "**ubuntu-23.10.1-desktop-amd64.iso**".

## 3. Install Three Ubuntu 23 Virtual Machines in VMWare

💡**Note**:
- Set the computer name to be "node1", "node2" and "node3" repectively.
- Create users with the same user name on each virtual machine.

## 4. Environment Setup

### 4.1. Software Installation

Run following commands on every virtual machine:

```bash
sudo apt update

sudo apt install -y open-vm-tools open-vm-tools-desktop
```

Restart every virtual machine, and now you are able to copy and paste content or files from your host machine to them!

Install some other softwares on every virtual machine:

```bash
sudo apt install -y build-essential cmake git vim-gtk3 openssh-server

# Turn down the firewall for ssh:
sudo ufw allow ssh
```

### 4.2. SSH Key Generation

Generate ssh keys on every virtual machine:

```bash
# Replace <1/2/3> with the corresponding number of the virtual machine
ssh-keygen -t rsa -b 4096 -C "node<1/2/3>@ubuntu"
```

You should be able to find the generated keys in `~/.ssh/` directory.

### 4.3. SSH Key Sharing

Check the ip address of every virtual machine:

```bash
ip a
```

Suppose you get following results:

| Host Name | IP Address |
| :---: | :---: |
| node1 | 192.168.x.10 |
| node2 | 192.168.x.11 |
| node3 | 192.168.x.12 |

💡**Note**: 
- These IP addresses are just examples, you should replace them with your own IP addresses.
- Try to ping every virtual machine from each other to make sure they can communicate with each other.

Share the public key of every virtual machine to every other virtual machine:

```bash
# Replace <1/2/3> with the corresponding number of the virtual machine
# Replace <IP Address> with the corresponding IP address of the virtual machine
ssh-copy-id node<1/2/3>@<IP Address>
```

The public keys are now written to the `~/.ssh/authorized_keys` file on every virtual machine.

### 4.4. Hosts File Configuration

Edit the `/etc/hosts` file on every virtual machine:

```bash
sudo vim /etc/hosts
```

Add following lines to the file:

```bash
192.168.x.10 node1
192.168.x.11 node2
192.168.x.12 node3
```

💡**Note**: 
- Replace `192.168.x.10`, `192.168.x.11` and `192.168.x.12` with your own IP addresses.

### 4.5. Reboot Every Virtual Machine

Reboot every virtual machine. 

### 4.6. Try to SSH to Every Virtual Machine

On `node1`, try to ssh to `node2` and `node3`:

```bash
ssh node2
# Add the fingerprint to the known_hosts file
exit
ssh node3
# Add the fingerprint to the known_hosts file
exit
```

On `node2`, try to ssh to `node1` and `node3`:

```bash
ssh node1
# Add the fingerprint to the known_hosts file
exit
ssh node3
# Add the fingerprint to the known_hosts file
exit
```

On `node3`, try to ssh to `node1` and `node2`:

```bash
ssh node1
# Add the fingerprint to the known_hosts file
exit
ssh node2
# Add the fingerprint to the known_hosts file
exit
```

## 5. Install zlib

Download source code of zlib from the official website: [link](https://zlib.net/).

You can download it on your host machine and then copy it to every virtual machine.

Suppose your downloaded file is named **zlib-1.3.1.tar.gz**.

Extract the file and install it:

```bash
tar -xf zlib-1.3.1.tar.gz

cd zlib-1.3.1

./configure --prefix=/usr/local/zlib-1.3.1

make -j $(nproc) all

sudo make install
```

Edit **/etc/bash.bashrc** file on every virtual machine with command `sudo vim /etc/bash.bashrc`.  Add following lines to the file:

```bash
export ZLIB_HOME=/usr/local/zlib-1.3.1

export LD_LIBRARY_PATH=$ZLIB_HOME/lib:$LD_LIBRARY_PATH
```

Then run `source /etc/bash.bashrc` to make the changes take effect.

## 6. Install openmpi

Download source code of openmpi from the official website: [link](https://www.open-mpi.org/software/).

Supposed the file you downloaded is named **openmpi-5.0.3.tar.bz2**.  

Open your terminal and change directory to the path of the compressed file.

```bash
# Extact the file, and you will get a folder named "openmpi-5.0.3"
tar -xf openmpi-5.0.3.tar.bz2

cd ./openmpi-5.0.3

./configure --prefix=/usr/local/openmpi-5.0.3

make -j $(nproc) all

sudo make install
```

Edit **/etc/bash.bashrc** file with command `sudo vim /etc/bash.bashrc`, add following lines to the end of that file:

```bash
# >>> openmpi >>>
export OPENMPI_HOME="/usr/local/openmpi-5.0.3"
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