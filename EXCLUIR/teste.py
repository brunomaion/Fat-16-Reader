import struct

a = hex(0)
b = hex(2)
print(a)

# Junta os dois bytes em uma única string de bytes
def juntar2Bytes(byte1, byte2):
    bytes_juntos = struct.pack('BB', int(byte2, 16), int(byte1, 16))
    return bytes_juntos.hex() 


print(juntar2Bytes(a, b)) 
print(int(juntar2Bytes(a, b), 16))

