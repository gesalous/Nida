#!/bin/sh

set -x

virt-install \
	--connect qemu:///session \
	--name fastmap \
	--memory 2048 \
	--disk path=/spare2/jmal/fastmap.qcow2,format=qcow2,size=25 \
	--disk path=/dev/nvme0n1,format=raw,device=disk \
	--rng /dev/random \
	--vcpus 8 \
	--os-type linux \
	--os-variant rhel7 \
	--network bridge=br902,model=virtio \
	--graphics none \
	--console pty,target_type=serial \
	--location 'http://ftp.cc.uoc.gr/mirrors/linux/centos/7/os/x86_64/' \
	--extra-args 'console=ttyS0,115200n8 serial'

