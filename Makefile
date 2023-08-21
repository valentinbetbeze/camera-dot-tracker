.PHONY: all build cmake clean format flash

BUILD_DIR := build
BUILD_TYPE ?= Debug
GENERATOR := "MinGW Makefiles"
GENERATOR_PATH := "C:/Program Files (x86)/GnuWin32/bin/make"

all: build

${BUILD_DIR}/Makefile:
	cmake \
		-B${BUILD_DIR} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DCMAKE_MAKE_PROGRAM:PATH=${GENERATOR_PATH} \
		-G${GENERATOR}

cmake: ${BUILD_DIR}/Makefile

build: cmake
	${GENERATOR_PATH} -C ${BUILD_DIR} --no-print-directory -j8

clean:
	rm -rf $(BUILD_DIR)

flash: all
	st-flash --reset write $(BUILD_DIR)/*.bin 0x8000000
