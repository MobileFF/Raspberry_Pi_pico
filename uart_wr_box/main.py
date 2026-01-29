from machine import Pin, I2C,UART,SPI
import ssd1306
import time
import os
import sdcard
import sys

# setup
print("UART WR Box")

# Initial Status

# Screen
#  0 : Init Screen
#  1 : Save Mode
#  2 : Load Mode
#  3 : Setting Mode
screen = 0

# Sequence in Screen
seq = 0

# Mode
#  0 : save mode
#  1 : load mode
#  2 : config mode
mode = 0

# Baudrate
baud_rate = 9600

# constant
modeStr = ["SAVE","LOAD","CONFIG"]

# file list
fileList = []
fileListIdx = 0

# Tact switch and LED
sw1 = Pin(18,Pin.IN,Pin.PULL_DOWN)
sw2 = Pin(19,Pin.IN,Pin.PULL_DOWN)
sw3 = Pin(20,Pin.IN,Pin.PULL_DOWN)
led = Pin("LED",Pin.OUT)

# UART
uart = UART(1,baudrate=baud_rate,tx=Pin(4),rx=Pin(5))

# OLED
i2c = I2C(0, sda=Pin(0), scl=Pin(1) )
display = ssd1306.SSD1306_I2C(128, 64, i2c)

# OLED I2C address(usually 0x3C)
addr = i2c.scan()
print( "OLED I2C Address :" + hex(addr[0]) )
print( "OLED Initialized")
display.text("UART WR Box",16,16,True)
display.show()
time.sleep(1)
display.fill(0)

# microSD Card(SPI)
cs = Pin(13,Pin.OUT,Pin.PULL_UP)
cs.value(1)

#spi = SPI(1,sck=Pin(10), mosi=Pin(11), miso=Pin(12))
spi = machine.SPI(1,
    baudrate=100000,
    polarity=0,
    phase=0,
    sck=machine.Pin(10),
    mosi=machine.Pin(11),
    miso=machine.Pin(12)
)
sd = None
while True:
    try:
        print("SD Card initialize...")
        sd = sdcard.SDCard(spi, cs)
        os.mount(sd, '/sd')
        print(os.listdir('/sd'))
        break
    except Exception as e:
        sys.print_exception(e)
        print("no SD Card")
        display.text("no SD Card",0,0,True)
        display.show()
        display.fill(0)
        time.sleep(5)

def screenControl():
    if(screen==0):
        screen_init()
    elif(screen==1):
        screen_sv()
    elif(screen==2):
        screen_ld()
    elif(screen==3):
        screen_cfg()
            
def screen_init():
    global modeStr,mode,screen
    display.text("Select: "+modeStr[mode],0,0,True)
    display.show()
    if (sw1.value()==1):
        mode = 2 if (mode==0) else mode-1
        time.sleep_ms(300)
        display.fill(0)
    if (sw2.value()==1):
        mode = 0 if (mode==2) else mode+1
        time.sleep_ms(300)
        display.fill(0)
    if (sw3.value()==1):
        screen = mode + 1
        time.sleep_ms(300)
        display.fill(0)

def screen_sv():
    global screen,seq,mode
    if (sw3.value()==1 and seq==0):
        seq += 1
    if (seq == 0):
        display.text("PRESS [OK]",0,0,True)
        display.show()
    else:
        display.text("save start...",0,16,True)
        display.show()
    
        firstLine = True
        findComment = False
        filename_len = 12
        filename = ""
        firstLineText = ""
        myFile = None
        
        while True:
            if (uart.any() == 0):
                continue
            
            valba = uart.read(1)
            val = valba[0]
            if (val == 0x1a):
                break
            else:
                if (val >= 0x20 or val == 0x0d or val == 0x0a):
#                     valStr = " "
#                     valStr[0] = chr(val)
                    valStr = chr(val)                    
                    print(valStr,end="")
                    
                    if (firstLine):
                        firstLineText = firstLineText + valStr
                    else:
                        #myFile.write(valStr)
                        myFile.write(valba)
                        pass
                    
                    if (firstLine and findComment and len(filename) < filename_len and val != 0x0d and val != 0x0a):
                        filename = filename + valStr
                    
                    # 先頭行に「'」(Single Quote)を発見したらそのあとの文字をファイル名とみなす
                    if (firstLine and val == 0x27):
                        findComment = True # コメント記号を発見したかどうかのフラグ(True＝コメント記号有）
                    
                    if (firstLine and (val == 0x0d or val == 0x0a)):
                        firstLine = False
                        if (findComment == False):
                            filename = "FOOBAR.TXT"
                        myFile = open("/sd/"+filename,mode="wb")
                        myFile.write(firstLineText)
                        print("Filename:"+filename)
                        print(firstLineText,end="")

        myFile.close()
         
        while(uart.any()>0):
             uart.read(1)
            
        display.text("Finish.",0,32,True)
        display.show()
        time.sleep_ms(3000)
        
        screen = 0
        seq = 0
        mode = 0
        display.fill(0)

def screen_ld():
    global seq,mode,screen,fileList,fileListIdx
    
    if (len(fileList) == 0):
        fileList = os.listdir('/sd')
        print(fileList)
        fileListIdx = 0
    
    display.text("Select Load File:",0,0,True)
    
    if(seq == 0 and sw1.value()==1):
        display.fill(0)
        print("load prev")
        fileListIdx -= 1
        if (fileListIdx < 0):
            fileListIdx = len(fileList)-1
        print(fileList[fileListIdx])
        time.sleep_ms(300);
    if(seq == 0 and sw2.value()==1):
        display.fill(0)
        print("load next")
        fileListIdx += 1
        if (fileListIdx >= len(fileList)):
            fileListIdx = 0
        print(fileList[fileListIdx])
        time.sleep_ms(300);

    display.text(fileList[fileListIdx],0,8,True)
    display.show()

    if(sw3.value()==1):
        seq += 1
        time.sleep_ms(300);

        if (seq==1):
            display.text("PRESS [OK]",0,16,True)
            display.show()
            time.sleep_ms(300);
            
        elif (seq==2):
            display.text("Load start...",0,24,True)
            display.show()

            myFile = open("/sd/"+fileList[fileListIdx],mode="rb")

            fsize=0
            while True:
                val = myFile.read(1)
                fsize += 1
                if (len(val)==0):
                    uart.write(chr(0x1A))
                    break
#                 if (val[0] == ''):
#                     uart.write(chr(0x1A))
#                     break
                uart.write(val)
                print("{:04x}:{:02x}({})".format(fsize,val[0],chr(val[0])))
                if fsize < 32:
                    pass
                    #print("{:04x}:{:02x}({})".format(fsize,val[0],chr(val[0])))
                    #print(val,end="")
                elif fsize % 1024 == 0:
                    print("[{}]".format(fsize))
                # print(val,end="")
                time.sleep_ms(10)
            myFile.close()
            display.text("Finish.",0,32,True)
            display.show()
            time.sleep_ms(3000)
            seq=0
            mode=0
            screen=0
            display.fill(0)

print("Start")
while True:
    screenControl()

