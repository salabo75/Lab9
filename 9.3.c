#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WRITER 3
#define READER 5

int *global;
pthread_rwlock_t rwlock;


void* print(void* arg){
    int size = *(int*) arg;
    while(1){
        sleep(1);
	printf("\n");
        while(pthread_rwlock_rdlock(&rwlock)!=0);
	printf("??????: ");
        for (int i = 0; i < size; i++){
    	  printf("%d ", global[i]);
   	}  
    	printf("\n");
    	sleep(3);
        pthread_rwlock_unlock(&rwlock);	
     }
	pthread_exit(NULL);
}


void *thWrite(void *arg){
  int n = *(int*) arg;
  while (1){
    sleep(1);
    
    pthread_rwlock_wrlock(&rwlock);
    
    srand(time(NULL));
    int r = rand() % n;
    int value = rand() % 9 + 1;
    global[r] = value;
    printf("?????????? ???????? %d ? [%d] ??????? ???????.\n", global[r], r);  
    
    pthread_rwlock_unlock(&rwlock);
    sleep(3);
  }
}

void *thRead(void *arg){
  int n = *(int*) arg;
  while (1){
    sleep(1);
    
    pthread_rwlock_rdlock(&rwlock);
    srand(time(NULL));
    int r = rand() % n;
    printf("????????? ??????? ???????? [%d] : %d .\n", r, global[r]);
    global[r] = 0;  
    
    pthread_rwlock_unlock(&rwlock);
    sleep(3);
  } 
}

int main(int argc, char **argv){
  char *opts = "n:";
  int n = 10;
  int opt;
  while ((opt = getopt(argc, argv, opts)) != -1){
    switch(opt){
      case 'n':
        n = atoi(optarg);	//???-?? ????????? ? ???????
        break;
      default: {
        printf("??????????? ???????...\n");
        break;
      };     
    }
  }

  global = malloc(sizeof(int) * n);	//??????

  pthread_attr_t attr;			
  pthread_t pt;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //??? ??????? ?????? ? ????????????? ?????????
  pthread_create(&pt, &attr, &print, &n); 	
  pthread_attr_destroy(&attr);		
  
  pthread_t *writer, *reader; 
  writer = malloc(sizeof(pthread_t) * WRITER);
  reader = malloc(sizeof(pthread_t) * READER);

  pthread_rwlock_init(&rwlock, NULL); //????????????????? ?????????? ??????-??????

  for (int i = 0; i < WRITER; i++){
    if (pthread_create(&writer[i], NULL, &thWrite, &n) != 0) { //?????? ???????-?????????
        fprintf(stderr, "Error (thread)\n");
        return 1;
      }
  }

  for (int i = 0; i < READER; i++){
    if (pthread_create(&reader[i], NULL, &thRead, &n) != 0) { //?????? ???????-?????????
        fprintf(stderr, "Error (thread)\n");
        return 1;
      }
  }

  for(int i = 0; i < WRITER; ++i){
    if (pthread_join(writer[i], NULL) != 0) { //???????? ?????????? ??????
      printf("Error\n");    
    }
  }

  for(int i = 0; i < READER; ++i){
    if (pthread_join(reader[i], NULL) != 0) { //???????? ?????????? ??????
      printf("Error\n");   
    }
  }

  for (int i = 0; i < n; i++){	//????? ????? ???????
    printf("%d ", global[i]);
  }
  printf("\n");
  
  pthread_rwlock_destroy(&rwlock); //??????????? ??? ??????? ??????????? ???????
  free(global);
  free(writer);
  free(reader);
  return 0;
}
