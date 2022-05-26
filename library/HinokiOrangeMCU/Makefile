export BUILD_CHECKER=$(abspath CMakeLists.txt) $(abspath Makefile)

all: 
	@$(MAKE) -s default

default: @clang.example.linux

@clang%: export CC=$(shell which clang)
@clang%:
	@$(MAKE) -s $*

.example%:
	@$(MAKE) -s -C example $*
