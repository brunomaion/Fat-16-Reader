import struct

nomeImagem ="fat161s.img"
#nomeImagem ="fat164s.img"

vetorBytes = []
vetorHex = []

########### Funções ################################


# Junta os dois bytes em uma única string de bytes
def juntar2Bytes(byte1, byte2):
    byte1= str(byte1)
    byte2= str(byte2)
    bytes_juntos = struct.pack('BB', int(byte2, 16), int(byte1, 16))
    result = int.from_bytes(bytes_juntos, byteorder='big')
    return result


####################################################

# Abre o arquivo em modo binário
with open(nomeImagem, 'rb') as f:
     
    # Lê os bytes do arquivo em blocos de 1 byte
    byte = f.read(1)

    while byte:
        # Converte o byte em hexadecimal
        hex_byte = hex(byte[0])
        # Adiciona o byte e seu valor hexadecimal às listas
        vetorBytes.append(byte)
        vetorHex.append(hex_byte)
        
        # Lê o próximo byte
        byte = f.read(1)


print(type(vetorHex[11]))

bytesSetor = juntar2Bytes(vetorHex[11], vetorHex[12])



print(bytesSetor)
#print(type(bytesSetor))
