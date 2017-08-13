import rsa

#create key
(pubkey, privkey) = rsa.newkeys(1024)

#save key
with open('public.pem','w+') as f:
    f.write(pubkey.save_pkcs1().decode())

with open('private.pem','w+') as f:
    f.write(privkey.save_pkcs1().decode())

#load key
with open('public.pem','r') as f:
    pubkey = rsa.PublicKey.load_pkcs1(f.read().encode())

with open('private.pem','r') as f:
    privkey = rsa.PrivateKey.load_pkcs1(f.read().encode())
    
#Plain
message = 'hello'

#encrypt by public key
crypto = rsa.encrypt(message.encode(), pubkey)
print('crypto='+crypto)
f=open('rsa_crypto.txt','w')
try:
	f.write(crypto)
finally:
	f.close()

#decrypt by private key
message = rsa.decrypt(crypto, privkey).decode()
print('message='+message)

#encrypt by private key
signature = rsa.sign(message.encode(), privkey, 'SHA-1')
print('signature='+signature)

#decrypt by public key
rsa.verify(message.encode(), signature, pubkey)
