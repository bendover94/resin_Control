# Resin exchange controller V1.0 
by Lucas Schmirl, dept. industrial engineering, UAS Technikum Wien, mr22m015@technikum-wien.at

---

# Control an arduino Nano and 2 channel Relais to be used with the DLP-System.

- The push of a button triggers the corresponding Relais circuit (total: 2x 12V).

- Additionaly the Relais states are visible at the OLED screen.

- For further use, the Arduino will take digital or analog signals from the DLP-System and can be programmed to act as desired.



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



## Convenient connections
For the ease of maintainance, the lid of the control box can be taken off and the components which are fixed to the lid are connected to the system via connectors.

- Momentary switch 1
- Momentary switch 2
- Oled screen



---

# Control a stepper motor with arduino uno
controlled from PC (USB-connection) using python.

[![Stepper control unit animation](https://res.cloudinary.com/marcomontalbano/image/upload/v1685572570/video_to_markdown/images/youtube--DMu0SnQDX-w-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/DMu0SnQDX-w "Stepper control unit animation")

# Requirements

install python3 for windows

install pip:
```bash
python3 -m pip install --upgrade pip
```

install pyserial:
```bash
python3 -m pip install pyserial
```

run program with:
```bash
python3 ./PcToArduino.py
```

## Fragen an Betreuer

- Steuerung: Matlab und Kommunikation zu arduino oder nur Arduino zu stepper erklären?
- Regler auch erklären?
- Was vom alten paper kann bleiben?
- Was von den alten prototypen darf ich beschreiben (erklärung warum sie nicht verwendet wurden)?
- Bedienungsanleitung?
- Wann kann ich das nächste mal für feedback abgeben?
- Komponentenliste in Paper anhängen? Preise? wer zahlt?
- Zugang für Github Repo?
- Repo auch anhängen?


Paper: [edit](https://www.overleaf.com/1342524919kjhmkdnryqwf), [view](https://www.overleaf.com/read/ywpgvjhnpwxv)