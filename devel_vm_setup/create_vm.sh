#!/bin/bash
virsh create --file - <<EOF
<domain type='kvm'>
  <name>ubuntu-kernel-hacking</name>
  <memory unit='KiB'>4096000</memory> <!-- 4 GB RAM -->
  <vcpu placement='static'>2</vcpu> <!-- 2 CPUs -->
  <os>
    <type arch='x86_64' machine='pc-i440fx-5.2'>hvm</type>
    <boot dev='cdrom'/>
    <boot dev='hd'/>
  </os>
  <devices>
    <disk type='file' device='disk'>
      <driver name='qemu' type='qcow2'/>
      <source file='/var/lib/libvirt/images/ubuntu-kernel-hacking.qcow2'/>
      <target dev='vda' bus='virtio'/>
      <address type='pci' domain='0x0000' bus='0x00' slot='0x04' function='0x0'/>
    </disk>
    <disk type='file' device='cdrom'>
      <driver name='qemu' type='raw'/>
      <source file='/home/gesalous/Downloads/ubuntu-24.04.1-live-server-amd64.iso'/>
      <target dev='hda' bus='ide'/>
      <address type='pci' domain='0x0000' bus='0x00' slot='0x03' function='0x0'/>
    </disk>
    <interface type='network'>
      <mac address='52:54:00:XX:XX:XX'/>
      <source network='default'/>
      <model type='virtio'/>
    </interface>
  </devices>
</domain>
EOF
