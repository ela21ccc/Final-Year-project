import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev


GPIO.setmode(GPIO.BCM)
receivedMessage0= []
receivedMessage1= []
pipes=[[0xF0,0xF0,0xF0,0xF0,0xE1],[0xF0,0xF0,0xF0,0xF0,0xE2],[0xF0,0xF0,0xF0,0xF0,0xE3]]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0,17)

radio.setPayloadSize(32)
radio.setChannel(0x76)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)

radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()
radio.openReadingPipe(1,pipes[1])
radio.openReadingPipe(2,pipes[2])

radio.startListening()

radio.printDetails()


while True:

    while not radio.available(0):
        time.sleep(1/100)
        
 

    radio.read(receivedMessage0,radio.getDynamicPayloadSize())
    print('Received: {}'.format (receivedMessage0))
    print('Translating our received Message into unicode characters... ')
    string0 =''
    for n in receivedMessage0:
        if(n >= 32 and n <= 126):
            string0=string0 + chr(n)
    print('Patient 1 parameters: {}'.format(string0))
    DataPat1=format(string0)
    print(DataPat1)
    bp1=DataPat1[1:5]
    Temp1=DataPat1[6:11]
    print('The bloodpressure of patient 1 is',bp1)
    print('The body Temperature of patient 1 is',Temp1)
    
    for i in range (0,10):
        print(" ")
        
    # For the Second TX

    if(radio.available()):
        radio.read(receivedMessage1,radio.getDynamicPayloadSize())
    print('Received: {}'.format (receivedMessage1))
    print('Translating our received Message into unicode characters... ')
    string1 =''
    for n in receivedMessage1:
        if(n >= 32 and n <= 126):
            string1 =string1+ chr(n)
    print('Patient 2 parameters: {}'.format(string1))
    DataPat2=format(string1)
    print(DataPat2)
    bp2=DataPat2[1:5]
    Temp2=DataPat2[6:11]
    print('The bloodpressure of patient 2 is',bp2)
    print('The body Temperature of patient 2 is',Temp2)
    for i in range (0,10):
        print(" ")
