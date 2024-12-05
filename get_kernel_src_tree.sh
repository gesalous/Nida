#!/bin/bash

# Function to display usage
usage() {
    echo "Usage: $(basename \$0) [kernel_version]"
    echo "Example: $(basename \$0) 5.14"
    echo "If no version is specified, the current kernel version will be used."
    exit 1
}

# Function to extract kernel version
get_current_kernel_version() {
    local full_version=$(uname -r)
    local major_minor=$(echo "$full_version" | sed -E 's/^([0-9]+\.[0-9]+).*/\1/')
    echo "$major_minor"
}

echo "---> $1"
# Check if help is requested
# Check if help is requested
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    usage
fi

# Use provided kernel version or extract from current kernel
if [ -n "$1" ]; then
    # Validate input format (major.minor)
    if ! [[ $1 =~ ^[0-9]+\.[0-9]+$ ]]; then
        echo "Error: Invalid kernel version format. Please use major.minor format (e.g., 5.14)"
        usage
    fi
    KERNEL_VERSION="$1"
else
    KERNEL_VERSION=$(get_current_kernel_version)
    if [ -z "$KERNEL_VERSION" ]; then
        echo "Error: Could not determine current kernel version"
        exit 1
    fi
    echo "No kernel version specified, using current kernel version: $KERNEL_VERSION"
fi

KERNEL_MAJOR=$(echo "$KERNEL_VERSION" | cut -d. -f1)

echo "Detected kernel version: $(uname -r)"
echo "Downloading Linux ${KERNEL_VERSION}..."

# Download and extract the kernel source
if wget "https://cdn.kernel.org/pub/linux/kernel/v${KERNEL_MAJOR}.x/linux-${KERNEL_VERSION}.tar.xz"; then
    tar xf "linux-${KERNEL_VERSION}.tar.xz" && rm "linux-${KERNEL_VERSION}.tar.xz"
    echo "Done. Headers are in linux-${KERNEL_VERSION}/drivers/nvme/host/"
else
    echo "Error: Failed to download kernel version ${KERNEL_VERSION}"
    exit 1
fi

