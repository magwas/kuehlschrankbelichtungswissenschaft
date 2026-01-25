#!/bin/bash
cat gravir.gcode |sed 's/G0 Z26.000/M5/;s/G0 Z24.000/M3 P100/;s/Z21.000//' >gravir-fixed.gcode
