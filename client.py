import socket
import sys
import subprocess



s = socket.socket()
s.connect(("localhost",9998))

n = s.recv(16)
print(n)

prc = subprocess.Popen("make>/dev/null &&./run e", stdout=subprocess.PIPE, shell=True)
key = str(prc.communicate()[0])

rabinKey = subprocess.Popen("./rabin e "+str(n)+" "+key, stdout=subprocess.PIPE, shell=True)
rabinKey = str(rabinKey.communicate()[0])
print(key)

s.send(rabinKey)

f = open ("cipherImage.bmp", "rb")
l = f.read(1024)
while (l):
    s.send(l)
    l = f.read(1024)
s.close()
print("bitti")