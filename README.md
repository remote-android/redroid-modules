English | [简体中文](./README_zh.md)
# ReDroid Kernel Modules
This repository contains the kernel modules necessary to run *ReDroid* instances.

Currently, only **4.14+** kernel is supported. Many Linux distributions already meet 
this requirement (Ubuntu 16.04+, AmazonLinux 2, CentOS 8+ etc.). If you are using a 
customized kernel, make sure the corresponding kernel headers are present in your system.

Suggest use *non production* machines to try these modules.

## Build & Deploy
- [Manual](#manual)
- [DKMS](#dkms)
- [Package Manager](#package-manager)
- [Docker](#docker)
- [K8S](#k8s)
- [Custom Kernel](#custom-kernel)
- [Kata Runtime / MicroVM](#kata-runtime)

## Manual
```bash
# single command
sudo bash -c "`curl -s https://raw.githubusercontent.com/remote-android/redroid-modules/master/deploy/build.sh`"

# Ubuntu / Debian etc.
sudo apt-get install -y git kmod make gcc linux-headers-`uname -r`
sudo make # build kernel modules
sudo make install # build and install *unsigned* kernel modules

# AmazonLinux2 / CentOS / RHEL
sudo yum install git kmod make "kernel-devel-uname-r == `uname -r`"
sudo make # build kernel modules
sudo make install # build and install *unsigned* kernel modules

# check modules status
lsmod | grep -e ashmem_linux -e binder_linux
# example output:
# binder_linux          147456  79
# ashmem_linux           16384  23

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
`linux-headers-generic` (Ubuntu), `kernel-devel` (CentOS, AmazonLinux2)

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

## Package Manager
TODO, stay tuned :)

## Docker
```bash
docker run --rm --cap-add CAP_SYS_MODULE --entrypoint /bin/bash NODE_OS -c "`curl -s https://raw.githubusercontent.com/remote-android/redroid-modules/master/deploy/build.sh`"
# for example, if you are running under ubuntu 18.04, please change NODE_OS to *ubunut:18.04*
```

## K8S
create overlay of your node OS, see `deploy/k8s/overlays/ubuntu1804` as an example.
then run `kubectl apply -k <YOUR_OVERLAY_PATH>`

## Custom Kernel
If use custom kernel (5.0+), you can enable `binderfs` and `ashmem` configs; So the kernel modules in this repo 
are not needed any more.

## Kata Runtime
Enable `binderfs` and `ashmem` in 5.0+ kernel.

Check out https://github.com/kata-containers/packaging/tree/master/kernel

