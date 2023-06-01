import serial
from time import sleep

# Establish serial connection
arduino_port = 'COM6'       # Replace with Arduino's serial port
baud_rate = 500000          # Set the baud rate used by Arduino

C_RED = "\033[0;31m"
C_GREEN = "\033[0;32m"
C_BLUE = "\033[0;34m"
C_YELLOW = "\033[0;33m"
C_OFF = "\033[0m"


# Open the serial port
try:
    arduino = serial.Serial(arduino_port, baud_rate, timeout=.1)
    print(f"\n{C_BLUE}Establishing connection to arduino at port: {arduino_port} ...{C_OFF}")
    sleep(6)    # Very important! Wait for the Arduino to establish the connection
    arduino.reset_input_buffer()
    #arduino.readline()

except:
    print(f"{C_RED}Connection to port {arduino_port} failed.{C_OFF} Please check for valid COM port")
    exit()
    




class StepperPump:
    def __init__(self, whichPump = 3, stepsToGo = 0, retractSteps = 0, RPM = 0, direction = 1, retractDelay = 200, fanSpeed = 0, fanTime = 0):
        self.whichPump = whichPump
        self.stepsToGo = stepsToGo
        self.retractSteps = retractSteps
        self.RPM = RPM
        self.direction = direction
        self.retractDelay = retractDelay
        self.fanSpeed = fanSpeed
        self.fanTime = fanTime
        valueError = False


        if (self.whichPump < 1 or self.whichPump > 4):
            print(f"{C_RED}The current whichPump value is: {self.whichPump}, but must be between 1 and 4 (Axis: X,Y,Z,A).{C_OFF}")
            valueError = True

        if (self.stepsToGo < 0):
            print(f"{C_RED}The current stepsToGo value is: {self.stepsToGo}, but must be non-negative value.{C_OFF}")
            valueError = True
        
        if (self.retractSteps < 0):
            print(f"{C_RED}The current retractSteps value is: {self.retractSteps}, but must be non-negative value.{C_OFF}")
            valueError = True

        if (self.RPM < 1 or self.RPM > 500):
            print(f"{C_RED}The current RPM value is: {self.RPM}, but must be in range of [1-500].{C_OFF}")
            valueError = True

        if (self.direction != 1 and self.direction != 0):
            print(f"{C_RED}The current direction value is: {self.direction}, but must be 0 or 1.{C_OFF}")
            valueError = True

        if (self.retractDelay < 0):
            print(f"{C_RED}The current retractDelay value is: {self.retractDelay}, but must be non-negative value.{C_OFF}")
            valueError = True

        if (self.fanSpeed < 0 or self.fanSpeed > 100):
            print(f"{C_RED}The current fanSpeed value is: {self.fanSpeed}, but must be in range of [0-100%].{C_OFF}")
            valueError = True

        if (self.fanTime < -1):
            print(f"{C_RED}The current fanTime value is: {self.fanTime}, but must be non-negative for fanTime or -1 for always on.{C_OFF}")
            valueError = True


        if (valueError):
            print("\n{C_RED}please check your values.{C_OFF}")
            exit()



    def sendCommand(self):
        self.Data = f"{self.whichPump};{self.stepsToGo};{self.retractSteps};{self.RPM};{self.direction};{self.retractDelay};{self.fanSpeed};{self.fanTime}\n"
        arduino.write(str(self.Data).encode())
        print(f"{C_YELLOW}\nTransmitted Data to stepper {self.whichPump}:\n\
                whichPump: {self.whichPump}, stepsToGo: {self.stepsToGo}, retractSteps: {self.retractSteps}, RPM: {self.RPM}, direction: {self.direction}, retractDelay: {self.retractDelay}, fanSpeed: {self.fanSpeed}, fanTime: {self.fanTime}{C_OFF}")



    def deactivate(self):
        self.Data = f"{self.whichPump};{0};{0};{0};{0};{0};{0};{0}\n"
        arduino.write(str(self.Data).encode())
        print(f"{C_GREEN}Transmitted deactivation command (stepper {self.whichPump} disabled, fan disabled).{C_OFF}")



def MicrometerToSteps(micrometer):
    steps = micrometer * 30 / 16
    return steps
    
    
    
    
    
    
    
    
    
    
    
    
if __name__ == "__main__":


    micrometer = 10


#240
#3200 * 2.65
    
    pumpZ = StepperPump(
                            3,      # whichPump         Axis: X==1,Y==2,Z==3,A==4
                            MicrometerToSteps(micrometer),    # stepsToGo         3200 == 360degrees
                            0,     # retractSteps      amount of steps to retract, if 0 than no retraction
                            5,     # RPM               Rounds per minute
                            1,      # direction         1 == Rein, 0 == Raus
                            0,    # retractDelay      delay between steps and retraction [ms]
                            100,    # fanSpeed          value from 0-100 %
                            -1       # fanTime           after-run time of fan [s] if -1 fan is forever on
                       )
    
    #pumpX = StepperPump(1, 3200, 100, 30, 0, 500, 100, -1)


    while(True):
        uInput = input(f"\nDo you want to send the command?\n{MicrometerToSteps(micrometer)} steps / {micrometer} micrometers.) y/end: ")
        if (uInput == 'y'):
            pumpZ.sendCommand()
            #pumpX.sendCommand()
        
        elif (uInput == 'end'):
            pumpZ.deactivate()
            #pumpX.deactivate()
            sleep(5)
            arduino.close() # Close the serial port#
            print(f"{C_BLUE}Disconnected.{C_OFF}")
            exit()
        
        else:
            pass