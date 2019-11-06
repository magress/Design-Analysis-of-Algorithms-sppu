//directed weighted graph bellmenford algorithm
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>



struct edge
{
	// This structure is equal to an edge. Edge contains two end points. These edges are directed edges so they
	//contain source and destination and some weight. These 3 are elements in this structure
	int source,destination,weight;
};

int Bellmanford(struct edge *E, int v, int e, int source_vertex)
{
	int *store_Distance = (int*)malloc(v*(sizeof(int)));
	int i,j,k,condition;
	
    // This is initial step that we know , we initialize all distance to infinity except source.
	// We assign source distance as 0(zero)
 
	for(i = 0; i < v; i++)
	{
		store_Distance[i] = INT_MAX;
	}
	
	store_Distance[source_vertex] = 0;
	
	for(i = 1; i <= v-1; i++)
	{
		condition = 0;
		for(j = 0; j < e; j++)
		{
			int u = E[j].source;
			int v = E[j].destination;
			int w = E[j].weight;

			if((store_Distance[u] != INT_MAX) && ((store_Distance[u] + w) < store_Distance[v]))
			{
				store_Distance[v] = store_Distance[u] + w;
				condition = 1; //indication that changes are still happening
			}
		}
		if(condition == 0) //condition to check if changes stops to happen before v-1 iterations
			break;
	}
	// Actually upto now shortest path found. But BellmanFord checks for negative edge cycle. In this step we check for that
    // shortest distances if graph doesn't contain negative weight cycle.
 
    // If we get a shorter path, then there is a negative edge cycle.
	if(condition == 1)
	{
		for(j=0; j<e; j++)
		{
			int u = E[j].source;
			int v = E[j].destination;
			int w = E[j].weight;

			if((store_Distance[u] != INT_MAX) && ((store_Distance[u] + w) < store_Distance[v]))
			{
				printf("\n---Negative Cycle---\n");
				return 0;
			}
		}		
	}
	printf("\nNumber of Iterations are:- %d \n",i);
	printf("Source\tDestination\tDistance\n");
	for( j = 0; j < v; j++)
	{
		if(store_Distance[j] == INT_MAX)
			printf("  %d Path Not Possible\n",j+1);
		else
			printf("%d\t%d\t%d\n",source_vertex+1,j+1,store_Distance[j]);
	}
	free(store_Distance);
	return 0;
}

int wrong(int s,int d,struct edge *E , int v,int i)//to check if any wrong condition arrives 
{
	if(s < 0 || s >= v || d < 0 || d >= v)
	{
		printf("\nEnter Proper Source or Destination");	
		return 1;
	}

	for(int j = 0; j < i; j++)
	{
		if(E[j].source == s && E[j].destination == d)
		{
			printf("\n Duplicate Egde... Enter new edge");
			return 1;
		}
	}
	return 0;
}

int main()
{
	int e,v;
	do
	{
		printf("\nEnter the number of Edges and Vertices:- ");
		scanf("%d %d",&e,&v);
		if(e <= 0 || v <= 0 || e > (v*(v-1))) //validation
			printf("\nGraph not Possible\n");
		else
			break;
	}while(1);
	int s,d,w,i;
	struct edge *E = (struct edge*)malloc(e*(sizeof(struct edge)));

	for(i = 0; i < e; i++)
	{
		printf("\n Enter Source , Destination and Weight of  %d Edge :- ",i+1);
		scanf("%d %d %d",&s,&d,&w);
		s--;
		d--;

		if((wrong(s,d,E,v,i) || (s == v)))				// checks for duplicate edges and proper source and destination
		{
			i--;
		}
		else
		{
			E[i].source = s;
			E[i].destination = d;
			E[i].weight = w;
		}
	}
	int source_vertex;
	do
	{
		printf("\nEnter Source Vertex:- ");
		scanf("%d", &source_vertex);
		source_vertex--;
		if(source_vertex < 0 || source_vertex >= v)
		{
			printf("\nSource Vertex Not Possible\n");
		}
		else
			break;
	}while(1);

	Bellmanford(E,v,e,source_vertex);
	free(E);
	return 0;
}





















/*Enter the number of Edges and Vertices:- 4
4

 Enter Source , Destination and Weight of an 1 Edge :- 1 2 4

 Enter Source , Destination and Weight of an 2 Edge :- 1 4 5

 Enter Source , Destination and Weight of an 3 Edge :- 4 3 3

 Enter Source , Destination and Weight of an 4 Edge :- 3 2 -10

Enter Source Vertex:- 1

Number of Iterations are:- 2 
Source	Destination	Distance
1	1	0
1	2	-2
1	3	8
1	4	5
*/

