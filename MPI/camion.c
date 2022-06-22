#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int id, nprocs;
	int aparcado = 0;
	int informacion[3];//id y si quiere entrar y salir y plaza ocupada
	int respuesta[3];//en la primera posicion ira la id del coche y en la segunda ira si se ha aparcado o no 0 = no aparacado y 1= se ha aparcado y la plaza en la que se ha aparcado
	
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	
	while(1){
		
		informacion[0]=id;
		informacion[1]=0;//Entrar = 0 porque quiere aparcar y salir = 1 porque ya ha aparcado
		informacion[2]=-1;//ninguna plaza ocupada
		
		do{
			MPI_Send(&informacion,3,MPI_INT,0,1,MPI_COMM_WORLD);//el segundo 0 es la TAG que 0 significa coche y 1 camion
			MPI_Recv(&respuesta,3,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
			aparcado = respuesta[1];
		
		}while(aparcado==0);
		
		sleep(rand()%21);//Dormir entre 0 y 20
		
		
		MPI_Send(&respuesta,3,MPI_INT,0,1,MPI_COMM_WORLD);
		MPI_Recv(&respuesta,3,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);			
		
	}
	MPI_Finalize();
	return 0;
}
