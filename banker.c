#include"queue.h"
struct resources{
		int *FINISH;
		int **alloc_resource;
		int **max_resource;
		int **req_resource;
		int **need;
		int *avail;
		int *sequence;
};
typedef struct resources * resource_t;

pthread_mutex_t mutex;
static int process;
static int resources;
static pthread_t *th_id;
/*Method to Initialize Global Variables*/
void resource_t_init(int x,int y,pthread_t *z){
	process=x;
	resources=y;
	th_id=z;
	pthread_mutex_init(&mutex,NULL);
}
/*int **resource_array_create(int process,int resources) ---> It returns 2D array of size [process][resources]
																by takinf input from user.*/
int **resource_array_create(int process,int resources)
{
	int **resource,i,j;
	resource=(int **)malloc(process*sizeof(int *));
	for(i=0;i<process;i++)
		resource[i]=(int *)malloc(resources*sizeof(int));
	printf("\nProcess");
	for(i=0;i<resources;i++)
		printf("\t%d",i);
	printf("\n");
	for(i=0;i<process;i++)
	{
		printf("P%d\t",i);
		for(j=0;j<resources;j++)
		{
				scanf("%d",&resource[i][j]);
		}
	}
	return resource;
}
/*int *avail_array_create(int resources) ----> It returns Available Resources Array*/
int *avail_array_create(int resources)
{
	int i;
	int *avail=(int *)malloc(resources*sizeof(int));
	for(i=0;i<resources;i++)
		printf("\t%d",i);
	printf("\nAVAIL\t");
	for(i=0;i<resources;i++)
	{
		scanf("%d",&avail[i]);
	}	
	return avail;
}
/*int *array_initialize(int size,int val)---->It returns Array of size and inilization with val*/
int *array_initialize(int size,int val)
{
	int i;
	int *x=(int *)malloc(size*sizeof(int));
	for(i=0;i<size;i++){
		x[i]=val;
	}
	return x;
}
/*void add(int *work,int *allocation) ----->This Funcn Adds Allocated Resources in Work Resources*/
void add(int *work,int *allocation)
{
	int i;
	for(i=0;i<resources;i++)
		work[i]+=allocation[i];
}
/*void intialize(int *temp,int **xyz,int j)---->This Funcn Initlialize temp array with the row of 2D array of xyz &
												row is passed by j.*/
void intialize(int *temp,int **xyz,int j)
{
	int i;
	for(i=0;i<resources;i++)
		temp[i]=xyz[j][i];
}
/*int satisfy_need(int *need,int *work)---->This Function return 1 is need matrix is less than Work matrix,
											if fails return -1.*/
int satisfy_need(int *need,int *work)
{
	int i;
	int temp=-1;
	for(i=0;i<resources;i++)
	{
		if(need[i]<=work[i])
			temp=1;
		else
		{
			temp=-1;
			break;
		}
	}
	return temp;
}
/*
*** Info About Vaiables Used in safe_sequence:-
	i ----> It is Index of Currently Checking Process for Safe Sequence
	process_entered ----> It is to Check how many Process are entered & if All process are entered then Queue must be Entertained then.
	queue_entertained ----> To keep track of how many processes from queue entertained
	queue_len ----> To  store Length of Queue
	temp ---->It is a Variable to check the Finish array is Compeletly True or Not
	first ----->This is Variable to Entertain Queue of Susended Processes.
*/	
int *safe_sequence(int **need,int *AVAIL,int **alloc_resource,int *FINISH){
	int *WORK=AVAIL;
	int *NEED=array_initialize(resources,0);
	int *allocation=array_initialize(resources,0);
	int i=0,j=0,k=0,process_entered=0,queue_entertained=0,queue_len=0,temp=0,first=0;
	/*Queue Creation(CQ)*/
	int *queue=array_initialize(process,0);
	int REAR=-1;
	int FRONT=-1;
	initialize_max(process);	/*Initializing Queue Limit*/
	int *sequence=array_initialize(process,-1);
	do{
		if(process_entered==process){
			if(first<queue_len){
				i=CQ_DEQUEUE(queue,&REAR,&FRONT);
				if(i==-1)
					break;
				first+=1;
			}
			else if(queue_entertained<queue_len && queue_entertained!=0){
				first=1;/*Why i set First to 1??*/
				i=CQ_DEQUEUE(queue,&REAR,&FRONT);
				if(i==-1)
					break;
			}
			else{
				printf("\nSystem is in Deadlock State.");
				return NULL;
			}
		}
		intialize(NEED,need,i);
		if(FINISH[i]==0&&satisfy_need(NEED,WORK)==1){	
			intialize(allocation,alloc_resource,i);
			add(WORK,allocation);
			FINISH[i]=1;
			sequence[j++]=i;
			if(process_entered==process){
				queue_entertained++;
			}
		}
		else if(FINISH[i]==0){
			if(CQ_ENQUEUE(queue,i,&REAR,&FRONT)==-1)
				break;
			if(process_entered!=process)
				queue_len++;
		}
		if(process_entered<process){
			process_entered++;
		}
		if(process_entered==process){
			for(k=0;k<process;k++){
				if(FINISH[k]==1)
					temp=1;
				else{
					temp=0;
					break;
				}
			}
			if(temp==1)
				break;
		}
		i++;
	}while(1);
	return sequence;
}
/*void copy(int **x,int **y,int a,int b)----> It copies y matrix in x size is given by a and b*/
void copy(int **x,int **y,int a,int b){
	int i,j;
	for(i=0;i<a;i++)
		for(j=0;j<b;j++)
			x[i][j]=y[i][j];
}
/*int **array_create(int a,int b)-----> It return 2d Array of size [a][b]*/
int **array_create(int a,int b){
	int **x,i;
	x=(int **)malloc(a*sizeof(int *));
	for(i=0;i<a;i++)
		x[i]=(int *)malloc(b*sizeof(int));
	return x;
}
/*resource_t create_copy(resource_t x)---->It create copy of x and return copied m*/
resource_t create_copy(resource_t x)
{
	resource_t m=(struct resources *)malloc(sizeof(struct resources));
	m->FINISH=array_initialize(process,0);
	m->alloc_resource=array_create(process,resources);
	copy(m->alloc_resource,x->alloc_resource,process,resources);
	m->max_resource=array_create(process,resources);
	copy(m->max_resource,x->max_resource,process,resources);
	m->req_resource=array_create(process,resources);
	copy(m->req_resource,x->req_resource,process,resources);
	m->need=array_create(process,resources);
	copy(m->need,x->need,process,resources);
	m->avail=(int *)malloc(resources*sizeof(int ));
	int i;
	for(i=0;i<resources;i++)
		m->avail[i]=x->avail[i];
	return m;
}

resource_t intialize_resource(){
	int i,j;
	resource_t x=(struct resources *)malloc(sizeof(struct resources));
	/*Initializing FINISH Array to 0 i.e 0-false 1-true*/
	x->FINISH=array_initialize(process,0);
	/*Assigning Allocation Matrix*/
	printf("Enter the Allocated resources for process:-");
	x->alloc_resource=resource_array_create(process,resources);
	/*Assigning Maxmimum Matrix*/
	printf("Enter the Maximum resources for process:-");
	x->max_resource=resource_array_create(process,resources);
	/*Assigning Request Matrix*/
	printf("Enter the Request resources for process:-");
	x->req_resource=resource_array_create(process,resources);
	/*Calulating Need Matrix*/
	x->need=(int **)malloc(process*sizeof(int *));
	for(i=0;i<process;i++)
		x->need[i]=(int *)malloc(resources*sizeof(int));
	for(i=0;i<process;i++)
		for(j=0;j<resources;j++)
			x->need[i][j]=(x->max_resource[i][j]-x->alloc_resource[i][j]);
	/*Assigning Available Matrix*/
	printf("Enter the Available Resources To System:-\n");
	x->avail=avail_array_create(resources);
	x->sequence=array_initialize(process,-1);
	resource_t a=create_copy(x);
	a->sequence=safe_sequence(a->need,a->avail,a->alloc_resource,a->FINISH);
	if(a->sequence!=NULL){
		printf("\nInitially Safe Sequence is:-\n");
	for(i=0;i<process;i++){
		x->sequence[i]=a->sequence[i];
		printf("P%d\t",x->sequence[i]);
		}
	}
	return x;
}

void subtract(int *work,int *allocation)
{
	int i;
	for(i=0;i<resources;i++)
		work[i]-=allocation[i];
}

int compare_seq(int *x,int *y){
	int i,temp=-1;
	for(i=0;i<process;i++){
		if(y[i]==x[i])
			temp=0;
		else{
			temp=-1;
			break;
		}
	}
	return temp;	
}

void put(int *temp,int **xyz,int j)
{
	int i;
	for(i=0;i<resources;i++)
		xyz[j][i]=temp[i];
}

void print_seq(int *x,int size)
{
	int i;
	for(i=0;i<size;i++){
		if(x[i]!=-1)
			printf("%2d",x[i]);
	}
}
int check_request(int *request){
	int i,temp=-1;
	for(i=0;i<resources;i++){
		if(request[i]==0)
			temp=1;
		else{
			temp=-1;
			break;
		}
	}
	return temp;
}
void *resource_request(void *input)
{
	pthread_mutex_lock(&mutex);
	int i,x;
	for(i=0;i<process;i++)
	{
		if(pthread_equal(pthread_self(),th_id[i])!=0){
			x=i;
			break;
		}
	}
	int *request=array_initialize(resources,0);
	int *NEED=array_initialize(resources,0);
	intialize(request,((struct resources *)input)->req_resource,x);
	if(check_request(request)==1){
		pthread_mutex_unlock(&mutex);
		return NULL;
	}
	intialize(NEED,((struct resources *)input)->need,x);
	if(satisfy_need(request,NEED)==1)
	{
		if(satisfy_need(request,((struct resources *)input)->avail)==1)
		{
			resource_t b=create_copy((struct resources *)input);
			subtract(b->avail,request);
			int *temp=(int *)malloc(resources*sizeof(int));
			for(i=0;i<resources;i++)
				temp[i]=b->avail[i];
			int *allocation=array_initialize(resources,0);
			intialize(allocation,b->alloc_resource,x);
			add(allocation,request);
			put(allocation,b->alloc_resource,x);
			subtract(NEED,request);
			put(NEED,b->need,x);
			b->sequence=safe_sequence(b->need,b->avail,b->alloc_resource,b->FINISH);
			if(b->sequence!=NULL){
				if(compare_seq(b->sequence,((struct resources *)input)->sequence)==-1){
					printf("P%d",x);
					printf("\t");
					print_seq(request,resources);
					printf("\t\tN\tUnsafe");
					printf("\t");
					print_seq(((struct resources *)input)->avail,resources);
					printf("\t     Sequence After Granting Request is ");
					print_seq(b->sequence,process);
					printf(".\n");
				}
				else{
					printf("P%d",x);
					printf("\t");
					print_seq(request,resources);
					printf("\t\tN\t  Safe");
					copy(((struct resources *)input)->alloc_resource,b->alloc_resource,process,resources);
					copy(((struct resources *)input)->need,b->need,process,resources);
					for(i=0;i<resources;i++)
						((struct resources *)input)->avail[i]=temp[i];
					printf("\t");
					print_seq(((struct resources *)input)->avail,resources);
					printf("\t     Resource Granted Immediately\n");
				}
			}
		}
		else
		{
			printf("\nResources Not available.");
		}
	}
	else
	{
		printf("\nRequest Can't be Satisfied.");
	}
	pthread_mutex_unlock(&mutex);
}