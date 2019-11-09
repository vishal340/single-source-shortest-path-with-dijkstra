#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<iterator>
#include<ctime>
using namespace std;
struct graph
{
unsigned int vertex;
unsigned int edge;
vector<pair<unsigned int,unsigned int> > adjacency;
int* dist;
};
void extract_data(vector<pair<unsigned int,unsigned int> > &adjacency,char* str,unsigned int vertex)
{
int n=1;
for(int i=0;i<strlen(&str[0])-1;i++)
{
if(str[i]==' ')
{
n++;
}
}
unsigned int edges[n];
//cout<<n<<"\t";
n=0;
edges[0]=0;
for(int i=0;i<strlen(&str[0])-1;i++)
{
if(str[i]!=' ')
{
edges[n]*=10;
edges[n]+=(int)str[i]-48;
}
else
{
n++;
edges[n]=0;
}
}
pair<unsigned int,unsigned int>edge;
for(int i=0;i<=n;i++)
if(edges[i]>vertex)
{
edge.first=vertex;
edge.second=edges[i];
adjacency.push_back(edge);
}
}
void sssp(graph &gp)
{
unsigned int distance=1;
vector<pair<unsigned int,unsigned int> >::iterator iter;
bool change=true;
for(int i=1;i<gp.vertex;i++)
{
if(change== true)
{
change=false;
for(iter=gp.adjacency.begin();iter<gp.adjacency.end();iter++)
{
if(gp.dist[iter->first] * gp.dist[iter->second] <=0 )
{
if(gp.dist[iter->first]== -1)
{
change=true;
gp.dist[iter->first]=distance;
}
else if(gp.dist[iter->second]== -1)
{
change=true;
gp.dist[iter->second]=distance;
}
}
}
distance++;
}
else
break;
}
cout<<"maximum distance\t"<<--distance<<"\n";
}

int main(int argc,char** argv)
{
struct timespec start,finish;
ifstream in("graph.txt");
string str;
unsigned int* edges;
for(int i=0;i<14;i++)
{
getline(in,str);
}
graph gp;
getline(in,str);
gp.vertex=0;
gp.edge=0;
int i=0;
while(str[i]!=' ')
{
gp.vertex*=10;
gp.vertex+=(int)str[i]-48;
i++;
}
i++;
while(i<strlen(&str[0])-1)
{
gp.edge*=10;
gp.edge+=(int)str[i]-48;
i++;
}
gp.dist=new int[gp.vertex+1];
for(int i=1;i<gp.vertex+1;i++)
gp.dist[i]=-1;
unsigned int line=1;
while(!in.eof())
{
getline(in,str);
extract_data(gp.adjacency,&str[0],line);
line++;
}
int source=atoi(argv[1]);
gp.dist[source]=0;

clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
sssp(gp);
clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&finish);
cout<<"Time taken\t"<<(finish.tv_sec-start.tv_sec)+(finish.tv_nsec-start.tv_nsec)/1e09<<"\n";
ofstream out("output.txt");
for(int i=1;i<gp.vertex+1;i++)
out<<i<<"\t"<<gp.dist[i]<<"\n";
return 0;
}
