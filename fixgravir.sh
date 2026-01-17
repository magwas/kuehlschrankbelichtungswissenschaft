#!/bin/bash
cat gravir.gcode |sed 's/G0 Z6.000/M5/;s/G0 Z4.000/M3 P100/' >gravir-fixed.gcode
