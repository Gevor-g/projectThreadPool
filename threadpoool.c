#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define THREAD_NUM 2
/*the project was intended to do operations with arrays, it sorts the half of the arrays in n^2, and makes all that
THREAD_NUM times times better 
half of the arrays are sorted, the other half computes the number of even numbers in it

*/
typedef struct Task {
    void (*taskFunction)(int*, int);
    int* arg1;
    int  arg2;
} Task;


int count = 1;
int arr[50][10];
Task taskQueue[256];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;


int zuyG(int* arr1,int n){
usleep(10000);
printf("\n");
count++;
int num = 0;
if(arr1 == NULL){
printf("%dth is nothing:(",count);
return num;
}

for(int i = 0; i < n ; i ++){
	if(arr1[i]%2 == 0){
		num ++;
	}
}
printf("%dth array as %d even nums",count,num);
return num;
}
void sort(int*  arr1, int n) {
    usleep(10000);
   printf("\n");

  
    count++;
    if(arr1 == NULL){
	    printf("%dth array  nothing",count);
	   
      return ;
    }
    for(int i = 0; i < n; i ++){
       for(int j = 0; j < n-1; j ++){
              
               if(arr1[j] > arr1[j+1]){
			int t = arr1[j];
			arr1[j] = arr1[j+1];
			arr1[j+1] = t;
		}
	
         }

    }
    for(int i = 0; i < n; i ++){
        if(arr1[i] == NULL){
		printf("%dth array  nothing",count);
          
		return;
	}
    }
    printf("%dth array ",count );
    printf("sorted! ");
    for(int i = 0; i < n; i ++){
	   	    
      printf("%d ",arr1[i]);
    }

}


void executeTask(Task* task){

    task->taskFunction(task->arg1, task->arg2);
    
}

void submitTask(Task task) {
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

void* startThread(void* args) {
    while (1) {
        Task task;

        pthread_mutex_lock(&mutexQueue);
        while (taskCount == 0) {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        task = taskQueue[0];
        int i;
        for (i = 0; i < taskCount - 1; i++) {
            taskQueue[i] = taskQueue[i + 1];
        }
        taskCount--;
        pthread_mutex_unlock(&mutexQueue);
        executeTask(&task);
    }
}

int main(int argc, char* argv[]) {
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], NULL, &startThread, NULL) != 0) {
            perror("Failed to create the thread");
        }
    }
  int n = 10;
    srand(time(NULL));
    for (i = 0; i < 50; i++) {
	    for(int j = 0; j < 10; j ++){
             arr[i][j] = rand()%100;
                  
	    }
        Task t = {
            .taskFunction = i%2 == 0 ? &sort : &zuyG,
	    .arg1 = arr[i],
            .arg2 = n
        };
        submitTask(t);
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join the thread");
        }
    }
    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);
    return 0;
}
