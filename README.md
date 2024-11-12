# Nida

Nida is an NVMe-oF (NVMe over Fabrics) initiator project.

## Development Setup

To generate `compile_commands.json`, run:

make clean && bear -- make

mv compile_commands.json build/

## Folder Structure

- **src/**: Contains the code for the NVMe-oF initiator. The code is based on the `nvme_rdma.c` initiator code from the Linux kernel 6.8.0 for InfiniBand.

- **Nida_daemon/**: Contains a userspace daemon that the NVMe-oF initiator communicates with via a Unix socket. Its purpose is solely for development until hardware and kernel APIs are available. The Nida daemon runs the Portails library and acts as a proxy, as porting the userspace Portails library directly into the kernel would require significant effort.

## Loading the Module

To load the needed module, run the `module_load` bash script:

bash
./module_load


## Development Notes

For development purposes, Nida uses userspace Nida daemon. Nida daemon a) runs
userspace portails library and b) acts as proxy for the kernel module to
perform portials API calls. We do this for rapid development until hardware and
kernel libraries are ready. If we port portails in kernel it would require
significant effort.

