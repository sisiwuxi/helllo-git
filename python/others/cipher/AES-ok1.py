import base64
from Crypto.Cipher import AES
from Crypto import Random

# padding
BS = 16
#pad = lambda s: s + (BS - len(s) % BS) * chr(0)
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS) 
unpad = lambda s : s[0:-ord(s[-1])]

#parser key,16-AES-128,24-AES-196,32-AES-256
key="1234567890123456"
iv="0102030405060708"

f=open('1.txt','r')
try:
    for line in open('1.txt'):
        text=f.readline()
        #print('text='+text)
finally:
    f.close()

#create an AES object
cipher = AES.new(key, AES.MODE_CBC, iv)

#padding
PaddingBuf=pad(text)
#print('PaddingBuf='+PaddingBuf)
f_PaddingBuf=open("PaddingBuf.txt",'w')
try:
    f_PaddingBuf.write(PaddingBuf)
finally:
    f_PaddingBuf.close()

#encrypt
CipherBuf=cipher.encrypt(PaddingBuf)
#print('CipherBuf='+str(CipherBuf))
CipherBuf_base64=base64.b64encode(CipherBuf) 
#print('CipherBuf_base64='+str(CipherBuf_base64))
f_CipherBuf=open("CipherBuf.txt",'w')
try:
    f_CipherBuf.write(str(CipherBuf_base64))
finally:
    f_CipherBuf.close()

#decrypt
enc=base64.b64decode(CipherBuf_base64)
#create an AES object, must create everytime
cipher = AES.new(key, AES.MODE_CBC, iv)
PlainBuff=cipher.decrypt(enc)
#print('PlainBuff='+str(PlainBuff))
f_PlainBuff=open("PlainBuff.txt",'w')
try:
    f_PlainBuff.write(str(PlainBuff))
finally:
    f_PlainBuff.close()



