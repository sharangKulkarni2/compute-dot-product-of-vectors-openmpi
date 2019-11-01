#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define SIZE 100000

int main(){

    int size, rank;
    MPI_Init(NULL, NULL);
    int *arr;
    int *arr1;
    int *data;
    int *data1;
    int i;
    int sz;
    int product;
    int dot_product;
    int color;
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm row_comm;
    if(rank==0)
        color =0;
    else 
        color =1;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &row_comm);// creating communicator for barrier

    if(rank == 0){
    	srand(0);
        dot_product = 0;
        product =0;
    	arr = (int*)calloc(SIZE, sizeof(int));
    	arr1= (int*)calloc(SIZE, sizeof(int));
    	for(i = 0; i < SIZE; i++){
    		arr[i] = (rand() % 10) + 1; 
    		arr1[i]= (rand() % 10) + 1;
            printf("%d %d\n",arr[i], arr1[i]);
    	}
    	sz = SIZE / size; // dividing elements among the processes.
    	data =(int*)calloc(sz, sizeof(int));
    	data1=(int*)calloc(sz, sizeof(int));
    	MPI_Scatter(arr,sz,MPI_INT,data,sz,MPI_INT,0,MPI_COMM_WORLD);
    	MPI_Scatter(arr1,sz,MPI_INT,data1,sz,MPI_INT,0,MPI_COMM_WORLD);
        for(i=0;i<sz;i++)
            product += data[i] * data1[i];
        MPI_Reduce(&product, &dot_product, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    	printf("dot product is %d", dot_product);
	}else{
		sz = SIZE / size;
		product = 0;
    	data = (int*)calloc(sz, sizeof(int));
    	data1 = (int*)calloc(sz, sizeof(int));
    	MPI_Scatter(arr,sz,MPI_INT,data,sz,MPI_INT,0,MPI_COMM_WORLD);
    	MPI_Scatter(arr1,sz,MPI_INT,data1,sz,MPI_INT,0,MPI_COMM_WORLD);
       /*
        for (i = 0; i < sz; ++i)
        {
            printf("rank : %d, first vector element:%d\n",rank, data[i]);
        }
         for (i = 0; i < sz; ++i)
        {
            printf("rank : %d, second vector element:%d\n",rank, data1[i]);
        }
        */
        for(i=0;i<sz;i++)
    		product += data[i] * data1[i];
        MPI_Barrier(row_comm);
        MPI_Reduce(&product, &dot_product, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        printf("\nmy rank :%d and product:%d\n",rank,product);

	}
    	 	
MPI_Finalize();
return 0;
}