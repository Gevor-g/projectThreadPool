int count;
int arr[50][10];
Task taskQueue[256];
int taskCount;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;


int zuyG(int* arr1,int n);
void sort(int*  arr1, int n);

void executeTask (Task* task);

void submitTask(Task task) ;
void* startThread(void* args);
