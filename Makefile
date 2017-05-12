SDK_DIR ?= sdk

.PHONY: all
all: sdk
	@$(MAKE) -s remote
	@$(MAKE) -s base

.PHONY: all-release
all-release: sdk
	@$(MAKE) -s remote-release
	@$(MAKE) -s base-release

################################################################################
# Remote                                                                       #
################################################################################

.PHONY: remote
remote:
	@$(MAKE) -s debug APP_DIR=remote OUT=firmware-remote

.PHONY: remote-release
remote-release:
	@$(MAKE) -s release APP_DIR=remote OUT=firmware-remote

.PHONY: dfu-remote
dfu-remote:
	@$(MAKE) -s dfu-debug APP_DIR=remote OUT=firmware-remote

.PHONY: dfu-remote-release
dfu-remote-release:
	@$(MAKE) -s dfu-release APP_DIR=remote OUT=firmware-remote

################################################################################
# Base                                                                         #
################################################################################

.PHONY: base
base:
	@$(MAKE) -s debug APP_DIR=base OUT=firmware-base

.PHONY: base-release
base-release:
	@$(MAKE) -s release APP_DIR=base OUT=firmware-base

.PHONY: dfu-base
dfu-base:
	@$(MAKE) -s dfu-debug APP_DIR=base OUT=firmware-base

.PHONY: dfu-base-release
dfu-base-release:
	@$(MAKE) -s dfu-release APP_DIR=base OUT=firmware-base

################################################################################
# Defaul                                                                       #
################################################################################

-include sdk/Makefile.mk

.PHONY: sdk
sdk:
	@if [ ! -f $(SDK_DIR)/Makefile.mk ]; then echo "Initializing Git submodules..."; git submodule update --init; fi

.PHONY: update
update: sdk
	@echo "Updating Git submodules..."; git submodule update --remote --merge
