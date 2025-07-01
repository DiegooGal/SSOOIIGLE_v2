# Practica 3 - SSOOIIGLE v2.0

----- Descripción -----

El objetivo es crear un buscador con varios tipos de clientes, puede haber 3 tipos:
	- free_client: Usuario con cuenta gratuita, con busquedas limitadas a un número fijo de palabras.
	- premium_client: Usuario con cuenta de pago, cada palabra encontrada supone el coste de 1 credito.
	- unlimited_client: Usuario sin restricciones de busquedas.
La busqueda se hace utilizando varios hilos en un solo archivo, que se gestionan y crean por el programa.


----- Compilación -----

Se proporciona un Makefile para compilar el programa. Para compilar, ejecute:

make all

Esto generará un ejecutable manager en el directorio exec/.

Para testear el código ejecute:

make test

Para limpiar los archivos generados ejecute:

make clean

Para limpiar el archivo de resultados ejecute:

make clear_file

----- Estructura del Makefile -----

DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

#g++ -g archivo.cpp -o archivo -pthread -std=c++17

CFLAGS := -I$(DIRHEA) -pthread -std=c++17
CC := g++

all : dirs manager  #pa, pb

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

manager: 
	$(CC) -o $(DIREXE)manager $(DIRSRC)manager.cpp $(CFLAGS)	

test:
	./$(DIREXE)manager 

solution:
	./$(DIREXE)manager

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~

clear_file: 
	echo "" > docs/Resultado_busquedas.txt


----- Estructura del Proyecto -----

El código está organizado en los siguientes directorios:

src/ - Código fuente del proyecto
include/ - Archivos de cabecera
obj/ - Archivos objeto generados durante la compilación
exec/ - Binarios ejecutables
docs/ - Archivos de texto para el diccionario y resultados

----- Autor -----

Diego García Álvarez
Sistemas Operativos 2 - Ingeniería de Computadores
Escuela Superior de Informática - Universidad de Castilla-La Mancha