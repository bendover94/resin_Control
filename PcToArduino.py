import serial
from time import sleep

# Establish serial connection
arduino_port = 'COM4'   # Replace with Arduino's serial port
baud_rate = 500000        # Set the baud rate used by Arduino

# Open the serial port
arduino = serial.Serial(arduino_port, baud_rate, timeout=.1)
sleep(6)
# Wait for the Arduino to establish the connection
arduino.reset_input_buffer()
#arduino.readline()


stepsToGo = 3200    # 3200 == 360degrees
retractSteps = 50   # steps to retract
RPM = 30            # Rounds per minute
direction = 1       # 1 == CW, 0 == CCW
retraction = 1      # toggle for retraction, 1 == on, 0 == off

Data = f"{stepsToGo};{retractSteps};{RPM};{direction};{retraction}\n"

try:
    arduino.write(str(Data).encode())

finally:
    print(f"Transmitted data: {Data}")
    arduino.close()


# # Close the serial port



