# Directory definitions
BUILD_DIR := $(PWD)/build
SRC_DIR := $(PWD)/src
KERNEL_SRC := /usr/src/linux-source-6.8.0/linux-source-6.8.0

# Kernel directory
KDIR := /lib/modules/$(shell uname -r)/build

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))

# Main targets
default: prepare
	cp $(PWD)/Kbuild $(BUILD_DIR)/Makefile
	$(MAKE) -C $(KDIR) \
		M=$(BUILD_DIR) \
		src=$(BUILD_DIR) \
		EXTRA_CFLAGS="-I$(KERNEL_SRC)/drivers/nvme/host" \
		modules

clean:
	$(MAKE) -C $(KDIR) M=$(BUILD_DIR) clean
	rm -rf $(BUILD_DIR)

# Prepare build directory by copying source files
prepare:
	cp $(SRC_DIR)/*.c $(BUILD_DIR)/
	cp $(SRC_DIR)/*.h $(BUILD_DIR)/ 2>/dev/null || true
