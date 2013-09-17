
# Copiar archivo a buffer

ifile = open("diccionario_es.txt", "r");
buffer = ifile.read();
ifile.close();

# Preparamos string, eliminamos caracteres invalidos

buffer = buffer.lower();

invalid_chars = ",.-;:_{}[]()/\"\n+?¿!¡`´";
for char in invalid_chars :
    buffer = buffer.replace( char, ' ' );


# Obtener lista de palabras

dict={}                         # Creamos el diccionario
palabras = buffer.split();      # Obtenermos todas las palabras 

for palabra in palabras :     # Eliminamos repeticiones
    dict[palabra] = 0;
palabras = dict.keys();

# Guardar lista en nuevo archivo

ofile = open("dict.txt", "w");
for palabra in palabras :
    ofile.write(palabra+"\n");
ofile.close();
