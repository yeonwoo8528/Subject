from PIL import Image

img = Image.new('RGB', (512, 512))
pix = img.load()

file = open('lena512_1bit.raw', 'rb')
data = file.read()
file.close()

for y in range(512):
    for x in range(512):
        pixel = data[y * 64 + x // 8]
        bit = (pixel >> (x % 8)) & 1
        tmp = bit * 255
        pix[x, y] = (tmp, tmp, tmp)

img.show()
