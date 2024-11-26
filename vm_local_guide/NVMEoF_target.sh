#!/bin/bash
TARGET_NAME=CHATZIS
NAMESPACE=27
NVME_PORT=1
DEVICE_PATH=/dev/loop0
INTERFACE_NAME=eth0

mkdir /sys/kernel/config/nvmet/subsystems/"$TARGET_NAME"

cd /sys/kernel/config/nvmet/subsystems/"$TARGET_NAME"

echo 1 > attr_allow_any_host

mkdir namespaces/"$NAMESPACE"

cd namespaces/"$NAMESPACE"

echo -n /dev/nvme0n1 > "$DEVICE_PATH"

echo 1 > enable

mkdir /sys/kernel/config/nvmet/ports/"$NVME_PORT"

cd /sys/kernel/config/nvmet/ports/"$NVME_PORT"

#ip addr show "$INTERFACE_NAME"

IP_ADDR=$(ip addr show "INTERFACE_NAME" | grep 'inet ' | sed -e 's/^.*inet //' -e 's/\/.*$//')

echo -n "$IP_ADDR" > addr_traddr

echo rdma > addr_trtype

#4420 is the port number the target listens to
echo 4420 > addr_trsvcid

echo ipv4 > addr_adrfam

ln -s /sys/kernel/config/nvmet/subsystems/"$TARGET_NAME"   /sys/kernel/config/nvmet/ports/1/subsystems/"$TARGET_NAME"

#verify
dmesg | grep "enabling port"


