import struct

nomeImagem ="fat161s.img"
#nomeImagem ="fat164s.img"

vetorBytes = []
vetorHex = []

########### Funções ################################


# Junta os dois bytes em uma única string de bytes
def juntar2Bytes(byte1, byte2):
    bytes_juntos = struct.pack('BB', int(byte2, 16), int(byte1, 16))
    return bytes_juntos.hex() 

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




bytesSetor = juntar2Bytes(vetorHex[11], vetorHex[12])
setorCluster = vetorHex[13]
setoresReservados =  juntar2Bytes(vetorHex[14], vetorHex[15])
directoryEntries = juntar2Bytes(vetorHex[17], vetorHex[18])
setoresFat = juntar2Bytes(vetorHex[22], vetorHex[23])
inicioFat = (int(setoresReservados, 16))*int(directoryEntries, 16)*(int(setoresFat, 16)+1)

######### FINAL
print("\n","----------------- LEITOR FAT 16 ----------------------", "\n")

#BR 11
print("Nº de bytes por setor: ", int(bytesSetor, 16))

#BR 13
print("Nº de setores por cluster: ", int(setorCluster, 16))

#BR 16
print("Nº de setores reservados: ", int(setoresReservados, 16))

#BR 17
print("Nº de entradas diretorio raiz: ", int(directoryEntries, 16))

#BR 22
print("Nº setores por fat: ", int(setoresFat, 16))

print(inicioFat)
