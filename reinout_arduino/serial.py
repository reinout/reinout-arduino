import serial


def main():
    ser = serial.Serial('/dev/tty.wchusbserial1410', 9600)
    while True:
        print(ser.readline())
