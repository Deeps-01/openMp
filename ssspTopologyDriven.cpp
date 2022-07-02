/* use this command for compiling 
g++ -Xpreprocessor -fopenmp /opt/homebrew/Cellar/libomp/14.0.5/lib/libomp.a
*/
//single src shortest path

#include <iostream>
#include <string.h>
#include </opt/homebrew/Cellar/libomp/14.0.5/include/omp.h>
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
	for (int i = 1; i <= n; i++)
		MyFile<<i<<" \t\t "<<dist[i]<<"\n";

  // Close the file
  MyFile.close();

	printf("Vertex Distance from Source\n");
	for (int i = 1; i <= n; i++)
		printf("%d \t\t %d\n", i, dist[i]);
    
    printf("total time taken is %g",t2-t1);   
	
}

 
void BellmanFord(vector <int> offset,vector<int> edges ,int V,int E, int src)
{
	
vector <int> dist(V);
bool change = true;
omp_set_num_threads(4);

double t1=omp_get_wtime();

   #pragma omp parallel for
	for (int i = 1; i <= V; i++)
		dist[i] = INT_MAX;

	dist[src] = 0;

	int k=0,u,v,tid;

	while(change){    
        change=false;
		#pragma omp parallel for default(none) shared(E,V,dist,change,offset,edges,cout) private(u,v,tid) firstprivate(k)
		for (int i = 1; i <= V; i++){
		    u=i;
			k=offset[u];

			while(k<offset[u+1])
			{
			v=edges[k];

			
			int wt = 1;

				if (dist[u] != INT_MAX && dist[u] + wt < dist[v]){
					dist[v] = dist[u] + wt;
					change=true;
				}
				k++;
			}		
		}
	} 
				
	double t2=omp_get_wtime();
	
	printans(dist, V,t1,t2);
}



int main(int argc,char *argv[])
{ 
    int V = 3370462; // Number of vertices in graph
    int E = 93373056; // Number of edges in graph
    vector <Edge> g(E);
	vector <int> edges(E);
    vector <int> offset(V+1);
    vector <int> offset2(V+1);


	ifstream infile;
	infile.open("out.wikipedia_link_ru.txt");
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

//   cout<<"offset2 of 0 "<<offset2[0]<<"\n";
  for(int i=1; i<=V ; i++)
  {
    offset2[i]=offset[i];
  }

  for(int i=0; i<E; i++)
  {
    int u=g[i].src;
    int v=g[i].dest;

    if(offset2[u] >= 0 && offset2[u] <= 2)
	  {
		// cout<<"u-val "<<u<<" v val "<<v<<" offset-2 "<<offset2[u]<<"\n";
	  }
    edges[offset2[u]]=v;
    offset2[u]++;
  }
	// for(int i=0 ; i<V ; i++)
	//    {
	// 	cout<<"i "<<i<<" "<<"offset-start "<<offset[i]<<"offset-end "<<offset[i+1]<<" ";
    //      for(int j = offset[i] ; j<offset[i+1] ; j++)
	// 	    {
    //           cout<<edges[j]<<" ";

	// 		}
    //    cout<<"\n";

	//    }
    BellmanFord( offset, edges , V, E, 1);

    return 0;
}


