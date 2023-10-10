from PIL import Image

img = Image.new('RGB', (512, 512))

file = open('lena512.raw', 'rb')
data = file.read()
file.close()

pix = img.load()
d = ((0, 127), (192, 63))
d2 = ((0, 128, 32, 160), (192, 64, 224, 96), (48, 176, 16, 144), (240, 112, 208, 80))

for y in range(512):
    for x in range(512):
        i = x % 4
        j = y % 4
        gray = 255 if data[y * 512 + x] > d2[i][j] else 0
        pix[x, y] = (gray, gray, gray)

img.show()
