#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
#include<fstream>
#include<iomanip>
using namespace std;
#define INF 1000000000


struct Edge
{
    int first;
    int second;
    double weight;
    int index;
};


bool compare_edge(Edge a,Edge b)
{
    return a.weight<b.weight;
}

struct Edge1
{
    double weight=INF;
    int to=-1;

    Edge1(double w,int t){
        weight=w;
        to=t;
    }

    bool operator<(Edge1 const& a) const{
        return make_pair(weight,to)<make_pair(a.weight,a.to);
    }
};


int n,m,i,j;
Edge edges[10002];
vector<pair<int,int> > adj[1002];
double ans=0.0;
vector<pair<int,int> > ans_prim,ans_kruskal;
vector<bool> visited(1002);
set<Edge1> q;
vector<Edge1> min_edge;
vector<int> parent, ranks;


int find_set(int v)
{
    if(v==parent[v])
      return v;
    return parent[v]=find_set(parent[v]);
}


void union_sets(int a,int b)
{
    a=find_set(a);
    b=find_set(b);
    if(a!=b){
       if(ranks[b]>ranks[a])
         swap(a,b);
       parent[b]=a;
       if(ranks[b]==ranks[a])
         ranks[a]++;
    }
}


void prim()
{
    visited.assign(n,false);
    for(i=0;i<n;i++){
        Edge1 ed1(INF,-1);
        min_edge.push_back(ed1);
    }

    min_edge[0].weight=0;
    min_edge[0].to=-1;
    Edge1 edge1(0,0);
    q.insert(edge1);

    for(i=0;i<n;i++){
        int p1=q.begin()->to;
        ans+=q.begin()->weight;
        q.erase(q.begin());
        visited[p1]=true;

        if(min_edge[p1].to!=-1){
           ans_prim.push_back(make_pair(p1,min_edge[p1].to));
        }

        for(pair<int,int> p: adj[p1]){

            if(!visited[p.first]&&min_edge[p.first].weight>edges[p.second].weight){
                Edge1 e1(min_edge[p.first].weight,p.first);
                q.erase(e1);

                min_edge[p.first].weight=edges[p.second].weight;
                min_edge[p.first].to=p1;

                Edge1 e2(edges[p.second].weight,p.first);
                q.insert(e2);
            }
        }
    }
}


void kruskal()
{
    sort(edges,edges+m,compare_edge);
    parent.resize(n);
    ranks.resize(n);

    for(i=0;i<n;i++){
        parent[i]=i;
        ranks[i]=0;
    }

    ans=0;
    for(Edge e: edges){
        if(find_set(e.first)!=find_set(e.second)){
           ans_kruskal.push_back(make_pair(e.first,e.second));
           ans+=e.weight;
           union_sets(e.first,e.second);
        }
    }
}


int main()
{
    ifstream in;
    in.open("sample.txt");
    in>>n>>m;

    for(i=0;i<m;i++){
        in>>edges[i].first>>edges[i].second>>edges[i].weight;
        edges[i].index=i;
        adj[edges[i].first].push_back(make_pair(edges[i].second,i));
        adj[edges[i].second].push_back(make_pair(edges[i].first,i));
    }


    prim();
    cout<<"Cost of the minimum spanning tree : ";
    cout<<fixed<<setprecision(1)<<ans<<endl;
    cout<<"List of edges selected by Prim's:{";
    for(i=0;i<ans_prim.size()-1;i++){
        cout<<"("<<ans_prim[i].first<<","<<ans_prim[i].second<<"),";
    }
    cout<<"("<<ans_prim[i].first<<","<<ans_prim[i].second<<")}"<<endl;


    kruskal();
    cout<<"List of edges selected by Kruskal's:{";
    for(i=0;i<ans_kruskal.size()-1;i++){
        cout<<"("<<ans_kruskal[i].first<<","<<ans_kruskal[i].second<<"),";
    }
    cout<<"("<<ans_kruskal[i].first<<","<<ans_kruskal[i].second<<")}"<<endl;

    return 0;
}
