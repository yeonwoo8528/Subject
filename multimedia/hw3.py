from PIL import Image

with open('forestfire.bmp', 'rb') as file:
    data = file.read()

bfoffBits = int.from_bytes(data[0x0A:0x0E], 'little')
biWidth = int.from_bytes(data[0x12:0x16], 'little')
biHeight = int.from_bytes(data[0x16:0x1A], 'little')

img = Image.new('RGB', (biWidth, biHeight))
pix = img.load()
temp = biHeight - 1
bit = bfoffBits

for y in range(biHeight - 1, -1, -1):
    for x in range(biWidth):
        if(temp - y): bit += 2
        pix[x, y] = (data[bit + 2], data[bit + 1], data[bit])
        bit += 3
        temp = y

img.show()
