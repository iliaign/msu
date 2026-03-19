import time
import serial
from PIL import Image

image = Image.open('1773925003614.jpg')


ser = serial.Serial('COM9', 9600, timeout=1)

img = image.resize((320, 240))
width, height = img.size


try:
    i = 0
    max = width * height 
    command = f"disp_screen 0xFF00FF\n"
    ser.write(command.encode())
    ##print(image.height, image.width)
    for y in range(height):
        for x in range(width):
            r, g, b =img.getpixel((x, y))
            color = (r << 16) | (g << 8) | b
            command = f"disp_px {x} {y} {color:06X}\n"
            ser.write(command.encode())
            i+=1
            if i % 1000 == 0:
                print(f" {i}/{max} {i/max:.1%} pixels")
            #time.sleep(0.001)  # небольшая пауза между пикселями, если нужно
finally:
    time.sleep(0.1)
    ser.close()
