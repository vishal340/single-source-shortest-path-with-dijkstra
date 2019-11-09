#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<iterator>
#include<ctime>
#include<limits>

using namespace std;

struct info_edge
{
int vertex1,vertex2;
int weight;
};

void extract_data(vector<info_edge> &adjacency,char* str)
{
int i,n=1,m=0;
int vertex[2];
int weight;
vertex[0]=0;
vertex[1]=0;
for(i=2;i<strlen(&str[0])-1;i++)
{
if(str[i]!=' ')
{
vertex[m]*=10;
vertex[m]+=(int)str[i]-48;
}
else if(m<1)
{
m++;
}
else
break;
}
info_edge edge;
if(vertex[0]<vertex[1])
{
edge.vertex1=--vertex[0];
edge.vertex2=--vertex[1];
}
else
return;
weight=0;
i++;
while(i<strlen(&str[0]))
{
weight*=10;
weight+=(int)str[i]-48;
i++;
}
edge.weight=weight;
adjacency.push_back(edge);
}

void sssp(vector<info_edge>adjacency,int* dist)
{
vector<info_edge>::iterator iter;
bool change=true;
int i=1;
while(change)
{
change=false;
for(iter=adjacency.begin();iter<adjacency.end();iter++)
{
if(dist[iter->vertex1] != dist[iter->vertex2])
{
int t=dist[iter->vertex1]-dist[iter->vertex2];
if(t>iter->weight)
{
change=true;
dist[iter->vertex1]=dist[iter->vertex2]+iter->weight;
}
else if(-t>iter->weight)
{
change=true;
dist[iter->vertex2]=dist[iter->vertex1]+iter->weight;
}
}
}
i++;
}
cout<<"Number of iterations :"<<--i<<"\n";
}

int main(int argc,char** argv)
{
struct timespec start,finish;
ifstream in(argv[2]);
string str;
int vertex;
int edge;
vector<info_edge> adjacency;
int* dist;
for(int i=0;i<4;i++)
{
getline(in,str);
}

getline(in,str);
vertex=0;
edge=0;
int i=5;
while(str[i]!=' ')
{
vertex*=10;
vertex+=(int)str[i]-48;
i++;
}
i++;
while(i<strlen(&str[0])-1)
{
edge*=10;
edge+=(int)str[i]-48;
i++;
}
for(i=0;i<2;i++)
getline(in,str);

dist=new int[vertex];
for(int i=0;i<vertex;i++)
dist[i]=std::numeric_limits<int>::max();
while(!in.eof())
{
getline(in,str);
extract_data(adjacency,&str[0]);
}
int source=atoi(argv[1]);
dist[source]=0;

clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
sssp(adjacency,dist);
clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&finish);
cout<<"Time taken\t"<<(finish.tv_sec-start.tv_sec)+(finish.tv_nsec-start.tv_nsec)/1e09<<"\n";
ofstream out("output1.txt");
for(int i=0;i<vertex;i++)
out<<i+1<<"\t"<<dist[i]<<"\n";
return 0;
}
