.PHONY: all build cmake clean format flash

BUILD_DIR := build
BUILD_TYPE ?= Debug
GENERATOR := "MinGW Makefiles"

all: build

${BUILD_DIR}/Makefile:
	cmake \
		-B${BUILD_DIR} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-G${GENERATOR}

cmake: ${BUILD_DIR}/Makefile

build: cmake
	$(MAKE) -C ${BUILD_DIR} --no-print-directory -j8

SRCS := $(shell find . -name '*.[ch]' -or -name '*.[ch]pp')
format: $(addsuffix .format,${SRCS})

%.format: %
	clang-format -i $<

clean:
	rm -rf $(BUILD_DIR)

flash: all
	st-flash --reset write $(BUILD_DIR)/*.bin 0x8000000
