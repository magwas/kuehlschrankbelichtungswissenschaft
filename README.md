# Kühlschrankbelichtungswissenschaft

Kühlschrankbelichtungswissenschaft means "fridge lighting science".
It is an open hardware, open source steampunk control unit for agriculture projects, with two set of the following features:

- controlled 220V mains plug for a lamp
- humidity sensor as a 3.5mm audio socket
- CO2 sensor
- two temperature sensors
- controlled 12V barrel outlets for ventillation, cooling, nutrient and heating

The CO2 and temperature sensors are connected using RJ11 connectors with the following pinout:

1. GND (green)
2. 3V3 (green-white)
3. SDA (brown)
4. SCL (brown-white)
5. 5V (red)
6. OneWire (red-white)

The ground of the connectors for RJ11 is controlled, as the CO2 sensor used have a fix I2C address.

The circuit design is in the conteroller directory.

The schematics layout looks as it is to make it easier to implement it as a freeform circuit.
This is the reason there is no PCB design. Of course you are welcome to build it on a PCB.
A two-sided PCB should be adequate for the job, there is nothing placement-critical there.

I did not put the USB-c cable between the board and the connector on the chassis on the circuit design,
simply because the symbol I use does not contain it.


## SOFTWARE INSTALLATION

The software uses ESP IDF, and the msglib library.
Install ESP IDF as documented at https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/
Clone msglib to a convenient place, like
```
mkdir ~/build
cd ~/build
git clone https://github.com/magwas/msglib
```

to build, flash and monitor the software, plug in the board to the USB, export MESSAGING_LIB with the location where you cloned it, activate ESP IDF, and run idf.py build, something like

```
export MESSAGING_LIB=$HOME/build/msglib
. ~/.espressif/tools/activate_idf_v5.5.2.sh ~/idf-esp-activate
idf.py -p /dev/ttyACM0 build flash monitor
```

## HARDWARE FABRICATION

The mechanical design is in the FreeCad model box.CFStd. I have a Snapmaker 2, so the CNC jobs use the marlin preprocessor.

The fixjobs.sh script

- fixes the output of the preprocessor (marlin emits M3 S100, snapmaker understands M3 P100, and the comments are in different form)
- removes all Z movement from the laser engraving jobs, replacing some of the to gcode turning the laser off and on again.

IMPORTANT: the engraving job fix translates movement to Z=26 (G0 Z26.000) to turning the laser off, and Z=21 (G0 Z21.000) to turning it on.
Make sure that the heights and depths of the engraving job correspont to these if you modify it.

postprocess all of the CNC jobs. The "Stock Prepare" job is for preparation of the stock on my own machine, you might of course choose other ways to fix your stock.
run fixjobs.sh or something equivalent for your setup.
the file cnc-workflow contains the instructions to do the milling and engraving. It contains the sizes of the stocks. I recommend to use some hardwood. I used white acacia, as I love that wood. You will need a 128x86 and 246x86 mm sheet of 4mm tempered glass.
Populate the dashboard with components. I try to recall here what I was using. Be aware that they are mostly cheap noname parts. Which means you can probably buy ones with similar sizes.

- For the mains plugs, I cannibalized some LEGRAND SUNO plugs, by removing everything above the bottom plastic and metal parts, and trimming down the plastic parts, keeping the tabs which open the electrodes for the assembly.
- The main switch is a 22x30 standard switch.
- The 12V barrel connectors I obtained are for 12mm diameter fine thread. I did not want to buy a tap for that, instead I made the holes the size such that they fit snuggly.
- The USB-C connector is a noname one which fits into the 3.5x10mm hole and its length is 15mm. I could not find a datasheet for a similar one.
- The connectors for the humidity sensors are standard 3.5mm audio jacks. They are actually the tye of swinging connector wher the round connector part is fixed to the anti-fracture housing with a thread. I use just the inner part.
- The design now have just a simple hole for the wifi antenna, as I am currently waiting for the board wersion which comes with a connector for it.
- The RJ11 connectors are the plain usual 90 degree ones.

All other components (and most of the aboveones ) are in the BOM of the circuit.

