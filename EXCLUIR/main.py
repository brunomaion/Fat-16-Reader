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




bytesSetor = juntar2Bytes(vetorHex[11], vetorHex[12])
setorCluster = vetorHex[13]
setoresReservados =  juntar2Bytes(vetorHex[14], vetorHex[15])
directoryEntries = juntar2Bytes(vetorHex[17], vetorHex[18])
setoresFat = juntar2Bytes(vetorHex[22], vetorHex[23])

inicioFat = setoresReservados*directoryEntries*setoresFat
rootDirInicio = 1+(2*setoresFat)
tamanhoRootDir = 32 * directoryEntries
dadosInicia = (rootDirInicio*bytesSetor)+tamanhoRootDir


######### FINAL
print("\n","----------------- LEITOR FAT 16 ----------------------", "\n")

##Boot Record 1 Setor

#BR 11
print("Nº de bytes por setor: ", bytesSetor)

#BR 13
print("Nº de setores por cluster: ", setorCluster)

#BR 16
print("Nº de setores reservados: ", setoresReservados)


#BR 17
print("Nº de entradas diretorio raiz: ", directoryEntries)

#BR 22
print("Nº setores por fat: ", setoresFat)

print("\n")

print("Fat 1 inicia em: ", "(deslocamento) ", 1, "(bytes)",bytesSetor) # BR 1 - F1 156 - F2 311
print("Fat 2 inicia em: ", "(deslocamento) ", setoresFat+1, "(bytes) ", (setoresFat+1)*bytesSetor) # BR 1 - F1 156 - F2 311
print("Root Dir inicia em: ", "(deslocamento) ", rootDirInicio, "(bytes) ", (rootDirInicio*bytesSetor))
print("Dados inicia em: ", "(deslocamento) ", dadosInicia)



