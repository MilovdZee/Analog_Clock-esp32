#!/bin/bash

# Configuration
PASSWORD="RoundClockPassword"
LOCAL_DATA="/tmp/arduino_clock_cli"
BINARY_PATH="./build_cache/Analog_Clock-esp32.ino.bin"
OTA_TOOL="$LOCAL_DATA/packages/esp32/hardware/esp32/2.0.17/tools/espota.py"

# Help function
show_help() {
    echo "Usage: $0 <IP_ADDRESS>"
    echo ""
    echo "Example:"
    echo "  $0 192.168.1.50"
    echo ""
    echo "Note: Ensure you have run ./compile.sh first to generate the binary."
}

# Check if IP address is provided
if [[ -z "$1" || "$1" == "-h" || "$1" == "--help" ]]; then
    show_help
    exit 1
fi

IP_ADDRESS=$1

# Check if binary exists
if [[ ! -f "$BINARY_PATH" ]]; then
    echo "Error: Binary not found at $BINARY_PATH"
    echo "Please run ./compile.sh before attempting OTA upload."
    exit 1
fi

# Check if OTA tool exists
if [[ ! -f "$OTA_TOOL" ]]; then
    echo "Error: OTA tool not found at $OTA_TOOL"
    echo "Is the private core 2.0.17 installed?"
    exit 1
fi

echo "Initiating OTA upload to $IP_ADDRESS..."
python3 "$OTA_TOOL" -i "$IP_ADDRESS" -p 3232 -a "$PASSWORD" -f "$BINARY_PATH"

