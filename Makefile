SDK_DIR ?= sdk

.PHONY: all
all: sdk
	@$(MAKE) -s remote
	@$(MAKE) -s base

.PHONY: remote
remote:
	@$(MAKE) -s debug APP_DIR=app_remote OUT=firmware-remote

.PHONY: base
base:
	@$(MAKE) -s debug APP_DIR=app_base OUT=firmware-base

-include sdk/Makefile.mk

.PHONY: sdk
sdk:
	@if [ ! -f $(SDK_DIR)/Makefile.mk ]; then echo "Initializing Git submodules..."; git submodule update --init; fi

.PHONY: update
update: sdk
	@echo "Updating Git submodules..."; git submodule update --remote --merge
