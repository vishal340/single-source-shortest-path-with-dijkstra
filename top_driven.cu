#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<iterator>
#include<ctime>
#include<limits>
#include<list>
#include<algorithm>

using namespace std;

struct info_edge     
{
int vertex1,vertex2;
int edge;
};

//This function to extract data from file
void extract_data(info_edge &info,char* str)
{
int i,m=0;
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
weight=0;
i++;
while(i<strlen(&str[0]))
{
weight*=10;
weight+=(int)str[i]-48;
i++;
}
info.vertex1=--vertex[0];
info.vertex2=--vertex[1];
info.edge=weight;
}



//This is the kernel
__global__ void sssp(pair<int,int>* adjacency,int* dist,int* change,int* count)
{
int n=blockDim.x*blockIdx.x + threadIdx.x;
int n1=n*10;
//if(n%10000==0)
//printf("%d\t",count[n]);
if(dist[n]!=2147483647)
for(int i=0;i<count[n];i++)
{
if(dist[adjacency[n1+i].first] > adjacency[n1+i].second + dist[n])
{
change[0]=1;
dist[adjacency[n1+i].first] = adjacency[n1+i].second + dist[n];
}
}
}

int main(int argc,char** argv)
{
struct timespec start,finish;
ifstream in(argv[2]);
string str;
for(int i=0;i<4;i++)
getline(in,str);

getline(in,str);
int vertex=0;
int edge=0;
int i=5;

//these 2 while loop for finding number of veritices and edges
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
vertex++;



pair<int,int>* adjacency1=new pair<int,int>[vertex*10];//datasructure of graph
int* count1=new int[vertex];

for(i=0;i<2;i++)
getline(in,str);
int* dist1=new int[vertex];

int source=atoi(argv[1]);
dist1[source]=0;

for(int i=0;i<vertex;i++)
{
count1[i]=0;
if(i!=source)
dist1[i]=std::numeric_limits<int>::max();
}
info_edge info;
pair<int,int> sub_info;


while(!in.eof())    		//loop on each line of file
{
getline(in,str);
extract_data(info,&str[0]);
sub_info.first=info.vertex2;
sub_info.second=info.edge;
adjacency1[info.vertex1*10+count1[info.vertex1]]=sub_info;
count1[info.vertex1]++;
}


pair<int,int>* adjacency;
int *dist,*count;
int* change1=new int[1];
int* change;

cudaMalloc((void**)&adjacency,sizeof(int)*2*vertex*10);
cudaMalloc((void**)&dist,sizeof(int)*vertex);
cudaMalloc((void**)&count,sizeof(int)*vertex);
cudaMemcpy(adjacency,adjacency1,sizeof(int)*2*vertex*10,cudaMemcpyHostToDevice);
cudaMemcpy(dist,dist1,sizeof(int)*vertex,cudaMemcpyHostToDevice);
cudaMemcpy(count,count1,sizeof(int)*vertex,cudaMemcpyHostToDevice);
dim3 dimBlock(512);
dim3 dimGrid(3693);

int iteration=0;
clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);


do
{
change1[0]=0;
cudaMalloc((void**)&change,sizeof(int));
cudaMemcpy(change,change1,sizeof(int),cudaMemcpyHostToDevice);

sssp<<<dimGrid,dimBlock>>>(adjacency,dist,change,count);

cudaMemcpy(change1,change,sizeof(int),cudaMemcpyDeviceToHost);
iteration++;
}while(change1[0]==1);

clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&finish);
cudaMemcpy(dist1,dist,sizeof(int)*vertex,cudaMemcpyDeviceToHost);



cout<<"no.of iterations: "<<iteration<<"\n";
cout<<"Time taken\t"<<(finish.tv_sec-start.tv_sec)+(finish.tv_nsec-start.tv_nsec)/1e09<<"\n";
ofstream out("output.txt");
for(int i=0;i<vertex-1;i++)
out<<i+1<<"\t"<<dist1[i]<<"\n";
return 0;
}
