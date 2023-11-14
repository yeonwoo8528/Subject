from PIL import Image
import numpy as np
from math import cos, pi, sqrt

luminance_quantization_table = np.array([
    [16, 11, 10, 16, 24, 40, 51, 61],
    [12, 12, 14, 19, 26, 58, 60, 55],
    [14, 13, 16, 24, 40, 57, 69, 56],
    [14, 17, 22, 29, 51, 87, 80, 62],
    [18, 22, 37, 56, 68, 109, 103, 77],
    [24, 35, 55, 64, 81, 104, 113, 92],
    [49, 64, 78, 87, 103, 121, 120, 101],
    [72, 92, 95, 98, 112, 100, 103, 99]
])

def forward_dct(block):
    result = np.zeros((8, 8))

    for u in range(8):
        for v in range(8):
            cu = 1 / sqrt(2) if u == 0 else 1
            cv = 1 / sqrt(2) if v == 0 else 1

            sum_val = 0
            for x in range(8):
                for y in range(8):
                    sum_val += block[y, x] * cos((2 * x + 1) * u * pi / 16) * cos((2 * y + 1) * v * pi / 16)

            result[v, u] = cu * cv * sum_val / 4

    return result

def quantize_dct(dct_result, quantization_table):
    return np.round(dct_result / quantization_table)

def dequantize_dct(quantized_dct, quantization_table):
    return quantized_dct * quantization_table

def inverse_dct(block):
    result = np.zeros((8, 8))

    for x in range(8):
        for y in range(8):

            sum_val = 0
            for u in range(8):
                for v in range(8):
                    cu = 1 / sqrt(2) if u == 0 else 1
                    cv = 1 / sqrt(2) if v == 0 else 1
                    sum_val += cu * cv * block[v, u] * cos((2 * x + 1) * u * pi / 16) * cos((2 * y + 1) * v * pi / 16)

            result[y, x] = sum_val / 4

    return result

img = Image.new('L', (512, 512))
result_img = Image.new('RGB', (512, 512))
pix = img.load()
result_pix = result_img.load()

file = open('lena512.raw', 'rb')
data = file.read()
file.close()

for y in range(img.height):
    for x in range(img.width):
        pix[x, y] = (data[y * img.width + x])

img_array = np.array(img)

for y in range(0, img.height, 8):
    for x in range(0, img.width, 8):
        block = img_array[y : y + 8, x : x + 8]
        dct_block = forward_dct(block)
        quantized_dct_block = quantize_dct(dct_block, luminance_quantization_table)
        dequantized_dct_block = dequantize_dct(quantized_dct_block, luminance_quantization_table)
        img_array[y : y + 8, x : x + 8] = inverse_dct(dequantized_dct_block)

for y in range(img.height):
    for x in range(img.width):
        result_pix[x, y] = (img_array[y, x], img_array[y, x], img_array[y, x])

result_img.show()
