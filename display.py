import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 115200)
time.sleep(5)

print('Sending')
ser.write('title;Ships in Space\n')
time.sleep(1)

print('Sending')
ser.write('drawCrossHair;200;180;63488\n')
time.sleep(1)

print('Sending')
ser.write('drawBox;50;180;10;2016\n')
time.sleep(1)

print('Sending')
ser.write('drawBox;50;180;20;2016\n')
time.sleep(1)

print('Sending')
ser.write('fillTriangle;200;120;200;130;210;125;65535\n')

while True:
    pass
