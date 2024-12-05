#!/bin/bash

# Extract major.minor (5.14) from full kernel version (5.14.0-284.11.1.el9_2.x86_64)
KERNEL_VERSION=$(uname -r | grep -o '^[0-9]\+\.[0-9]\+')
KERNEL_MAJOR=$(echo "$KERNEL_VERSION" | cut -d. -f1)

echo "Detected kernel version: $(uname -r)"
echo "Downloading Linux ${KERNEL_VERSION}..."

wget "https://cdn.kernel.org/pub/linux/kernel/v${KERNEL_MAJOR}.x/linux-${KERNEL_VERSION}.tar.xz" \
    && tar xf "linux-${KERNEL_VERSION}.tar.xz" \
    && rm "linux-${KERNEL_VERSION}.tar.xz"

echo "Done. Headers are in linux-${KERNEL_VERSION}/drivers/nvme/host/"

