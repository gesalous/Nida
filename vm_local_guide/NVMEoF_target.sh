#!/bin/bash

# Check if all required parameters are provided
if [ $# -ne 3 ]; then
  echo "Usage: \$0 <target_name> <interface_name> <backing_file>"
  echo "Example: \$0 nqn.2024-01.com.example:target1 ens33 /path/to/backing.img"
  exit 1
fi

# Assign command line parameters to variables
TARGET_NAME="$1"
INTERFACE_NAME="$2"
BACKING_FILE="$3"

# Create backing file if it doesn't exist
if [ ! -f "$BACKING_FILE" ]; then
  echo "Backing file '$BACKING_FILE' does not exist. Creating 1GB backing file..."
  dd if=/dev/zero of="$BACKING_FILE" bs=1M count=1024 status=progress
  if [ $? -ne 0 ]; then
      echo "Error: Failed to create backing file"
      exit 1
  fi
  echo "Backing file created successfully"
fi

# Constants
NAMESPACE=27
NVME_PORT=4420
DEVICE_PATH=/dev/loop0

# Load required modules
sudo modprobe loop
sudo modprobe rdma_rxe
sudo modprobe nvmet
sudo modprobe nvme
sudo modprobe nvme_fabrics

echo "Loaded all modules"

echo "Creating SoftRoce..."
rdma link add rxe_0 type rxe netdev "$INTERFACE_NAME"
ibv_devinfo

echo "Creating SoftRoce success"

# Setup loop device
losetup /dev/loop0 "$BACKING_FILE"

# Create nvmet subsystem
mkdir -p /sys/kernel/config/nvmet/subsystems/"$TARGET_NAME"

cd /sys/kernel/config/nvmet/subsystems/"$TARGET_NAME"

echo 1 > attr_allow_any_host

mkdir -p namespaces/"$NAMESPACE"

cd namespaces/"$NAMESPACE"

echo -n "$DEVICE_PATH" > device_path

echo 1 > enable

# Setup port
mkdir -p /sys/kernel/config/nvmet/ports/"$NAMESPACE"

cd /sys/kernel/config/nvmet/ports/"$NAMESPACE"

# Get IP address
IP_ADDR=$(ip addr show "$INTERFACE_NAME" | grep 'inet ' | sed -e 's/^.*inet //' -e 's/\/.*$//')

if [ -z "$IP_ADDR" ]; then
  echo "Error: Could not get IP address for interface $INTERFACE_NAME"
  exit 1
fi

echo -n "$IP_ADDR" > addr_traddr
echo rdma > addr_trtype
echo "$NVME_PORT" > addr_trsvcid
echo ipv4 > addr_adrfam

# Create symbolic link
ln -s /sys/kernel/config/nvmet/subsystems/"$TARGET_NAME" /sys/kernel/config/nvmet/ports/"$NAMESPACE"/subsystems/"$TARGET_NAME"

# Verify
echo "Checking dmesg for port enabling..."
dmesg | grep "enabling port"

echo "Setup completed. Target configuration:"
echo "Target Name: $TARGET_NAME"
echo "Interface: $INTERFACE_NAME"
echo "IP Address: $IP_ADDR"
echo "Backing File: $BACKING_FILE"

