#include<stdio.h>
#include<stdlib.h>

int placequeen(int r2,int c2,int *p)
{
	int r1,c1;
	for(r1=0;r1<r2;r1++)
	{
		c1=p[r1];
		if(c1==c2 || abs(r1-r2)==abs(c1-c2)) //same column or diagonally adjacent
			return 0; //queen cannot be placed
	}
	return 1;
}

void print(int n,int *p)
{
	int i,j,k;
	for(i=0;i<=2*n+1;i++)
		printf("-");
	printf("\n");
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("|");
			if(j==p[i])
				printf("♛ ");
			else
				printf(" ");
	    }
		printf("|\n");
		for(k=0;k<=2*n+1;k++)
			printf("-");
		printf("\n");
	}
}

int iterative(int n,int sol)
{
	int *ptr;
	ptr=(int*)malloc(n*sizeof(int));
	int i,j,r=0,cnt=0,ch;
	
	for(i=0;i<n;i++)
		ptr[i]=-1;

	while(r!=-1)
	{
		ptr[r]++;
		if(ptr[r]<n)
		{
			if(placequeen(r,ptr[r],ptr))
			{
				if(r==n-1)
				{	
					printf("\nSoln. number %d for %d-Queen Problem - \n",++cnt,n);
					print(n,ptr);
					if(sol==1)
						return 0;
				}
				else
					r++;
			}
			
			
		}
		else
		{
			ptr[r]=-1;
			r--;
		}
	}
	free(ptr);
	return cnt;
}

void recursive(int n,int sol,int *ptr,int *p,int r)
{
	int i,j,c;
	static int d;
		
	for(c=0;c<n;c++)
	{
		if(placequeen(r,c,ptr))
		{
			ptr[r]=c;
			if(r==n-1)
			{
				(*p)++;
				printf("\nSoln. number %d for %d-Queen Problem - \n",*p,n);
				print(n,ptr);
				if(sol==1)
				{
					d=1;
					return;
				}
				printf("aas");
			}
			else
			{
				recursive(n,sol,ptr,p,r+1);
			}	
		}
		if(d==1)
			return;
	}
}

int main()
{
	int ch1,ch2,n,b,a;
	int *ptr;
	do
	{
		printf("\nN-Queen Problem using Backtracking...");
		printf("\n1.Iterative Solution...");
		printf("\n2.Recursive Solution...");
		printf("\n3.Exit");
		printf("\n\nEnter your choice: ");
		scanf("%d",&ch1);
		switch(ch1)
		{
			case 1:
				do
				{	
					printf("\nN-Queen Problem using Iterative Algorithm ...");
					printf("\n1.Print All Solutions..");
					printf("\n2.Print Single Solution..");
					printf("\n3.Return to main menu..");
					printf("\n\nEnter your choice: ");
					scanf("%d",&ch2);
					switch(ch2)
					{
						case 1:
							printf("\nEnter the number of queens: ");
							scanf("%d",&n);
							if(n<4)
								printf("\nSolution not possible!..");
							else
							{
								b=iterative(n,0);
								printf("\nThe total number of solutions in %d-Queen Problem is %d\n",n,b);
							}	
							break;
						case 2:
							printf("\nEnter the number of queens: ");
							scanf("%d",&n);
							if(n<4)
								printf("\nSolution not possible....");
							else
								iterative(n,1);
							break;
					}
				}while(ch2!=3);
				break;
			case 2:
				do
				{
					printf("\nN-Queen Problem using Recursive Algorithm ...");
					printf("\n1.Print All Solutions..");
					printf("\n2.Print Single Solution..");
					printf("\n3.Return to main menu..");
					printf("\n\nEnter your choice: ");
					scanf("%d",&ch2);
					switch(ch2)
					{
						case 1:
							printf("\nEnter the number of queens: ");
							scanf("%d",&n);
							if(n<4)
								printf("\nSolution not possible....");
							else
							{
								a=0;
								ptr=(int*)malloc(n*sizeof(int));
								recursive(n,0,ptr,&a,0);
								printf("\nThe total number of solutions in %d-Queen Problem is %d\n",n,a);
								free(ptr);
							}
							break;
						case 2:
							printf("\nEnter the number of queens: ");
							scanf("%d",&n);
							if(n<4)
								printf("\nSolution not possible....");
							else
							{
								a=0;
								ptr=(int*)malloc(n*sizeof(int));
								recursive(n,1,ptr,&a,0);
								free(ptr);
							}
							break;
					}
				}while(ch2!=3);
				break;
			case 3 :
				break;	
		}
	}while(ch1!=3);
}
