# Simple script to run an ELF file on FVP Corstone-300

# Check if ELF file is provided
if [ $# -eq 0 ]; then
    echo "Usage: ./run_fvp.sh <elf_file>"
    echo "Example: ./run_fvp.sh cmsis_nn_example_arm.elf"
    exit 1
fi

ELF_FILE="$1"

# Check if file exists
if [ ! -f "$ELF_FILE" ]; then
    echo "Error: File '$ELF_FILE' not found!"
    exit 1
fi

# Get absolute path
ELF_ABS=$(realpath "$ELF_FILE")
UART_LOG="${ELF_FILE%.elf}_uart.log"

echo "================================================"
echo "Running: $ELF_FILE"
echo "UART output will be saved to: $UART_LOG"
echo "================================================"
echo ""

# Run on FVP with timeout
  ../modules/CMSIS-NN/Tests/UnitTest/downloads/corstone300_download/models/Linux64_armv8l_GCC-9.3/FVP_Corstone_SSE-300_Ethos-U55 \
  -a "cmsis_nn_example.elf" \
  -C mps3_board.visualisation.disable-visualisation=1 \
  -C cpu0.semihosting-enable=0 \
  -C mps3_board.telnetterminal0.start_telnet=0 \
  -C mps3_board.uart0.out_file="$(pwd)/${UART_LOG}" \
  -C mps3_board.uart0.unbuffered_output=1 \
  -C mps3_board.uart0.shutdown_on_eot=1 \
  --cpulimit 2 \
  --simlimit 500 \
  || true

echo ""
echo "================================================"
echo "FVP execution complete!"
echo "Check output: cat $UART_LOG"
echo "================================================"

