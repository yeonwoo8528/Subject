from PIL import Image

in_file = open('lena512.raw', 'rb')
out_file = open('lena512_1bit.raw', 'wb')
b = bytearray(1)

for i in range(512 * 512 // 8):
    data = in_file.read(8)
    b[0] = 0b00000000
    for j in range(8):
        b[0] |= 0b00000001 << j if data[j] > 127 else 0b00000000
    out_file.write(b)

in_file.close()
out_file.close()
