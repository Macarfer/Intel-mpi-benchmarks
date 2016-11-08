#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include "IMB_settings.h"

#ifndef MSGSPERSAMPLE
	#define MSGSPERSAMPLE 1000
#endif
#ifndef OVERALL_VOL
	#define OVERALL_VOL 4194304
#endif
#ifndef N_WARMUP
	#define N_WARMUP 2
#endif

int main(int argc, char *argv[])
{
	/*Initialize mpi environment*/
	int rank=0,size;
	//MPI_BYTE  message;
	double inicio,total;
	int i,actual=0,X=sizeof(MPI_BYTE),n_barr;
	int n_sample = MSGSPERSAMPLE;
    //printf("Tamano de n_sample-> %d\n",n_sample);
	
    //Inicio o lugar de comunicacion entre os procesos
	MPI_Init(&argc,&argv);
		//printf("Initial time %ld\n",MPI_Wtime());
        //Obtenho o rango de cada un dos procesos
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //Establezo unha barreira para que todos comecen de forma simultanea a comunicacion
	MPI_Comm_size(MPI_COMM_WORLD,&n_barr);


    MPI_Comm_size (MPI_COMM_WORLD, &size);    /* get number of processes */


          /*
                WARMUP!!!
        */
	

	for(i=0;i<N_WARMUP;i++){
		MPI_Barrier( MPI_COMM_WORLD);
	}

	X=0;

	if(!rank)
		printf("#bytes\t#repetitions\tt[μsec]\n");

	for(;X<=OVERALL_VOL && n_sample>=10;){ 
		i=0;
		MPI_Barrier(MPI_COMM_WORLD);

		inicio=MPI_Wtime();

		for(;i<n_sample;i++){
			MPI_Barrier( MPI_COMM_WORLD);
		}

		total=MPI_Wtime();
        total=((total-inicio));///n_sample);

        if(!rank)
        	printf("%d\t%d\t%f\t\n",X,n_sample,total*1000000);

        if(X==0)
        	X=1;
        else
        	X*=2;

        n_sample=fmax(1,fmin(MSGSPERSAMPLE,OVERALL_VOL/X));

    }
    MPI_Finalize();
    return 0;
}
