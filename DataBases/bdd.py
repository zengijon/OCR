from PIL import Image 
from PIL import ImageFont
from PIL import ImageDraw
from random import *
from os import listdir
from os.path import isfile, join




def BaseDeDonnees(filename, font):

	sizefont = randint(12, 72)
	width = 1920
	height = 1080

	print(font)


	f1 = ImageFont.truetype(font, sizefont)

	img = Image.new('RGB', (width, height), color = 'white')

	draw = ImageDraw.Draw(img)

	i = 48
	y = 0

	while (y < height-sizefont):

		x = 0
		while (x < width-sizefont):

			if i == 58:
				i = 65
				x =0
				y += sizefont
				caractere = chr(i)
				draw.text((x, y), caractere, (0,0,0), font=f1)
				i += 1
				x += sizefont

			elif i == 91:
				i = 97
				x = 0
				y += sizefont
				caractere = chr(i)
				draw.text((x, y), caractere, (0,0,0), font=f1)
				i += 1
				x += sizefont

			elif i == 123:
				break

			else:
				caractere = chr(i)
				draw.text((x, y), caractere, (0,0,0), font=f1)
				i += 1
				x += sizefont
		y += sizefont


	img.save(filename)


fichiers_polices = [f for f in listdir("C:/Users/Addam/Documents/fonts") if isfile(join("C:/Users/Addam/Documents/fonts", f))]

a = 1

for font in fichiers_polices:
	filename= "C:/Users/Addam/BaseDeDonnees(HARDCORE)2/Train" + str(a) + ".png"
	font = "C:/Users/Addam/Documents/fonts/" + font
	if(font[len(font)-1] == 'f'):
		BaseDeDonnees(filename, font)
		a += 1
	
