#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

static double result = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct threadArgs{
	double a;
	double b;
};

void* func(void* arg){
	struct threadArgs args = *(struct threadArgs*) arg;
	pthread_mutex_lock(&mutex);
	result+=(4-powl(args.a + ((args.b - args.a)/2), 2));
	pthread_mutex_unlock(&mutex);
	return 0;
}

int main(int argc, char **argv){
	int p = 5; //??????????? ?????????? ???????
	double a = 0;
	double b = 2;
	int n;
	double h;
	pthread_t *pt;
	struct threadArgs args;
	args.a = a;
	
	if (argc > 1){
		p = atoi(argv[1]);
	}

	pt = (pthread_t*) malloc(sizeof(pthread_t) *p);
	
	h = (b-a)/p;
	args.b = args.a + h;
	
	for(int i = 0; i < p; i++){
		
		if(pthread_create(&pt[i], NULL, &func, &args) != 0){
			fprintf(stderr, "Error thread %d\n", i);
			return 1;
		}
		
		if(pthread_join(pt[i], NULL) != 0){		//???????? ?????????? ??????
			fprintf(stderr, "Error %d\n", i);
			return 1;
		}
		args.a = args.b;
		args.b += h;
	}
	
	result*=h;
	
	printf("???????? ???????? ?????????: %f\n", result);
	pthread_mutex_destroy(&mutex);
	free(pt);
	return 0;
}
