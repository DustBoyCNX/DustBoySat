#!/bin/bash

echo "hello"
# build using pio
# pio run -t compile

# propt release file name with prefix 
# pio run -t compile | grep "firmware.bin" | awk '{print $2}'