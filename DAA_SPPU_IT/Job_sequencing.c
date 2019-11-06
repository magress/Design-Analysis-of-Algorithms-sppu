#include<stdio.h>
#include<stdlib.h>
#include<math.h>
typedef struct job
{
	int id,size;
	char name[100];
	int rt;
}job;

void swap(int* a, int* b) // for swapping
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

void heapify(int arr[], int n, int i, job *jb) 
{ 
    int pn = i; 
    int lc = 2*i + 1; 
    int rc = 2*i + 2; 

    if (lc < n && jb[arr[lc]].size > jb[arr[pn]].size) 
        pn = lc; 
  
    if (rc < n && jb[arr[rc]].size > jb[arr[pn]].size) 
        pn = rc; 
  
    if (pn != i) 
    { 
        swap(&arr[i], &arr[pn]); 

        heapify(arr, n, pn,jb); 
    } 
} 

void heapSort(int arr[], int n,job *jb) 
{ 
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i,jb); 
  
    for (int i=n-1; i>=0; i--) 
    { 
        swap(&arr[0], &arr[i]); 
        heapify(arr, i, 0,jb); 
    } 
} 

int main()
{
	int n,t,k=0,*temp,p;
	float sum,mrt=0;
	printf("\nEnter no_of_jobs : ");
	scanf("%d",&n);
	printf("\nEnter no_of_tapes : ");
	scanf("%d",&t);
	job *sol=(job *)calloc(n,sizeof(job)); //in sol array we store job id according to size
	
	for(int i=0;i<n;i++)
	{
		char c;
		sol[i].id=i+1;
		printf("\nEnter name of job %d: ",sol[i].id);
		c=getchar();
		scanf("%[^\t\n]s",sol[i].name);
		printf("\nEnter size of job %d: ",sol[i].id);
		scanf("%d",&sol[i].size);	
	}
	if(n<=t) //if jobs are less than total tapes
	{ 
		int *a;
		a=(int *)malloc(t*sizeof(int));
		for(int i=0;i<n;i++)
		{
			a[i]=sol[i].id;
			sol[i].rt=sol[i].size;
			sum+=sol[i].size;
		}
		
		for(int i=0;i<t;i++)
		{
			printf("\nJobid of tape %d: ",i+1);
			if(i<n)
				printf("%d\n",a[i]);
			else
				printf("--\n"); //when colums are empty
		}
		printf("\nJobid   Jobname Jobsize Job_Retrieval_time");
		for(int i=0;i<n;i++)
		{
			printf("\n%d\t%s\t%d\t %d\t",sol[i].id,sol[i].name,sol[i].size,sol[i].rt);
		}
		printf("\nAverage retrieval time is %.2f \n",sum/n);
		free(a);
	}
	else
	{
		temp=(int *)malloc(n*sizeof(int)); //we are sorting according to job id
		for(int i=0;i<n;i++)
			temp[i]=i;
		heapSort(temp,n,sol);
    		printf(" \n ");
		
		if(n%t==0)
			p=n/t; //we are calculating no. of columns so that we can know the size of matrix // row is no. of tapes
		else
			p=n/t+1;
		int *a;
		a=(int *)malloc(t*p*sizeof(int));
		for(int i=0;i<t;i++)
		{
			k=i;
			sum=0;
			for(int j=0;j<p;j++)
			{
				if(k<n)
				{
					*(a+i*p+j)=temp[k]; //in temp elements are in sorted order
					sum+=sol[*(a+i*p+j)].size; 
					sol[*(a+i*p+j)].rt=sum;
					mrt=mrt+sum;
						
				}
				k+=t; // as we store 2d matrix linearly so with this we directly reaches to next column of row 1
			}
				
		}
		k=0;
		for(int i=0;i<t;i++)
		{
			printf("\nJobid of tape %d : ",i+1);
			for(int j=0;j<p;j++)
			{
				if(*(a+i*p+j)==0&&k==0) 
				{
					printf("%d  ",*(a+i*p+j)+1);
					k++;
				}
				else if(*(a+i*p+j))
					printf("%d  ",*(a+i*p+j)+1);
				else
					printf("-  "); // for blank spaces 
			}
			printf("\n");
		}
		printf("\nJobid   Jobname Jobsize Job_Retrieval_time");
		for(int i=0;i<n;i++)
		{
			printf("\n%d\t%s\t%d\t %d\t",sol[i].id,sol[i].name,sol[i].size,sol[i].rt);
		}
		printf("\nMinimum avgerage retrieval time: %.2f\n",mrt/n);
		free(a);
	}	
	free(temp);
	free(sol);
	return 0;
}
