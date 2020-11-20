# ReDroid Kernel Modules
This repository contains the kernel modules necessary to run the Android containers.

Currently, only **4.14+** Kernel is supported. Many Linux distributions already meet 
this requirement (Ubuntu 16.04+, AmazonLinux 2, CentOS 8+ etc.). If you are using a 
customized kernel, make sure the corresponding kernel headers are present in your system.

Suggest use **non production** machines to try these modules.

## Build & Deploy
- [Manual](#manual)
- [DKMS](#dkms)
- [Docker](#docker)
- [K8S](#k8s)

## Manual
```bash
# Ubuntu / Debian etc.
apt install make gcc linux-headers-`uname -r`
make # build kernel modules
make install # build and install *unsigned* kernel modules

# AmazonLinux2 / CentOS / RHEL
yum install make "kernel-devel-uname-r == `uname -r`"
make # build kernel modules
make install # build and install *unsigned* kernel modules

# check modules status
lsmod | grep -e ashmem_linux -e binder_linux
# example output:
# binder_linux          147456  79
#ashmem_linux           16384  23

# we can also check like this
cat /proc/filesystems | grep binder # output should like: nodev	binder
cat /proc/misc | grep ashmem # output should like: 56 ashmem
```
Please refer the related docs if you want to sign these kernel modules.

## DKMS
DKMS can help to automatically build and deploy kernel modules if kernel upgraded.
You need to have `dkms` and linux-headers on your system. You can install them by
`sudo apt install dkms` or `sudo yum install dkms`.

Package name for linux-headers varies on different distributions, e.g.
`linux-headers-generic` (Ubuntu), `linux-headers-amd64` (Debian),
`kernel-devel` (CentOS, Fedora), `kernel-default-devel` (openSUSE).

```bash
# prepare config files
cp redroid.conf /etc/modules-load.d/ # auto load kernel modules after system boot
cp 99-redroid.rules /lib/udev/rules.d/ # change device node permissions

# prepare kernel modules source
cp -rT ashmem /usr/src/redroid-ashmem-1
cp -rT binder /usr/src/redroid-binder-1

# install via dkms
dkms install redroid-ashmem/1
dkms install redroid-binder/1
```

## Docker
```bash
# TODO
```

## K8S
```bash
kubectl apply -f deploy/<YOUR-OS>.yaml
```

## TODO
- package as deb and rpm

