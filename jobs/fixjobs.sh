#!/bin/bash
for i in *.gcode
do
	sed -i 's/M3 S100.0/M3 P100.0/;s/^(/;(/' $i
done
sed -i 's/G0 Z26.000/M5/;s/G0 Z24.000/M3 P100/;s/Z21.000//' engraving.gcode
