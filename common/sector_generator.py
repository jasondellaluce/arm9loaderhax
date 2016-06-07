#!/usr/bin/env python

from __future__ import print_function
from Crypto.Cipher import AES
from Crypto.Hash import SHA256
from Crypto.Util import Counter
from ctypes import *
import struct
import sys

#http://www.falatic.com/index.php/108/python-and-bitwise-rotation
rol = lambda val, r_bits, max_bits: \
    (val << r_bits%max_bits) & (2**max_bits-1) | \
    ((val & (2**max_bits-1)) >> (max_bits-(r_bits%max_bits)))

ror = lambda val, r_bits, max_bits: \
    ((val & (2**max_bits-1)) >> r_bits%max_bits) | \
    (val << (max_bits-(r_bits%max_bits)) & (2**max_bits-1))

def to_bytes(num):
    numarr = bytearray()
    tmp = num
    while len(numarr) < 16:
        numarr.append(tmp & 0xFF)
        tmp >>= 8
    return bytes(numarr[::-1])

with open(sys.argv[1], "rb") as sectorFile:
    secretSector = sectorFile.read(0x200)

    with open(sys.argv[2], "rb") as otpFile:
        hash = SHA256.new()
        otpData = otpFile.read(0x90)
        hash.update(otpData)
        keyX = ''.join(hash.hexdigest()[0:32])
        keyY = ''.join(hash.hexdigest()[32:64])
        normalKey = rol((rol(int(keyX, 16), 2, 128) ^ int(keyY, 16)) + 0x1FF9E9AAC5FE0408024591DC5D52768A, 87, 128)
        print(normalKey)
        ecbmode = AES.new(to_bytes(normalKey), AES.MODE_ECB)

        with open(sys.argv[3], "wb") as outFile:
            for x in range(0, 0x20):
                outFile.seek(x*0x10)
                if x == 1 :
                    outFile.write(ecbmode.encrypt(to_bytes(int('0824D3CB4AE94D624DAA526047C59394', 16))))
                else :
                    outFile.write(ecbmode.encrypt(secretSector[x*0x10:(x+1)*0x10]))
