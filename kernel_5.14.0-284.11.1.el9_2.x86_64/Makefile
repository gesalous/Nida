# Directory definitions
BUILD_DIR := $(PWD)/build
SRC_DIR := $(PWD)/src
KERNEL_SRC := /home/carv/rpmbuild/SOURCES/linux-5.14.0-284.11.1.el9_2
# Additional Module.symvers locations  
PORTAL_SYMVERS := /usr/src/bxi3-portals/Module.symvers  
BXI_MODULE_SYMVERS := /usr/src/bxi3-module/Module.symvers  

# Kernel directory
KDIR := /lib/modules/$(shell uname -r)/build

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))

# Main targets
default: prepare merge_symvers
	cp $(PWD)/Kbuild $(BUILD_DIR)/Makefile
	$(MAKE) -C $(KDIR) \
		M=$(BUILD_DIR) \
		src=$(BUILD_DIR) \
		KBUILD_EXTRA_SYMBOLS="$(PORTAL_SYMVERS) $(BXI_MODULE_SYMVERS)" \
		EXTRA_CFLAGS="-I$(KERNEL_SRC)/drivers/nvme/host" \
		modules

clean:
	$(MAKE) -C $(KDIR) M=$(BUILD_DIR) clean
	rm -rf $(BUILD_DIR)

# Prepare build directory by copying source files
prepare:
	cp $(SRC_DIR)/*.c $(BUILD_DIR)/
	cp $(SRC_DIR)/*.h $(BUILD_DIR)/ 2>/dev/null || true
merge_symvers:
	cat $(PORTAL_SYMVERS) $(BXI_MODULE_SYMVERS) > $(BUILD_DIR)/Module.symvers  

