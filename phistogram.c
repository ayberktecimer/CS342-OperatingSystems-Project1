#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<sys/wait.h> 
#include<unistd.h> 
#include <unistd.h> 
#include <string.h>
#include <float.h>
#include <math.h>
#include <sys/time.h>

struct timeval begin, end;	
long time_elapsed = 0;

int main(int argc, char *argv[])
{
	gettimeofday(&begin, NULL);

	double minvalue = atof(argv[1]);
	double maxvalue = atof(argv[2]);
	int bincount = atoi(argv[3]);
	int N = atoi(argv[4]);

	
	/*
		arg[0] = ./xxxx
		arg[1] = minvalue
		arg[2] = maxvalue
		arg[3] = bincount
		arg[4] = N
		arg[5...] = file1 .. filen 
	*/
	
	FILE *fp;

	char* input_holder[N];
	for(int i=5 ; i< N+5;i++){
		input_holder[i-5] = argv[i];
	}
	int outfile_array[bincount];
	memset(outfile_array, 0, sizeof outfile_array);
	//printf("The contents of %s file are:\n", file1);
	/*for(int i =0 ; i< bincount; i++){
		printf("out file elementleri: %d\n", outfile_array[i]);
	}*/

	int* all_files[N];

	double w = (maxvalue - minvalue) / bincount ;

	pid_t  n= 0; // stores process id

	

	for(int q=0 ; q< N;q++){

		n = fork();
		//printf("n is: %d\n", n);
		if (n < 0) {
			fprintf(stderr, "Fork Failed");
			exit(-1);
		}
	else if (n == 0) { /* child process*/
		fp = fopen(input_holder[q], "r"); // read mode
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

			//printf("temp_value : %f\n\n", temp_value );
			for (int i = 0 ; i < bincount ; i++ ){
				
				if((i*w) + minvalue <= temp_value && temp_value < ((i+1)*w) + minvalue){
					num_of_values_bin[i]= num_of_values_bin[i] + 1;
				}
			}
			if(fabs(temp_value- maxvalue) < FLT_MIN){
				num_of_values_bin[bincount-1]= num_of_values_bin[bincount-1] + 1;
			}
		//execlp("/bin/ls", "ls", NULL);
		}
		//printf("k is: %d\n", k);
		all_files[q]= num_of_values_bin;
		//printf("%d\n", all_files[q][0]);
		char int_file_name[100] = "int" ;
		char t[10] ;
		sprintf(t, "%d", q);

		strcat(int_file_name, t);
		
		FILE *fp_int = fopen(int_file_name, "w");
		for (int i =0 ; i< bincount; i++){
			//printf("Number of values in input %d: %d\n",q ,all_files[q][i]);
			fprintf(fp_int,"%d\n", all_files[q][i]);
			//outfile_array[i] = outfile_array[i] + all_files[q][i];
		}
		

		fclose(fp);
		fclose(fp_int);
		exit(0);
	}
	//exit(0);
}

for(int i=0; i< N;i++){
	wait (NULL);
}



int holder_for_out_file=0;
FILE *fp_out = fopen(argv[argc-1], "w");
for(int i=0; i< N; i++){
	char int_file_name[100] = "int" ;
	char t[10] ;
	sprintf(t, "%d", i);

	strcat(int_file_name, t);
	holder_for_out_file=0;
		fp = fopen(int_file_name, "r"); // read mode
		//printf("filename : %s\n", int_file_name);
		char temp[100];
		while(fgets (temp, sizeof(temp), fp)!=NULL ){
			//printf("%d\n", atoi(temp));
			outfile_array[holder_for_out_file] += atoi(temp);
			holder_for_out_file ++;
		}
		fclose(fp);
	}
	for(int qq = 0 ; qq < bincount ; qq++){
		fprintf(fp_out, "bin %d: %d\n", (qq+1),outfile_array[qq]);
		//printf("outfile degerleri : %d \n", outfile_array[qq]);
		//fprintf(fp_out, "%d : %d\n", qq,outfile_array[qq]);
	}
	//phistogram(3,49,5,2,f1,f2);

	gettimeofday(&end, NULL);
	time_elapsed = (end.tv_sec - begin.tv_sec)*1000000 + 
	((end.tv_usec - begin.tv_usec));
	printf("elapsed time in microsecond : %ld \n", time_elapsed);
	return 0;
}

