# Resin exchange controller V1.0 
by Lucas Schmirl, FH Technikum-Wien

---

## This script controls an arduino Nano to be used with the DLP-System.

- The push of a button triggers the corresponding Relais circuit (total: 2x 12V).

- Additionaly the Relais states are visible at the OLED screen.

- For further use, the Arduino will take digital or analog signals from the DLP-System and can be programmed to act as desired.

---

## Wires
### Control

- `Inputs:` 2 Buttons wired to D2 (Button 1) and D3 (Button 2) each to GND
- `Outputs:`      2 Relais signals wired to D6 (Relais 1) and D7 (Realais 2)
- `OLED screen:`  I2C connection SDA to A4 and SCK to A5

### Power

- `Arduino` gets powerd either with mini-USB (5V) or VIN and GND (7V-12V) (current option 2)
- DCDC-Buck is included (currently set to 5V)
- `The Relais` get powered from 
    - control signal from arduino (with jumper) from external source (5V DC) (without jumper)
    - external power source (max: 10A 250VAC, 30VDC)

---

## Convenient connections
For the ease of maintainance, the lid of the control box can be taken off and the components which are fixed to the lid are connected to the system via connectors.

- Momentary switch 1
- Momentary switch 2
- Oled screen


