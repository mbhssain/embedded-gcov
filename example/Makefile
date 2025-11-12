# Paths
CMSIS_NN_PATH = ../modules/CMSIS-NN
CMSIS_NN_INC = -I$(CMSIS_NN_PATH)/Include
CMSIS_NN_SRC = $(CMSIS_NN_PATH)/Source

# CMSIS-5 paths (following CMSIS-NN unit test pattern)
CMSIS5_PATH = ../CMSIS_5
CMSIS5_CORE_INC = -I$(CMSIS5_PATH)/CMSIS/Core/Include

TARGET_CPU = cortex-m55
ARM_CPU = ARMCM55
ARM_FEATURES =

# CMSIS-5 Device paths for target CPU
CMSIS5_DEVICE_PATH = $(CMSIS5_PATH)/Device/ARM/$(ARM_CPU)
CMSIS5_DEVICE_INC = -I$(CMSIS5_DEVICE_PATH)/Include
CMSIS5_STARTUP = $(CMSIS5_DEVICE_PATH)/Source/startup_$(ARM_CPU).c
CMSIS5_SYSTEM = $(CMSIS5_DEVICE_PATH)/Source/system_$(ARM_CPU).c

# Linker script options
CMSIS5_LINKER = $(CMSIS5_DEVICE_PATH)/Source/GCC/gcc_arm.ld
CORSTONE300_PATH = ../../cmsisnn-demo/Corstone-300
CORSTONE300_INC = -I$(CORSTONE300_PATH)
CORSTONE300_LINKER = $(CORSTONE300_PATH)/linker.ld
CORSTONE300_UART = $(CORSTONE300_PATH)/uart.c
CORSTONE300_RETARGET = $(CORSTONE300_PATH)/retarget.c

# Compiler flags
CFLAGS = -Wall -O0 -fprofile-arcs -ftest-coverage
GCOV_SRC = ../code/gcov_public.c ../code/gcov_gcc.c ../code/gcov_printf.c

# Cross-compilation settings
ARM_CC = arm-none-eabi-gcc
ARM_CFLAGS = -mcpu=cortex-m55 -mthumb \
             -march=armv8.1-m.main+mve.fp+fp.dp \
             -mfloat-abi=hard \
             -specs=nano.specs -specs=nosys.specs \
             -DARM_MATH_MVEI=1 \
             -Wall -O0 -fprofile-arcs -ftest-coverage
SYSCALLS_STUB = syscalls_stub.c
all: example cmsis_nn_example_arm

example:
	gcc $(CFLAGS) -o example example.c $(GCOV_SRC)
	cp example-*.gcno ../objs/ 2>/dev/null || true
	./example > ./example_log.txt

cmsis_nn_example:
	gcc $(CFLAGS) $(CMSIS_NN_INC) -o cmsis_nn_example cmsis_nn_example.c \
		$(CMSIS_NN_SRC)/PoolingFunctions/arm_max_pool_s8.c \
		$(GCOV_SRC)
	cp cmsis_nn_example-*.gcno ../objs/ 2>/dev/null || true
	./cmsis_nn_example > ./cmsis_nn_example_log.txt

cmsis_nn_example_arm:
	$(ARM_CC) $(ARM_CFLAGS) $(CMSIS_NN_INC) -o cmsis_nn_example_arm.elf cmsis_nn_example.c \
		$(CMSIS_NN_SRC)/PoolingFunctions/arm_max_pool_s8.c \
		$(SYSCALLS_STUB) \
		$(GCOV_SRC)
	@echo ""
	@echo "========================================"
	@echo "ARM binary created: cmsis_nn_example_arm.elf"
	@echo "This needs to run on FVP or hardware"
	@echo "To run: make run-cmsis_nn_arm"
	@echo "========================================"

cmsis_nn_example_arm_no_cov:
	$(ARM_CC) -mcpu=$(TARGET_CPU) -mthumb \
		-march=armv8.1-m.main+mve.fp+fp.dp \
		-mfloat-abi=hard \
		-specs=nano.specs -specs=nosys.specs \
		-Wall -Ofast \
		-fomit-frame-pointer \
		-include$(ARM_CPU)$(ARM_FEATURES).h \
		-D$(ARM_CPU)$(ARM_FEATURES) \
		$(CMSIS_NN_INC) \
		$(CMSIS5_CORE_INC) \
		$(CMSIS5_DEVICE_INC) \
		-T $(CMSIS5_LINKER) \
		-Wl,-Map=cmsis_nn_example_arm_no_cov.map \
		-o cmsis_nn_example_arm_no_cov.elf \
		$(CMSIS5_STARTUP) \
		$(CMSIS5_SYSTEM) \
		syscalls_stub.c \
		cmsis_nn_example.c \
		$(CMSIS_NN_SRC)/PoolingFunctions/arm_max_pool_s8.c
	@echo ""
	@echo "========================================"
	@echo "ARM binary (NO COVERAGE) created: cmsis_nn_example_arm_no_cov.elf"
	@echo "Target: $(TARGET_CPU) ($(ARM_CPU)$(ARM_FEATURES))"
	@echo "Using CMSIS-5 startup, system & linker"
	@echo "Optimization: -Ofast with MVE enabled"
	@echo "Note: Using generic memory layout (not Corstone-300 specific)"
	@echo "Size:"
	@arm-none-eabi-size cmsis_nn_example_arm_no_cov.elf
	@echo "To run: ./run_fvp.sh cmsis_nn_example_arm_no_cov.elf"
	@echo "========================================"

run-cmsis_nn_arm_no_cov:
	@echo "Running CMSIS-NN example on ARM (NO COVERAGE)..."
	@FVP_Corstone_SSE-300_Ethos-U55 \
	-a cmsis_nn_example_arm_no_cov.elf \
	-C mps3_board.visualisation.disable-visualisation=1 \
	-C mps3_board.telnetterminal0.start_telnet=0 \
	-C mps3_board.uart0.out_file="-" \
	-C mps3_board.uart0.unbuffered_output=1 \
	-C mps3_board.uart0.shutdown_on_eot=1 \
	--simlimit 0
	@echo "========================================"
	@echo "CMSIS-NN example run complete (NO COVERAGE)"
	@echo "========================================"

run-cmsis_nn_arm:
	@echo "Running CMSIS-NN example on ARM..."
	@FVP_Corstone_SSE-300_Ethos-U55 \
	-a cmsis_nn_example_arm.elf \
	-C mps3_board.visualisation.disable-visualisation=1 \
	-C mps3_board.telnetterminal0.start_telnet=0 \
	-C mps3_board.uart0.out_file="-" \
	-C mps3_board.uart0.unbuffered_output=1 \
	-C mps3_board.uart0.shutdown_on_eot=1 \
	--simlimit 0
	
	@echo "========================================"
	@echo "CMSIS-NN example run complete"
	@echo "========================================"
	
example-coverage-report:
	@echo "=== Coverage Report for example ==="
	@gcov example-example.gcda 2>/dev/null || echo "Run 'make example' first"
	@echo ""

coverage-report:
	@echo "=== Coverage Report for cmsis_nn_example ==="
	@gcov cmsis_nn_example-cmsis_nn_example.gcda 2>/dev/null || echo "Run 'make cmsis_nn_example' first"
	@echo ""

arm-coverage-report:
	@echo "=== Coverage Report for cmsis_nn_example ==="
	@arm-none-eabi-gcov cmsis_nn_example-cmsis_nn_example.gcda 2>/dev/null || echo "Run 'make cmsis_nn_example' first"
	@echo ""
	# @echo "=== Coverage Report for example ==="
	# @gcov example-example.gcda 2>/dev/null || echo "Run 'make example' first"

html:
	@echo "Generating HTML coverage reports..."
	@mkdir -p coverage_html
	@lcov --capture --directory . --output-file coverage_html/coverage.info --branch-coverage --ignore-errors deprecated 2>/dev/null || \
		(echo "Error: lcov not found. Install with: brew install lcov (macOS) or apt-get install lcov (Linux)" && exit 1)
	@lcov --remove coverage_html/coverage.info '*/gcov_*' --output-file coverage_html/coverage_filtered.info --branch-coverage --ignore-errors unused,deprecated
	@genhtml coverage_html/coverage_filtered.info --output-directory coverage_html --title "CMSIS-NN Coverage Report" --legend --show-details --branch-coverage --ignore-errors deprecated
	@echo ""
	@echo "========================================"
	@echo "HTML coverage report generated!"
	@echo "Location: coverage_html/index.html"
	@echo "========================================"
	@echo "To view: open coverage_html/index.html"

clean:
	rm -f example cmsis_nn_example cmsis_nn_example_arm.elf cmsis_nn_example_arm_no_cov.elf *.gcda *.gcno *.log.txt *.log *.gcov *.map ../objs/*.gcno
	rm -rf coverage_html

.PHONY: all example cmsis_nn_example cmsis_nn_example_arm cmsis_nn_example_arm_no_cov coverage-report arm-coverage-report html clean run-cmsis_nn_arm run-cmsis_nn_arm_no_cov

