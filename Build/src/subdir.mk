################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../src/GPUSolver.cu \
../src/main.cu 

CPP_SRCS += \
../src/CPUSolver.cpp \
../src/Matrix.cpp \
../src/Solver.cpp \
../src/Vector.cpp \
../src/benchmark.cpp \
../src/tests.cpp 

OBJS += \
./src/CPUSolver.o \
./src/GPUSolver.o \
./src/Matrix.o \
./src/Solver.o \
./src/Vector.o \
./src/benchmark.o \
./src/main.o \
./src/tests.o 

CU_DEPS += \
./src/GPUSolver.d \
./src/main.d 

CPP_DEPS += \
./src/CPUSolver.d \
./src/Matrix.d \
./src/Solver.d \
./src/Vector.d \
./src/benchmark.d \
./src/tests.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -G -g -O0 -std=c++11 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21  -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -G -g -O0 -std=c++11 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -G -g -O0 -std=c++11 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21  -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -G -g -O0 -std=c++11 --compile --relocatable-device-code=false -gencode arch=compute_20,code=compute_20 -gencode arch=compute_20,code=sm_21  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


