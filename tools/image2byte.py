from PIL import Image
import sys
import math
import re

if len(sys.argv) != 4:
	print("image2byte <path> <size x> <size y>")
	exit()

path = sys.argv[1]
x = int(sys.argv[2])
y = int(sys.argv[3])
size = x * y

im = Image.open(path)
indata = list(im.getdata())
outdata = [0] * size
strdata = ""

for i, pixel in enumerate(indata):
	if pixel[0] == 255 and pixel[1] == 255 and pixel[2] == 255: #look for white pixels
		strdata += "-"
	else:
		outdata[i] = 1
		strdata += "*"
	if i%x == (x-1):
		strdata += "\n"

print("image representation:")
print(strdata)
with open("imagedata.txt","w") as f:
	f.write("const uint8_t level_map[]["+str(x)+"] PROGMEM =\n{\n")
	for i, item in enumerate(outdata):
		if i == 0:
			f.write("  {")
		elif i % x == 0:
			f.write("},\n  {")
		else:
			f.write(",")
		f.write(str(item))
	f.write("}\n};")
print("saved to imagedata.txt")


