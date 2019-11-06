//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//structure of node
typedef struct node
{
	int nodeno;
	int levelno;
	int city;
	
	int *RCM;
	int lb;//lower bound
	
	struct node *parent;
	
}node;

//to deal with list of live and dead nodes
typedef struct list
{
	node *info;
	struct list *next;
	
}list;

//before calculating the RCM of child just copying the RCM of parent into child
void initialise_RCM(int *CRCM,int *PRCM,int ncities)
{
	int i,j;
	for(i=0;i<ncities;i++)
		for(j=0;j<ncities;j++)
			*(CRCM+i*ncities+j)=*(PRCM+i*ncities+j);
}

//calculating the cost related to the reduction of that particular node or lower bound
int reduced_cost(int *RCM,int ncities)
{
	int i,j;//loop control
	int cost=0;//lower bound
	
	for(i=0;i<ncities;i++)//traversing row by row
	{
		int mini=INT_MAX;
		for(j=0;j<ncities;j++)
		{
			if(mini>*(RCM+i*ncities+j))
				mini=*(RCM+i*ncities+j);
		}
		if(mini!=INT_MAX && mini!=0)
		{
			cost+=mini;
			for(j=0;j<ncities;j++)
			{
				if(*(RCM+i*ncities+j)!=INT_MAX)
					*(RCM+i*ncities+j)-=mini;
			}
		}
	}
	
	for(i=0;i<ncities;i++)//traversing column by column
	{
		int mini=INT_MAX;
		for(j=0;j<ncities;j++)
		{
			if(mini>*(RCM+i+j*ncities))
				mini=*(RCM+i+j*ncities);
		}
		if(mini!=INT_MAX && mini!=0)
		{
			cost+=mini;
			for(j=0;j<ncities;j++)
			{
				if(*(RCM+i+j*ncities)!=INT_MAX)
					*(RCM+i+j*ncities)-=mini;
			}
		}
	}
	
	return cost;
}

//checking whether the same node is not comming in the path 
int not_found(int *path,int c,int v)
{
	int i;
	for(i=1;i<=v;i++)
		if(*(path+i)==c)
			return 0;
	return 1;
}

//for making row col in an entry infinity
void initialise_infi(int *RCM,int p,int c,int ncities)
{
	int i;
	for(i=0;i<ncities;i++)
	{
		*(RCM+p*ncities+i)=INT_MAX;
		*(RCM+i*ncities+c)=INT_MAX;
	}
	*(RCM+c*ncities)=INT_MAX;
}

//for creating list node
list *newnode(node *d)
{
	list *temp=(list *)malloc(sizeof(list));
	temp->info=d;
	temp->next=NULL;
	return temp;
}

//for adding element to list of live nodes
list *enqueue(list *l,node *d)
{
	list *start=l;
	list *temp=newnode(d);
	if(l==NULL || l->info->lb>d->lb)
	{
		temp->next=l;
		l=temp;
	}
	else
	{
		while(start->next!=NULL && start->info->lb<d->lb)
		{
			start=start->next;
		}
		temp->next=start->next;
		start->next=temp;
	}
	
	return l;
}

//deleting a node from the list of live nodes
node *dequeue(list *l)
{
	list *temp=l;
	l=l->next;
	node *t=temp->info;
	free(temp);
	return t;
}

void tsp(node *R,int *adjm,int v)
{
	node *E;//for dealing with E-Node
	E=R;//root is made E-Nodesol
	
	int upper=INT_MAX;//for handling the upper bound
	
	int i,j,l,c;
	
	node *T;//a temperary variable used for dealing with solution vection 
	
	node *ans=NULL;//for storing the address of answer node
	
	int *soln=(int *)malloc((v-1)*sizeof(int));//for dealing with solution vector
	
	list *live_list=NULL;//for dealing with list of live nodes
	list *dead_list=NULL;//for dealing with list of dead nodes
	list *ll=NULL;
	
	*soln=0;
	int count = 2;
	
	while(1)
	{
		l = E->levelno;
		j=l;
		T = E;
		
		while(T!=R)//deals with solution vector
		{
			soln[j]=T->city;
			T=T->parent;
			j--;
		}
		
		for(c=1;c<v;c++)
		{
		
			int p = E->city;
			
			if(*(adjm+p*v+c)!=INT_MAX)//checking connectivity between parent and child node
			{
				if(not_found(soln,c,l)) //checking for duplicacy of nodes in the solution, returns 1 if not found 
				{
					if(l<(v-2) || (l==(v-2) && *(adjm+c*v)!=INT_MAX)) /*checking if c is any other node 
							or c is the answer node and there is an edge between c and root */
					{

						node *child = (node *)malloc(sizeof(node));
						child->nodeno=count++;
						child->city=c;
						j=l+1;
						child->levelno=j;
						
						child->RCM=(int *)malloc(v*v*sizeof(int));
						initialise_RCM(child->RCM,E->RCM,v);
						initialise_infi(child->RCM,p,c,v);
						
						child->parent=E;
							
						printf("\nPath array --> ");
						
						*(soln+j)=c;
						
						for(i=0;i<=j;i++)
						{
							printf("%d\t",*(soln+i)+1);
						}
						printf("\n");
						
						printf("\nchild->nodeno :: %d\n",child->nodeno);
						printf("child->city :: %d\n",child->city+1);
						printf("child->levelno :: %d\n",child->levelno);
						
						int cost=reduced_cost(child->RCM,v);
						child->lb=E->lb+cost+(*(E->RCM+p*v+c));
						
						printf("\nRCM\n");
						
						for(i=0;i<v;i++)
						{
							for(j=0;j<v;j++)
							{
								if(*(child->RCM+i*v+j)==INT_MAX)
									printf("~ ");
								else
									printf("%d ",*(child->RCM+i*v+j));
							}
							printf("\n");
						}
						printf("\n");
							
						printf("cost(%d)=E->lb+reduced_cost(%d)+parent->RCM[%d][%d]\n",child->nodeno,child->nodeno,p,c);
						printf("cost(%d)=%d+%d+%d\n",child->nodeno,E->lb,cost,*(E->RCM+p*v+c));
						printf("cost(%d)=%d\n",child->nodeno,child->lb);
						
						printf("--------------------------------");
							
						if(child->levelno==v-1)//answer node
						{
							if(child->lb<upper)
							{
								upper=child->lb;
								if(ans!=NULL)
									free(ans);
								ans=child;
							}
						}
						else
						{
							live_list=enqueue(live_list,child);
							
							ll=live_list;
						}
					}//if
				}//if
			}//if
		}//for
		
		printf("\nList of Live Nodes : ");
		
		while(ll!=NULL){
		
			printf("->%d",ll->info->nodeno+1);
			ll = ll->next;
		}
		
		printf("\n");
		
		dead_list=enqueue(dead_list,E);
		
		i=1;
		while(live_list != NULL)
		{
			ll = live_list->next;
			E=dequeue(live_list);
			live_list=ll;
			if(E->lb>upper)
			{
			
				printf("\nThe node %d gets killed because %d>%d\n",E->nodeno+1,E->lb,upper);
				free(E->RCM);
				free(E);
			}
			else
			{
				i=0;
				break;
			}
		
		}
		if(i==1)
			break;
			
		printf("\n********************************\n\n");
			
		printf("Current Node : %d\n\n",E->nodeno+1);
		
		ll = live_list;
		
		ll = dead_list;
		printf("List of Dead Nodes : %d",ll->info->nodeno+1);
		
		while(ll->next!=NULL){
		
			ll = ll->next;
			printf("->%d",ll->info->nodeno+1);
		
		}
		
		printf("\n\n********************************\n");		
		
	}//while
	
	printf("\n********************************\n\n");
	
	ll = dead_list;
	printf("List of Dead Nodes : %d",ll->info->nodeno+1);
		
	while(ll->next!=NULL){
		
		ll = ll->next;
		printf("->%d",ll->info->nodeno+1);
		
		
	}
	
	printf("\n\n********************************\n");	
	
	if(upper!=INT_MAX)
	{
		i=0;
		T=ans;
		while(T!=R)
		{
			soln[i]=T->city;
			T=T->parent;
			i++;
		}
		
		printf("\nPATH --> 1 -> ");
		for(i=v-2;i>=0;i--)
			printf("%d -> ",*(soln+i)+1);
		printf("1\n");
	
		free(ans->RCM);
		free(ans);
	}
	else
		printf("\nNo path present!!!\n");
		
	while(dead_list!=NULL)
	{
		ll=dead_list;
		dead_list=dead_list->next;
		T=ll->info;
		free(T->RCM);
		free(T);
		free(ll);
	}
	
}//tsp

int valid(int k,int ncities)
{

	if(k>0 && k<=ncities)
		return 1;
	return 0;
}

int main()
{
	int ncities;//no of cities
	int i, j;//loop control
	int ch;//choice
	int s,d,w;//src, dest, wt.
	
	do{
	
		printf("Enter number of cities :: ");
		scanf("%d",&ncities);
		
	}while(ncities<2);//minimum of two cities should be there
	
	int *adjm=(int *)malloc(ncities*ncities*sizeof(int));
	
	for(i=0;i<ncities;i++)
		for(j=0;j<ncities;j++)
			*(adjm+i*ncities+j)=INT_MAX;
			
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~UNCOMMENT IT AFTER TESTING~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	i=0;	
	printf("Enter the edges (1 to continue and 0 if done)\n");
	do
	{
		printf("Enter source, dest and weight for edge number %d :: ",i+1);
		scanf("%d%d%d",&s,&d,&w);
		if(!valid(s,ncities) || !valid(d,ncities))
		{
			printf("Edge not possible!!! Enter again...\n");
			i--;
		}
		else if(*(adjm+(s-1)*ncities+(d-1))!=INT_MAX)
		{
			printf("Edge already exists!!! Enter again...\n");
			i--;
		}
		else
			*(adjm+(s-1)*ncities+(d-1))=w;
		i++;
		printf("Continue(1/0) :: ");
		scanf("%d",&ch);
	}while(ch==1);
	*/
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~COMMENT IT AFTER TESTING~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for(i=0;i<ncities;i++){
	
		for(j=0;j<ncities;j++){
		
			scanf("%d",(adjm+i*ncities+j));
			if( *(adjm+i*ncities+j) == -1 )  *(adjm+i*ncities+j) = INT_MAX;
			
		}
		
	}
	
	//Creation of root node	
	node *root=(node *)malloc(sizeof(node));
	root->nodeno=1;
	root->city=0;
	root->levelno=0;
	root->RCM=(int *)malloc(ncities*ncities*sizeof(int));
	root->parent=NULL;
	
	printf("\n\nAdjacency Matrix\n");
	for(i=0;i<ncities;i++)
	{
		for(j=0;j<ncities;j++)
		{
			if(*(adjm+i*ncities+j)==INT_MAX)
				printf("~ ");
			else
				printf("%d ",*(adjm+i*ncities+j));
		}
		printf("\n");
	}
	printf("\n");
	
	initialise_RCM(root->RCM,adjm,ncities);
	
	printf("--------------------------------");
	
	printf("\n\nroot->nodeno :: %d\n",root->nodeno);
	printf("root->city :: %d\n",root->city+1);
	printf("root->levelno :: %d\n",root->levelno);
	
	root->lb=reduced_cost(root->RCM,ncities);
	printf("root->cost :: %d\n\n",root->lb);
	
	printf("\n\nRCM of root node\n");
	for(i=0;i<ncities;i++)
	{
		for(j=0;j<ncities;j++)
		{
			if(*(root->RCM+i*ncities+j)==INT_MAX)
				printf("~ ");
			else
				printf("%d ",*(root->RCM+i*ncities+j));
		}
		printf("\n");
	}
	printf("\n");
	
	
	printf("--------------------------------");
	
	tsp(root,adjm,ncities);
	
	return 0;
}

/*

fully connected and path exist
5
-1 20 30 10 11
15 -1 16 4 2
3 5 -1 2 4
19 6 18 -1 3
16 4 7 16 -1

path : 1->4->2->5->3->1
last node : 11
cost : 28

partially conneted but path does not exist
4
-1 6 2 -1
-1 -1 -1 5
-1 -1 -1 2
2 -1 -1 -1

partially connected and path exist
5               
-1 2 -1 -1 -1
-1 -1 6 8 -1
1 -1 -1 -1 -1
-1 -1 -1 -1 9
-1 -1 2 -1 -1

path : 1->2->4->5->3->1
last node : 6
cost : 22

partially conneted but path does not exist
4
-1 2 -1 -1
-1 -1 6 -1
3 -1 -1 7
-1 8 -1 -1

*/
