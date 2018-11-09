#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "math.h"

void print_id(){
	pid_t pid = getpid();
	pid_t ppid = getppid();
	printf("pid  = %d\tppid = %d\n", pid, ppid);
}

int main(int argc, char const *argv[]){
	int k, n;
	if(argc > 1){
		printf("Num of proc: ");
		int ret = scanf("%d", &k);
		if(ret != 1){
			printf("Bad number!\n");
			return 1;
		}
		printf("Num addings: ");
		ret = scanf("%d", &n);
		if(ret != 1){
			printf("Bad number!\n");
			return 1;
		}
	}
	else{
		FILE *f = fopen("input", "r");
		int ret = fscanf(f, "%d\n%d", &k, &n);
		fclose(f);
		if(ret < 2){
			printf("file error\n");
			return 1;
		}
	}
	time_t start = clock();
	int* pipes = (int*)malloc(k*sizeof(int)*2);
	// printf("%d\t%d\n", n, k);
	int pid;
	long double result = 0.0;
	for(int i = 0; i < k; ++i){
		int *cur_pipe = &pipes[2*i];
		if(pipe(cur_pipe) < 0){
			printf("pipe error\n");
			return 1;
		}

		// printf("i: %d\n", i);
		if(!fork()){
			int j = i + 1;
			while(j < n){
				result += ((long double)pow(-1,j+1)*4.0)/((long double)j*2-1.0);
				j += k;
			}
			// print_id();
			// printf("writed: %Lf\n", result);
			close(cur_pipe[0]);
			write(cur_pipe[1], (const void*)&result, sizeof(result));
			//write result
			return 0;
		}
	}
	long double *buf = (long double *)malloc(sizeof(long double));
	for(int i = 0; i < k; ++i){
		close(pipes[2*i+1]);
		read(pipes[2*i], buf, sizeof(*buf));
		// printf("readed: %Lf\n", *buf);
		result += *buf;
		wait(NULL);
	}
	time_t end = clock();
	printf("Num of proc   : %d\n", k);
	printf("Num of addings: %d\n", n);
	printf("Pi: %Lf\n", result);
	// printf("time: %f sec\n", ((double)(end - start))/CLOCKS_PER_SEC);
	// printf("%d\n", CLOCKS_PER_SEC);
	return 0;
}
