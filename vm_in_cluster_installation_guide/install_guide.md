Building a VM to test FastMap
=============================
This is an Arch Linux based guide on how to set up a CentOS 7 VM with a 4.14 kernel
in order to test FastMap. Commands denoted with *$* require root permissions.

Libvirt Setup
-------------
Firstly install libvirt, QEMU, and virt-install along with their dependencies,
and some auxiliary networking packages:

`$ pacman -Sy libvirt qemu virt-install iptables-nft dnsmasq bridge-utils`

Start libvirt by starting/enabling `libvirt.service`.

### Configuring Libvirt
The libvirt KVM/QEMU driver has multiple instances to choose from, denoted by
connection URIs. The ones that concern us are:

1. **qemu:///system**
   System wide driver instance, offers a *default* network using bridged NAT, therefore
   does not require much network setup. Requires user authentication to connect to this
   instance, either via polkit agent (which requires entering the user password on
   each connection), or by adding the user to the *libvirt* group (no password required).

2. **qemu:///session**
   Per user session instance, does not require authentication, but requires manually
   setting up a bridge for networking, else the only option offered is **slow** SLIRP.

For more information on the KVM/QEMU hypervisor driver check [this](https://libvirt.org/drvqemu.html).

Installing the VM
-----------------
We will use `virt-install` to install a VM with the features we want. The following
virt-install command covers everything we need, and can be tweaked to preference.

```sh
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
```

Let's go over some of these options:

- `--connect` specifies which driver instance to connect to

- `--name` specifies a *domain name* for the virtual machine, libvirt treats
  VMs as domains

- `--disk` specifies both a new disk to contain the VM image 
  (*fastmap.qcow2*, size 25GB), as well as an existing NVMe disk
  to attach to the VM

- `--rng` specifies the host */dev/random* device to be used to feed random numbers
  to the guest, it is specified here to avoid some GSSAPI related delay at boot time.

- `--network` does what is says, here *br902* is a preconfigured bridge. If using
  the `qemu:///system` driver, one may instead write `--network default` with no
  prior configuration being necessary.

- The `console`, `location`, `graphics`, and `extra-args` options specify how to use
  a console to install CentOS 7 from the university fileserver in text mode.

Upon running the command, the CentOS 7 text mode installer is pretty easy to follow
and the whole installation process should not take more than 10 minutes.

Once the installation is complete, the VM will reboot and prompt you to login.
Login using the root account and install the necessary packages from the
[included file](./packages) by running:

`$ xargs yum -y install < packages`

Also install the development tools group:

`$ yum group install -y "development tools"`

Once everything is installed, power off the VM and the installation process is done.

Running & Configuring the VM
----------------------------

### virsh
`virsh` is a console utility used to manage libvirt virtual machines. It is
installed with the libvirt package. Virsh commands generally follow the template:

`virsh` *command* *domain* *additional-parameters*

Virsh supports lots of commands, which can be viewed with `virsh help`, or per-command
with `virsh` *command* `--help`. Some of the most commonly used commands are:

- *start*
  Start a domain

- *shutdown/reboot/destroy*
  Shutdown/reboot/force shutdown a domain

- *list*
  View active domains. Use with `--all` to view all domains, running or not

- *console*
  Connect to the serial console of a domain. Useful for kernel debugging.

### Connecting to the VM
To connect to the VM, you can either start the VM with virsh, connect to its console
and check its assigned IP in order to SSH, or if you have root privileges, you can
edit the host `/etc/nsswitch.conf` file in the following way:

`hosts: files libvirt_guest dns myhostname`

[Name Service Switch](https://en.wikipedia.org/wiki/Name_Service_Switch) is a tool
which allows for modular name resolution mechanisms in a plug-and-play manner. The
above file edit instructs NSS to use the `libvirt-guest` module when failing to lookup
a host using the local system host files. Thus, we can SSH to the VM by its hostname.

### Guest Configuration
Having installed the necessary packages, it is now time to build the kernel for
FastMap. If you are using a stable version which does not require 
kernel modifications you can grab and install any **4.14** kernel as per the usual 
process. If instead working with an unstable version with kernel modification follow
the kernel instruction on the [FastMap Github](https://github.com/CARV-ICS-FORTH/FastMap).

When configuring the kernel you need to make sure NVMe support is enabled 
(if using an NVMe device), as well as the null block device module 
(CONFIG\_BLK\_DEV\_NULL) for testing. Also enable huge pages 
(CONFIG\_TRANSPARENT\_HUGEPAGES) if planning to run huge page setups.

#### KASAN
If you want to perform memory debugging, it is a good idea to enable the
[Kernel Address SANitiser](https://www.kernel.org/doc/html/latest/dev-tools/kasan.html).
The configuration option is CONFIG\_KASAN. To compile the kernel with KASAN support
you will need gcc version >8.3.0, which is much younger than what CentOS 7 offers
by default. 

This is why we installed `devtoolset-8-gcc` as a package dependency.
By running `source /opt/rh/devtoolset-8/enable` the running shell environment is
modified and running `gcc --version` will yield a newer version. This command
must also be run before building FastMap as a regular user, so including it
in the *.bashrc* file is a good idea.

Build and install the kernel as usual.

Troubleshooting
---------------

### QCOW2 image running out of space
The specified 25GB for the virtual machine image should be enough for our needs but
you may find yourself running out of space on your root partition. Not to worry, this
is why we installed `gdisk` along with `growpart` beforehand. Allotting a few more
gigabytes is a simple 3 step process.

1. From the host, run `virsh blockresize` *domain* *<vm qcow image location>* *<new_size>*, e.g., `virsh blockresize fastmap /spare2/jmal/fastmap.qcow2 24G`.
   This can even be done while the virtual machine is running. This command essentially
   runs `qemu-img resize` on your vm image.

2. Inside the guest, run `$ growpart <device path> <partition number>`, for instance, if
   your root partition is `/dev/vda2`, you would run `$ growpart /dev/vda 2`. You can
   check partitions and mount points by running `lsblk`.

3. Now that the root partition has been extended with the added space from the 
   blockresize command, the final step is to grow the root filesystem, in order for
   the added space to be visible. The command to run here depends on the type of the
   root filesystem, but for the most common cases, XFS and ext4, the commands would
   be `$ xfs_growfs /` and `resize2fs <root partition path>`.

