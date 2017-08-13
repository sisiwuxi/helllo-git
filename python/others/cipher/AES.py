import math
import base64
from Crypto.Cipher import AES
from Crypto import Random

# padding
BS = 16
pad = lambda s: s + (BS - len(s) % BS) * chr(0)
#pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS) 
unpad = lambda s : s[0:-ord(s[-1])]

#str2binary buff
def parse_hex(hex_str):
    l=int(math.ceil(len(hex_str)/2))
    buf=''
    for i in range(0,l):
        s=hex_str[(i*2):((i+1)*2)]
        buf=buf+chr(int(s,16))
    return buf

#parser key,16-AES-128,24-AES-196,32-AES-256
key="1234567890123456"
iv="0102030405060708"
#key=parse_hex("68b329da9893e34099c7d8ad5cb9c940")
#iv=parse_hex("68b329da9893e34099c7d8ad5cb9c940")

f=open('1.txt','r')
try:
    for line in open('1.txt'):
        text=f.readline()
        print('text='+text)
finally:
    f.close()
raw_buf=parse_hex(text)

#create an AES object
cipher = AES.new(key, AES.MODE_CBC, iv)

#padding
PaddingBuf=pad(raw_buf)
print('PaddingBuf='+PaddingBuf)
f_PaddingBuf=open("PaddingBuf.txt",'w')
try:
    f_PaddingBuf.write(PaddingBuf)
finally:
    f_PaddingBuf.close()

#encrypt
CipherBuf=cipher.encrypt(PaddingBuf)
print('CipherBuf='+CipherBuf)
f_CipherBuf=open("CipherBuf.txt",'w')
try:
    f_CipherBuf.write(CipherBuf)
finally:
    f_CipherBuf.close()

#decrypt
'''
f=open('CipherBuf.txt','rb')
try:
    #text=f.read()
    #print('text='+text)
    temp=''
    for line in f:
        #temp=temp+line.rstrip('\n')
        temp=temp+line
        print('temp='+temp)
finally:
    f.close()

length_CipherBuf=len(CipherBuf)
length_temp=len(temp)
print('length_CipherBuf='+str(length_CipherBuf)+' length_temp='+str(length_temp))
PlainBuff=aes_obj.decrypt(temp)
print('PlainBuff='+PlainBuff)
'''

cipher = AES.new(key, AES.MODE_CBC, iv)
PlainBuff=cipher.decrypt(CipherBuf)
print('PlainBuff='+PlainBuff)
f_PlainBuff=open("PlainBuff.txt",'w')
try:
    f_PlainBuff.write(PlainBuff)
finally:
    f_PlainBuff.close()



