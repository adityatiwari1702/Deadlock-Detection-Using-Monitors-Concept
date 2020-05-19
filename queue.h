static int MAX_SIZE;
void initialize_max(int x){
	MAX_SIZE=x;
}
int CQ_ENQUEUE(int CQ[],int VAL,int *REAR,int *FRONT)
{
	if((*REAR == MAX_SIZE-1 && *FRONT == 0)||(*REAR == (*FRONT -1)))
	{
		printf("\nQueue Full.");
		return -1;
	}
	if(*REAR == MAX_SIZE-1)
		*REAR =0;
	else
		*REAR = *REAR +1;
	if((*FRONT) == *REAR)
	{
		printf("\nQueue Full.");
		return 0;
	}
	CQ[*REAR]=VAL;
	if(*FRONT == -1)
		*FRONT =0;
	return 0;
}
int CQ_DEQUEUE(int CQ[],int *REAR,int *FRONT)
{
	if(*FRONT == -1)
	{
		printf("\nQueue is Empty.");
		return -1;
	}
	int TEMP = CQ[*FRONT];
	if(*FRONT == *REAR)
	{
		*FRONT= -1;
		*REAR = -1;
		return TEMP;
	}
	if(*FRONT == MAX_SIZE-1)
	{
		*FRONT = 0;
	}
	else
	{
		*FRONT=*FRONT+1;
	}
	return TEMP;
}
