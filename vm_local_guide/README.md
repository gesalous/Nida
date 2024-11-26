# Instructions
The instructions assume ubuntu 24.04
First, you need to have in the vm two basic ingredients 
a) RDMA (infiniband, RoCE, BXI) and b) NVMe device


## Install softRoCE

Load required kernel modules

`sudo modprobe rdma_rxe`

In the case where a) is not present install softRoCE package 
`sudo apt-get install rdma-core perftest`

`sudo rdma link add rxe_0 type rxe netdev <network adapter name e.g. lo, ens0>`

Verify that everything is ok
`rdma link show`

### Install infiniband tools

`sudo apt install -y libibverbs1 libibverbs-dev ibverbs-utils rdma-core perftest infiniband-diags`

### Test

Server: `ib_write_bw -d rxe_0 --qp 10 --mtu 1024 --retry_count 7`

Client: `ib_write_bw -d rxe_0 -a 192.168.122.220 --qp 10 --mtu 1024 --retry_count 7`

## Setup an NVMEoF target 
If there is a spare NVME device you can ommit the following step

Create a file with dd e.g.
`dd if=/dev/zero of=<file_name> bs=1G count=1`

Check and Load the required kernel modules
`lsmod | grep loop`

If not already loaded
`sudo modprobe loop`

Then
`sudo losetup /dev/loop0 /path/to/backing_file`

## Load required modules for NVMEoF
`sudo modprobe nvmet`
`sudo modprobe nvme`
`sudo modprobe nvme_fabrics`


## Create NVMeOF target via configfs
Script is at NVMEoF_target.sh 

## Connect NVMEoF initiator to the target
# Connect to the NVMe target
nvme connect -t rdma -n <TARGET_NAME> -a <IP address> -s <NVME port>



