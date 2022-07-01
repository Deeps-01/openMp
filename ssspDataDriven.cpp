/* use this command for compiling 
g++ -Xpreprocessor -fopenmp /opt/homebrew/Cellar/libomp/14.0.5/lib/libomp.a
*/
//single src shortest path

#include <iostream>
#include <string.h>
#include </opt/homebrew/Cellar/libomp/14.0.5/include/omp.h>
#include "atomic.h"
#include <sstream>
#include <fstream>
#include <climits>
#include <vector>
#include <algorithm>


using namespace std;


struct Edge {
	int src, dest, weight;
};
/*
struct Graph {
	
	int V, E;
	struct Edge* edge;
};


struct Graph* createGraph(int V, int E)
{
	struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
	graph->V = V;
	graph->E = E;
	graph->edge = (struct Edge *)malloc(graph->E * sizeof(struct Edge));
	return graph;
}
*/

void printans(vector <int> dist, int n,double t1,double t2)
{
	//storing in a file

	ofstream MyFile("CSRparallel_output.txt");
	MyFile<<"Vertex Distance from Source\n";
	MyFile<<"NodeId\tShortest distance from source \n";
	for (int i = 0; i < n; i++)
		MyFile<<i<<" \t\t "<<dist[i]<<"\n";

  // Close the file
  MyFile.close();

	printf("Vertex Distance from Source\n");
	for (int i = 0; i < n; i++)
		printf("%d \t\t %d\n", i, dist[i]);
    
    cout<<"total time taken is "<<t2-t1<<endl;   

	
}

 
void BellmanFord(vector <int> offset,vector<int> edges ,int V,int E, int src)
{
	
vector <int> dist(V);
vector <bool> change(V);
vector <bool> localchange(V);
bool over=false;



double t1=omp_get_wtime();

   #pragma omp parallel for
	for (int i = 0; i < V; i++)
		dist[i] = INT_MAX;

	dist[src] = 0;
	change[src]=true;

    int nbr,k;

	omp_set_num_threads(16);

	while(!over){    
        over=true;
		#pragma omp parallel for default(none) shared(change,localchange,V,dist,edges,over,offset) private(nbr) firstprivate(k) 
		for (int i = 0; i < V; i++){


			if(change[i]==true)
			{
			  for(k=offset[i];k<offset[i+1]; k++)
			  {
		   		nbr=edges[k];

				int wt = 2;
				 int dist_new = dist[i]+wt;
				 
				bool modified_new = true;
				if(dist[nbr]>dist_new)
                {
                 atomicMin(&dist[nbr],dist_new);                
                    over=false;  
                    localchange[nbr]=modified_new;
                  
                }
			  }		
		    }



	    } 

		#pragma omp parallel for
		for(int i=0; i<V; i++)
		{
			change[i]=localchange[i];
			localchange[i]=false;
		}

	}
				
	double t2=omp_get_wtime();
	
	printans(dist, V,t1,t2);
}



int main(int argc,char *argv[])
{ 
    int V = 1971281; // Number of vertices in graph
    int E = 5533214; // Number of edges in graph
    vector <Edge> g(E);
	vector <int> edges(E);
    vector <int> offset(V+1);
    vector <int> offset2(V+1);


	ifstream infile;
	infile.open("roadNet-CA.txt");
	string line;
	int i=0;

	while(getline(infile,line))
	{
		if(line.length()==0||line[0] < '0'||line[0]>'9'){
			continue;
		}

		stringstream ss(line); 

		ss>>g[i].src;
		ss>>g[i].dest;
		
		i++;
		
	}

	for(int i=0; i<E ; i++)
  {

    int u=g[i].src;
    int v=g[i].dest;

    offset[u+1]+=1;
  }

  for(int i=1; i<=V ; i++)
  {
    offset[i]+=offset[i-1];
  }


  for(int i=1; i<=V ; i++)
  {
    offset2[i]=offset[i];
  }

  for(int i=0; i<E; i++)
  {
    int u=g[i].src;
    int v=g[i].dest;
    edges[offset2[u]]=v;
    offset2[u]++;
  }
	
    BellmanFord( offset, edges , V, E, 0);

    return 0;
}


