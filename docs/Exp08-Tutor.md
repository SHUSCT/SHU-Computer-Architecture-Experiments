# Exp08 Tutor: NFS

## 1. Introduction to Network File System

NFS or Network File System is a distributed file system protocol that allows you to share directories over a network. By using NFS shared storage, system administrators can consolidate resources onto centralized servers on the network. A client system can access the remote share with (**read, write**) privileges and do not have access to the underlying block storage. You can mount remote directories on your system and work with the files on the remote machine as if they were local files. 

![network file system](imgs/nfs.png)

Below are the versions of NFS supported by Ubuntu 22.04:

#### NFS version 3 (NFSv3)

- Has support for safe asynchronous writes and is more robust at error handling than the previous NFSv2
- Supports 64-bit file sizes and offsets, allowing clients to access more than 2 GB of file data.

#### NFS version 4 (NFSv4)

- Works through firewalls and on the Internet
- No longer requires rpcbind service
- Supports Access Control Lists (ACLs)
- Utilizes stateful operations.

In this tutor , we will explain how to set up an NFSv4 Server on Ubuntu 22.04. We’ll also show you how to mount an NFS file system on the client machine.

## 2. Prerequisites

### 2.1 Network configuration

We’ll use two machines, one running Ubuntu 22.04, which will act as an NFS server, and another one running Ubuntu 22.04 or any other Linux distribution on which we will mount the share. The server and the clients should be able to communicate with each other over a private network. You can use public IP addresses and configure the server firewall to allow traffic on port only from trusted sources.

The machines in this example have the following IPs:

| Server         | Private IP    | Public IP     |
| -------------- | ------------- | ------------- |
| NFS Server IP  | 192.168.0.206 | 1.94.105.187  |
| NFS Clients IP | 192.168.0.52  | 60.204.148.88 |

### 2.2 Setting server hostname

```shell
sudo hostnamectl set-hostname NameYouWant
```

## 3. Setting up the NFS server

The first step is to set up the NFS server. We’ll install the necessary packages, create and export the NFS directories, and configure the firewall.

### 3.1 Installing the NFS server

The NFS server package provides user-space support needed to run the NFS kernel server. To install the package, run:

```shell
sudo apt-get update
sudo apt-get install nfs-kernel-server -y
```

Once the installation is completed, the NFS services will start automatically. On Ubuntu 22.04, NFS version 2 is disabled. Versions 3 and 4 are enabled. You can verify that by running the `cat` command.

```shell
root@Server:~# sudo cat /proc/fs/nfsd/versions
-2 +3 +4 +4.1 +4.2
```

NFSv2 is pretty old now, and there is no reason to enable it. NFS server configuration is defined in and files. The default settings are sufficient for most situations. `/etc/default/nfs-kernel-server` `/etc/default/nfs-common`

### 3.2 Creating the file systems

The NFSv4 server uses a global root directory, and the exported directories are relative to this directory. You can link the share mount point to the directories you want to export using bind mounts.

In this example, we’ll set the directory as NFS root. To better explain how the NFS mounts can be configured, we’re going to share two directories ( and ) with different configuration settings. The is owned by the user , and is owned by .`/srv/nfs4``/var/www``/opt/backups``/var/www/``www-data``/opt/backups``root`

First create the root directory and the share mount points:

```
sudo mkdir -p /srv/nfs4/backupssudo mkdir -p /srv/nfs4/wwwCopyCopy
```

Bind mount the directories to the share mount points:

```
sudo mount --bind /opt/backups /srv/nfs4/backupssudo mount --bind /var/www /srv/nfs4/wwwCopyCopy
```

To make the bind mounts permanent across reboots, open the file:`/etc/fstab`

```
sudo nano /etc/fstabCopy
```

and add the following lines:

/etc/fstab

```ini
/opt/backups /srv/nfs4/backups  none   bind   0   0
/var/www     /srv/nfs4/www      none   bind   0   0
```

### 3.3 Exporting the file systems

The next step is to add the file systems that will be exported and the clients allowed to access those shares to the file.`/etc/exports`

Each line for an exported file system has the following form:

```txt
export host(options)
```

Where is the exported directory, is a hostname or IP address/range that can access the export, and are the host options.`export``host``options`

Open the file and add the following lines:`/etc/exports`

```
sudo nano /etc/exportsCopy
```

/etc/exports

```ini
/srv/nfs4         192.168.33.0/24(rw,sync,no_subtree_check,crossmnt,fsid=0)
/srv/nfs4/backups 192.168.33.0/24(ro,sync,no_subtree_check) 192.168.33.3(rw,sync,no_subtree_check)
/srv/nfs4/www     192.168.33.20(rw,sync,no_subtree_check)
```

The first line contains the option, which define the NFS root directory (). Access to this NFS volume is allowed only to the clients from the subnet. The option is required to share directories that are sub-directories of an exported directory.`fsid=0``/srv/nfs4``192.168.33.0/24``crossmnt`

The second line shows how to specify multiple export rules for one filesystem. The read access is allowed to the whole range, and both read and write access only to the IP address. The option tells NFS to write changes to the disk before replying.`192.168.33.0/24``192.168.33.3``sync`

The last line is self-explanatory. For more information about all the available options type in your terminal.`man exports`

Save the file and export the shares:

```
sudo exportfs -arCopy
```

You need to run the command above each time you modify the file. If there are any errors or warnings, they will be shown on the terminal.`/etc/exports`

To view the current active exports and their state, use:

```
sudo exportfs -vCopy
```

The output will include all shares with their options. As you can see there are also options that we haven’t define in the file. Those are default options and if you want to change them you’ll need to explicitly set those options.`/etc/exports`

```output
/srv/nfs4/backups
		192.168.33.3(rw,wdelay,root_squash,no_subtree_check,sec=sys,rw,secure,root_squash,no_all_squash)
/srv/nfs4/www 	192.168.33.20(rw,wdelay,root_squash,no_subtree_check,sec=sys,rw,secure,root_squash,no_all_squash)
/srv/nfs4     	192.168.33.0/24(rw,wdelay,crossmnt,root_squash,no_subtree_check,fsid=0,sec=sys,rw,secure,root_squash,no_all_squash)
/srv/nfs4/backups
		192.168.33.0/24(ro,wdelay,root_squash,no_subtree_check,sec=sys,ro,secure,root_squash,no_all_squash)
Copy
```

On Ubuntu, is enabled by default. This is one of the most important options concerning NFS security. It prevents root users connected from the clients from having root privileges on the mounted shares by mapping root and to / /.`root_squash``UID``GID``nobody``nogroup``UID``GID`

In order for the users on the client machines to have access, NFS expects the client’s user and group ID’s to match with those on the server. Another option is to use the NFSv4 idmapping feature that translates user and group IDs to names and the other way around.

That’s it. At this point, you have set up an NFS server on your Ubuntu server. You can now move to the next step and configure the clients and connect to the NFS server.

### 3.4 Firewall configuration

If you are installing Jenkins on a remote Ubuntu server that is protected by a [firewall](https://linuxize.com/post/how-to-setup-a-firewall-with-ufw-on-ubuntu-20-04/) , you’ll need to enable traffic on the NFS port:

```
sudo ufw allow from 192.168.33.0/24 to any port nfsCopy
```

Verify the change:

```
sudo ufw statusCopy
```

The output should show that the traffic on port is allowed:`2049`

```output
To                         Action      From
--                         ------      ----
2049                       ALLOW       192.168.33.0/24           
22/tcp                     ALLOW       Anywhere                  
22/tcp (v6)                ALLOW       Anywhere (v6)  
Copy
```

## 4. Setting Up the NFS Clients

Now that the NFS server is set up and shares are exported, the next step is to configure the clients and mount the remote file systems.

We’ll focus on Linux systems, but you can also [mount the NFS share](https://linuxize.com/post/how-to-mount-an-nfs-share-in-linux/) on macOS and Windows machines.

### 4.1 Installing the NFS client

On the client machines, we need to install only the tools required to mount a remote NFS file system.

- **Install NFS client on Debian and Ubuntu**

  The name of the package that includes programs for mounting NFS file systems on Debian-based distributions is . To install it, run:`nfs-common`

  ```
  sudo apt updatesudo apt install nfs-commonCopyCopy
  ```

- **Install NFS client on CentOS and Fedora**

  On Red Hat and its derivatives, install the package:`nfs-utils`

  ```
  sudo yum install nfs-utilsCopy
  ```

### 4.2 Mounting file systems

We’ll work on the client machine with IP , which has read and write access to the file system and read-only access to the file system.`192.168.33.20``/srv/nfs4/www``/srv/nfs4/backups`

Create two new directories for the mount points:

```
sudo mkdir -p /backupssudo mkdir -p /srv/wwwCopyCopy
```

You can create the directories at any location you want.

Mount the exported file systems with the [`mount`](https://linuxize.com/post/how-to-mount-and-unmount-file-systems-in-linux/) command:

```
sudo mount -t nfs -o vers=4 192.168.33.10:/backups /backupssudo mount -t nfs -o vers=4 192.168.33.10:/www /srv/wwwCopyCopy
```

Where is the IP of the NFS server. You can also use the hostname instead of the IP address, but it needs to be resolvable by the client machine. This is usually done by mapping the hostname to the IP in the [`/etc/hosts`](https://linuxize.com/post/how-to-edit-your-hosts-file/) file.`192.168.33.10`

When mounting an NFSv4 filesystem, omit the NFS root directory. Use , instead of .`/backups``/srv/nfs4/backups`

Verify that the remote file systems are mounted successfully using either the mount or [`df`](https://linuxize.com/post/how-to-check-disk-space-in-linux-using-the-df-command/) command:

```
df -hCopy
```

The command will print all mounted file systems. The last two lines are the mounted shares:

```oputput
Filesystem              Size  Used Avail Use% Mounted on
udev                    951M     0  951M   0% /dev
tmpfs                   199M  676K  199M   1% /run
/dev/sda3               124G  2.8G  115G   3% /
tmpfs                   994M     0  994M   0% /dev/shm
tmpfs                   5.0M     0  5.0M   0% /run/lock
tmpfs                   994M     0  994M   0% /sys/fs/cgroup
/dev/sda1               456M  197M  226M  47% /boot
tmpfs                   199M     0  199M   0% /run/user/1000
192.168.33.10:/backups  124G  2.8G  115G   3% /backups
192.168.33.10:/www      124G  2.8G  115G   3% /srv/wwwCopy
```

To make the mounts permanent on reboot, open the file and add the following lines::`/etc/fstab`

```
sudo nano /etc/fstabCopy
```

/etc/fstab

```ini
192.168.33.10:/backups /backups   nfs   defaults,timeo=900,retrans=5,_netdev	0 0
192.168.33.10:/www /srv/www       nfs   defaults,timeo=900,retrans=5,_netdev	0 0
```

Copy

For information about the available options when mounting an NFS file system, type in your terminal.`man nfs`

Another option to mount remote file systems is to use either the tool or to create a systemd unit.`autofs`

### 4.3 Testing NFS Access

Let’s test the access to the shares by [creating a new file](https://linuxize.com/post/create-a-file-in-linux/) on each of them.

First, try to create a test file to the directory using the [`touch`](https://linuxize.com/post/create-a-file-in-linux/) command:`/backups`

```
sudo touch /backups/test.txtCopy
```

The file system is exported as read-only and as expected you will see a error message:`/backup``Permission denied`

```output
touch: cannot touch ‘/backups/test’: Permission denied
Copy
```

Next, try to create a test file to the directory as a root using the [`sudo`](https://linuxize.com/post/sudo-command-in-linux/) command:`/srv/www`

```
sudo touch /srv/www/test.txtCopy
```

Again, you will see message.`Permission denied`

```output
touch: cannot touch ‘/srv/www’: Permission denied
Copy
```

If you recall, the [directory is owned](https://linuxize.com/post/linux-chown-command/) by the user, and this share has option set which maps the root user to the user and group that doesn’t have write permissions to the remote share.`/var/www``www-data``root_squash``nobody``nogroup`

Assuming that you have a use on the client machine with the same and as on the remote server (which should be the case if, for example, you [installed nginx](https://linuxize.com/post/how-to-install-nginx-on-ubuntu-20-04/) on both machines), you can try to create a file as user :`www-data``UID``GID``www-data`

```
sudo -u www-data touch /srv/www/test.txtCopy
```

The command will show no output which means the file was successfully created.

To verify it list the files in the directory:`/srv/www`

```
ls -la /srv/wwwCopy
```

The output should show the newly created file:

```output
drwxr-xr-x 3 www-data www-data 4096 Apr 10 22:18 .
drwxr-xr-x 3 root     root     4096 Apr 10 22:29 ..
-rw-r--r-- 1 www-data www-data    0 Apr 10 21:58 index.html
-rw-r--r-- 1 www-data www-data    0 Apr 10 22:18 test.txt
Copy
```

### 4.4 Unmounting NFS File System

If the remote NFS share is no longer needed, you can unmount it as any other mounted file system using the command.`umount`

For example, to unmount the share, you would run:`/backup`

```
sudo umount /backupsCopy
```

If the mount point is defined in the file, make sure you remove the line or comment it out by adding at the beginning of the line.`/etc/fstab``#`

## 4. Conclusion

We have shown you how to set up an NFS server and how to mount the remote file systems on the client machines. If you’re implementing NFS in production and sharing sensible data, it is a good idea to enable kerberos authentication.

By default, the NFS protocol is not encrypted and does not provide user authentication. Access to the server is restricted by the client’s IP addresses or hostnames. 

As an alternative to NFS, you can use SSHFS (SSH Filesystem) to mount remote directories over an SSH connection. SSHFS is encrypted by default and much easier to configure and use.

Feel free to leave a comment if you have any questions.