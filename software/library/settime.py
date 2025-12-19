#!/usr/bin/env python3

import sys
import time
import serial
from datetime import datetime
import pytz

def read_for_a_while(ser):
    start_time = time.time()
    while time.time() - start_time < 1.0:
        if ser.in_waiting:
            line = ser.readline().decode('utf-8', errors='ignore').rstrip()
            if line:
                print(line)
        time.sleep(0.01)

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <serial_port> [cmd]", file=sys.stderr)
        sys.exit(1)

    cmd = 1
    port = sys.argv[1]
    if len(sys.argv) == 3:
        cmd = sys.argv[2]
    
    try:
        with serial.Serial(port=port,
                           baudrate=9600) as ser:
            unix_time = int(time.time() + time.localtime().tm_gmtoff)
            clock_command = f"clock {cmd} {unix_time}\r\n"
            ser.write(clock_command.encode('utf-8'))
            ser.flush()
            clock_command = f"clock 0 0\r\n"
            ser.write(clock_command.encode('utf-8'))
            ser.flush()
            read_for_a_while(ser)

    except serial.SerialException as e:
        print(f"Error opening serial port {port}: {e}", file=sys.stderr)
        sys.exit(1)
    except KeyboardInterrupt:
        print("\nInterrupted by user", file=sys.stderr)
        sys.exit(0)

if __name__ == "__main__":
    main()
