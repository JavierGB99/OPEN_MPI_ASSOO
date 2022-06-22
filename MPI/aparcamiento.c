#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void estadoParking(int* aparcamiento, int plazas){

	printf("Parking: ");
	int i;
	for(i=0;i<plazas;i++){
		printf("[%d] ",aparcamiento[i]);
	}
	printf("\n");
}

int main(int argc, char **argv){

	int rank, nprocs,plazas,plazas_libres,identificador;
	int esclavo[3] = {0,0,0};//id , entrar salir, plaza a ocupar si esta aparcados
	int informacion[3] ={0,0,-1};	
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	
	plazas = atoi(argv[1]);
	plazas_libres = plazas;
	int aparcamiento[plazas];
	
	int i;
	for(i=0;i<plazas;i++){
		aparcamiento[i]=0;//0 significa que no hay nadie ocupandola
	}
	
	while(1){
		//hay que esperar hasta que alguien quiera aparcar
		MPI_Recv(&esclavo,3,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		identificador = status.MPI_TAG;
		informacion[0]=esclavo[0];
		informacion[1]=esclavo[1];// Queremos saber si quiere entrar o salir
		if(esclavo[1]==0){//Quiere entrar
		
			if(identificador == 0){//es un coche	
			
				for(i =0;i<plazas;i++){
				
					if (aparcamiento[i]==0){
					
						aparcamiento[i]=esclavo[0];
						plazas_libres--;
						informacion[2]=i;			
						informacion[1]=1;	//plaza ocupada
						printf("ENTRADA: Coche %d aparca en %d. Plazas libres %d\n",esclavo[0],i,plazas_libres);
						estadoParking(aparcamiento,plazas);
						break;
					}
				}
				
				MPI_Send(&informacion,3,MPI_INT,informacion[0],0,MPI_COMM_WORLD);	
			
			}else{//camion
			
				for(i =0;i<plazas;i++){
				
					if (aparcamiento[i]==0 && aparcamiento[i+1]==0){
					
						aparcamiento[i]=esclavo[0];
						aparcamiento[i+1]=esclavo[0];
						plazas_libres=plazas_libres-2;
						informacion[2]=i;			
						informacion[1]=1;	//plaza ocupada
						printf("ENTRADA: Camion %d aparca en %d y %d. Plazas libres %d\n",esclavo[0],i,i+1,plazas_libres);
						estadoParking(aparcamiento,plazas);
						break;
					}
				}
				
				MPI_Send(&informacion,3,MPI_INT,informacion[0],1,MPI_COMM_WORLD);
			}
		
		}else{//Quiere salir
			
			if(identificador == 0){//es un coche	
			
				for(i=0;i<plazas;i++){
					if (i==esclavo[2]){
					
						aparcamiento[i]=0;
						plazas_libres++;			
						informacion[1]=0;	
						printf("Salida: Coche %d saliendo de la plaza %d. Plazas libres %d\n",esclavo[0],i,plazas_libres);
						estadoParking(aparcamiento,plazas);
						break;
					}
				}
				
				MPI_Send(&informacion,3,MPI_INT,informacion[0],0,MPI_COMM_WORLD);	
			}else{//camion
			
				for(i=0;i<plazas;i++){
				
					if (i==esclavo[2]){
					
						aparcamiento[i]=0;
						aparcamiento[i+1]=0;
						plazas_libres=plazas_libres+2;			
						informacion[1]=0;	
						printf("Salida: Camion %d saliendo de la plaza %d y %d. Plazas libres %d\n",esclavo[0],i,i+1,plazas_libres);
						estadoParking(aparcamiento,plazas);
						break;
					}
				}
				
				MPI_Send(&informacion,3,MPI_INT,informacion[0],1,MPI_COMM_WORLD);
			}
			
		}
	
	}
	
	MPI_Finalize();
	return 0;
}

