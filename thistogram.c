
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h> 
#include<sys/wait.h> 
#include<unistd.h> 
#include <string.h>
#include <float.h>
#include <math.h>
#include <sys/time.h>

void *thistogram (void *param);   /*  thread start function */

double minvalue;
double maxvalue;
int bincount;
int N;
double w ;

int * input_ptr;
/*
arg[0] = ./xxxx
arg[1] = minvalue
arg[2] = maxvalue
arg[3] = bincount
arg[4] = N
arg[5...] = file1 .. filen 
*/

struct param {
	int index;
	char* n;
};



struct timeval begin, end;	

long time_elapsed = 0;

int  main(int argc, char *argv[]){
	gettimeofday(&begin, NULL);

	minvalue = atof(argv[1]);
	maxvalue = atof(argv[2]);
	bincount = atoi(argv[3]);
	N = atoi(argv[4]);
	pthread_t tid[N]; /* id of the created thread */

	//pthread_attr_t attr[argc];  /* set of thread attributes */
	w = (maxvalue - minvalue) / bincount ;

	int input_array[N][bincount];
	memset(input_array, 0, sizeof input_array);
//	int input_array[N][bincount];
//	memset(input_array, 0, sizeof(input_array[0][0]) * N * bincount);
	input_ptr = &input_array[0][0];


	int out_array[bincount];
	memset(out_array, 0, sizeof out_array);

	struct param params[N];


	for(int i = 5; i< N+5 ; i++){
		params[i-5].index = i-5;
		params[i-5].n= argv[i];
		int rc= pthread_create (&tid[i-5], NULL, thistogram, &params[i-5]);

		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for (int i = 5; i < N+5; i++)
	{
		pthread_join (tid[i-5], NULL); 
	}



	for (int i = 0; i < bincount; i++)
	{
		for(int q = 0; q < N; q++)
		{
			
			out_array[i] +=  input_array[q][i];
			
		}
	}

	/*time_elapsed = (end.tv_sec - begin.tv_sec)*1000000 + 
	((end.tv_usec - begin.tv_usec));*/

	//printf("elapsed time in microsecond : %ld \n", time_elapsed);

	FILE *fp_out = fopen(argv[argc-1], "w");
	for(int qq = 0 ; qq < bincount ; qq++){
		fprintf(fp_out, "bin %d : %d\n", (qq+1),out_array[qq]);
	}
	fclose(fp_out);
	gettimeofday(&end, NULL);
	time_elapsed = (end.tv_sec - begin.tv_sec)*1000000 + 
	((end.tv_usec - begin.tv_usec));
	printf("elapsed time in microsecond : %ld \n", time_elapsed);
}

void *thistogram (void *st)
{
	struct param *pt = (struct param *)st;
	FILE *fp = fopen(pt->n, "r");
	//int i; 
	//int upper; 
	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	int num_of_values_bin[bincount];
	memset(num_of_values_bin, 0, sizeof num_of_values_bin);
		//int *num_of_values_bin = NULL;
		//num_of_values_bin = malloc(sizeof(int)*bincount);
	char temp[100];

	while(fgets (temp, sizeof(temp), fp)!=NULL )
	{
		double temp_value =  atof(temp);

		//printf("temp_value : %f maxvalue: %f\n", temp_value, maxvalue );
		for (int i = 0 ; i < bincount ; i++ ){

			if((i*w) + minvalue <= temp_value && temp_value < ((i+1)*w) + minvalue){
				num_of_values_bin[i]= num_of_values_bin[i] + 1;
				//*(input_ptr + (i*bincount) + i)
				// input_array[pt->index][i]
				*(input_ptr + (pt->index * bincount) + i) = num_of_values_bin[i];
			}
		}
		if(fabs(temp_value- maxvalue) < FLT_MIN){
			num_of_values_bin[bincount-1]= num_of_values_bin[bincount-1] + 1;
			*(input_ptr + (pt->index * bincount) + (bincount-1)) = num_of_values_bin[bincount-1];
		}
		//execlp("/bin/ls", "ls", NULL);
	}
	fclose(fp);

	pthread_exit(0); 
}