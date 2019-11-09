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


//Input method: 1st argument is source vertex and second argument is graph set
//The input file should have exactly one edge in each line
//Even with that adjust the function "edge info" below according to input file
//look into Readme.md for additional information


int binary_search(std::list<std::pair<int,int> >::iterator &it,int source,int end,int value,int vertex){
    if(source+1>=end){
        if(source==end)
            return source;
        else
        {
            if(value==it->second)
            {
                if(it->first<vertex){
                    it++;
                    return end;
                }
                else
                return source;
            }
            else{
            if(value<it->second)
            {
                return source;
            }
            else if(value>it->second)
            {
                it++;
                return end;
            }
            }
            
        }
    }
    else
    {
        int middle=(int)(source+end)/2;
        std::advance(it,(int)(end-source)/2);
        if(it->second!=value){
            if(it->second > value){
                std::advance(it,(source-end)/2);
                return binary_search(it,source,middle,value,vertex);
            }
            else{
                it++;
                return binary_search(it,middle+1,end,value,vertex);
            }
        }
        else
        {
            int counter=0;
            while(it->second==value && it->first<vertex)
            {
                if(counter==(int)(end-source)/2+1)
                    break;
                it++;
                counter++;
            }
            if(counter==0)
            while(it->second==value && it->first>vertex)
            {
                if(counter==(int)(source-end)/2-1)
                    break;
                it--;
                counter--;
            }
            if(counter<0){
                it++;
                counter++;
            }
            counter+=(int)middle;
            return counter;
        }
    }
}

int search(std::list<std::pair<int,int> >::iterator &it,int source,int end,int value,int vertex){
    if(end==source+1)
    {
        if(value==it->second)
        {
            if(it->first==vertex)
                return source;
            else{
                it++;
                return end;
            }
        }
        it++;
        return end;
    }
    else
    {
        int middle=(int)((source+end)/2);
        std::advance(it,middle-source);
        if(it->second !=value){
            if(it->second > value){
                std::advance(it, source -middle);
                return search(it,source,middle-1,value,vertex);
            }
            else{
                it++;
                return search(it,middle+1,end,value,vertex);
            }
        }
        else
        {
            int counter=0;
            while(it->first<vertex)
            {
                it++;
                counter++;
            }
            while(it->first>vertex)
            {
                it--;
                counter--;
            }
            counter+=middle;
            return counter;
        }
    }
}

struct info_edge
{
    int vertex1,vertex2;
    int edge;
};
void extract_data(info_edge &info,char* str)
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

void sssp(std::vector<std::vector<std::pair<int,int> > >adjacency,int source,int* dist)
{
    struct timespec start,finish;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
    std::list<std::pair<int,int> >priority_queue;
    std::pair<int,int> insert;
    insert.first=source;
    insert.second=0;
    priority_queue.push_front(insert);
    std::vector<std::pair<int,int> >::iterator iter;
    std::list<std::pair<int,int> >::iterator it;
    int position,prev_pos;

    while(!priority_queue.empty())
    {
        std::pair<int,int> edge_info=priority_queue.front();
        priority_queue.pop_front();
        for(iter=adjacency[edge_info.first].begin();iter !=adjacency[edge_info.first].end();iter++)
        {
            if(dist[iter->first] > iter->second + dist[edge_info.first])
            {
                if(dist[iter->first]!=std::numeric_limits<int>::max())
                {
                    if(priority_queue.size()==1)
                        prev_pos=0;
                    else{
                        it=priority_queue.begin();
                        prev_pos=search(it,0,priority_queue.size()-1,dist[iter->first],iter->first);
                    }
                }
                else
                    prev_pos=std::numeric_limits<int>::max();
                dist[iter->first]=iter->second + dist[edge_info.first];
                if(priority_queue.size()==0)
                {
                    insert.first=iter->first;
                    insert.second=dist[iter->first];
                    priority_queue.push_front(insert);
                }
                else
                {
                    if(prev_pos!=std::numeric_limits<int>::max())
                    {
                        it=priority_queue.begin();
                        position=binary_search(it,0,prev_pos,dist[iter->first],iter->first);
                        if(position!=prev_pos)
                        {
                            insert.first=iter->first;
                            insert.second=dist[iter->first];
                            priority_queue.insert(it,insert);
                            std::advance(it,prev_pos-position);
                            priority_queue.erase(it);
                        }
                        else
                        {
                            it->second=dist[iter->first];
                        }
                    }
                    else
                    {
                        insert.first=iter->first;
                        insert.second=dist[iter->first];
                        if(priority_queue.back().second<dist[iter->first] || (priority_queue.back().second==dist[iter->first] && priority_queue.back().first<iter->first))
                        {
                            priority_queue.push_back(insert);
                        }
                        else
                        {
                            it=priority_queue.begin();
                            position=binary_search(it,0,priority_queue.size()-1,dist[iter->first],iter->first);
                            priority_queue.insert(it,insert);
                        }
                    }
                }
            }
        }
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&finish);
    std::cout<<"Time taken\t"<<(finish.tv_sec-start.tv_sec)+(finish.tv_nsec-start.tv_nsec)/1e09<<"\n";
}

int main(int argc,char** argv)
{
    std::ifstream in(argv[2]);
    std::string str;
    for(int i=0;i<4;i++)
        getline(in,str);

    getline(in,str);
    int vertex=0;
    int edge=0;
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
    std::vector<std::vector<std::pair<int,int> > > adjacency(vertex);

    for(i=0;i<2;i++)
        getline(in,str);
    int* dist=new int[vertex];
    int source=atoi(argv[1]);
    dist[source]=0;

    for(int i=0;i<vertex;i++)
    {
        if(i!=source)
            dist[i]=std::numeric_limits<int>::max();
    }

    info_edge info;
    std::pair<int,int> sub_info;
    while(!in.eof())
    {
        getline(in,str);
        extract_data(info,&str[0]);
        sub_info.first=info.vertex2;
        sub_info.second=info.edge;
        adjacency[info.vertex1].push_back(sub_info);
    }

    sssp(adjacency,source,dist);

    std::ofstream out("output1.txt");
    for(int i=0;i<vertex;i++)
        out<<i+1<<"\t"<<dist[i]<<"\n";
    return 0;
}