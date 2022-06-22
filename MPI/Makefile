#
# Makefile
# 	Makefile global del sistema
#
all: aparcamiento coche camion

aparcamiento: aparcamiento.c

	mpicc aparcamiento.c -o aparcamiento

coche: coche.c

	mpicc coche.c -o coche
	
camion: camion.c

	mpicc camion.c -o camion

run: aparcamiento coche camion

	mpirun --hostfile localhost -np 1 aparcamiento 5 : -np 8 coche : -np 4 camion

clean:
	rm aparcamiento 
	rm coche
	rm camion
