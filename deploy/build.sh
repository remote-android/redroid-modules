#!/bin/bash

grep -q ashmem /proc/misc && grep -q binder /proc/filesystems && { echo "already installed"; exit 0; }

kernel=$(uname -r)

# install required packages
if [ -n "`command -v yum`" ]; then
    yum install -y git kmod make "kernel-devel-uname-r == $kernel"
elif [ -n "`command -v apt-get`" ]; then
    apt-get update && apt-get install -y git kmod make gcc linux-headers-$kernel
else
    echo "install packages FAILED, OS: `uname -a`"
    exit 1
fi

cd /root
# get modules source
if [ -d "redroid-modules" ]; then
    cd redroid-modules && git fetch origin && git reset --hard origin/master && cd -
else
    git clone https://github.com/remote-android/redroid-modules.git
fi

cd redroid-modules && make install && echo "install succeed" || { echo "install FAILED"; exit 1; }
