import socket
import sys
import subprocess

# Python code to remove duplicate elements 
def Remove(duplicate): 
    final_list = [] 
    for num in duplicate: 
        if num not in final_list: 
            final_list.append(num) 
    return final_list 

def listToString(s):  
    
    # initialize an empty string 
    str1 = ""  
    
    # traverse in the string   
    for ele in s:  
        str1 += ele+" "   
    
    # return string   
    return str1

s = socket.socket()
s.bind(("localhost",9998))

print("dinlemede")

s.listen(10) # Accepts up to 10 connections.


sc, address = s.accept()

buffer = subprocess.Popen("./rabin k", stdout=subprocess.PIPE, shell=True)

p, q, n = str(buffer.communicate()[0]).split(' ')

#print(p,q,n)

sc.send(n)


rabinKey=str(sc.recv(2048))

decryptedRabin = subprocess.Popen("./rabin d "+str(p)+" "+str(q)+" "+rabinKey, stdout=subprocess.PIPE, shell=True)


allKeys = list(decryptedRabin.communicate()[0].split(" "))
while True:
    try:
        allKeys.remove('')
    except:
        break
a=len(allKeys)
i=0
#print(len(allKeys))
while i < a:
    if int(allKeys[i]) > 255:
        del allKeys[i] 
        a=a-1
        i=i-1
        
    i=i+1

allKeys=Remove(allKeys)

key = listToString(allKeys)

print(key)


f = open("Recv.bmp",'wb') #open in binary
l=1
while (l):       
# receive data and write it to file
    l = sc.recv(1024)
    while (l):
            f.write(l)
            l = sc.recv(1024)
            #print(sc)
f.close()
print("dosya kapandi")


sc.close()
print("baglanti kapandi")

subprocess.Popen("./run d "+key, stdout=subprocess.PIPE, shell=True)


s.close()
print("socket kapandi")