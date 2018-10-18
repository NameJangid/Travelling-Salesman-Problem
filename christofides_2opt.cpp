#include<iostream>
#include <vector>
#include <utility>
#include<algorithm>
#include <queue>
#include <stack>

#define MILESTONE cout<<"~#~#~"<<endl

using namespace std;

typedef pair<float,pair<int,int> > cPair;

bool euclidean;
int N;
vector<vector<float> > mat;
vector<pair<float,float> > cord;

void printMatrix(vector<vector<float> > &v)
{
  for (int i=0;i<v.size();++i) {
    for (int j=0;j<v.size();++j) {
      cout << v[i][j] <<" ";
    }
    cout<<endl;
  }
  return;
}

void printVector(vector<int> &v,string spacer)
{
  for (int i=0;i<v.size()-2;++i)
    cout<<v[i] <<spacer;
  cout<<v[v.size()-2]<<endl;
  return;
}
vector<vector<float> > prims(vector<vector<float> > &v)
{
  int n=v.size();
  priority_queue< cPair, vector<cPair>, greater<cPair> > pq;
  vector<vector<float> > mst(n,vector<float> (n,0));
  for (int i=1;i<n;++i) {
    pq.push(make_pair(v[0][i],make_pair(0,i)));
  }
  vector<bool> visited(n,false);

  visited[0]=true;

  while (!pq.empty())
  {
    int src=pq.top().second.first, dst=pq.top().second.second;
    float cost=pq.top().first;
    pq.pop();
    if (!visited[dst]) 
    {
      mst[src][dst]=cost; mst[dst][src]=cost;
      visited[dst]=true;
      
      for (int j=0;j<n;++j)
      {
        if (!visited[j])
          pq.push(make_pair(v[dst][j],make_pair(dst,j)));
      }
    }
  }
  return mst;
}

vector<int> odd_vertices(vector<vector<float> > &v)
{
  vector<int> odd;
  for (int i=0;i<v.size();++i)
  {
    int connections=0;
    for (int j=0;j<v.size();++j)
    {
      if (v[i][j] != 0)
        ++connections;
    }
    if (connections % 2 == 1) odd.push_back(i);
  }
  return odd;
}

vector<vector<float> > approxPerfectMatching(vector<vector<float> > &v, vector<vector<float> > &mst, vector<int> &odd)
{
  vector<bool> visited(v.size(),false);
  vector<vector<float> > apm(v.size(),vector<float> (v.size(),0.0));

  priority_queue< cPair, vector<cPair>, greater<cPair> > pq;
  for (int i=0;i<odd.size();++i)
  {
    for(int j=i+1;j<odd.size();++j) {
      pq.push( make_pair(v[odd[i]][odd[j]], make_pair(odd[i],odd[j]) ) );
    }
  }
  int count=0;
  while (count < odd.size())
  {
    int node1 = pq.top().second.first, node2 = pq.top().second.second;
    float cost = pq.top().first;
    pq.pop();
    if (!visited[node1] && !visited[node2] && mst[node1][node2]==0)
    {
      visited[node1] = true; visited[node2] = true;
      apm[node1][node2] = v[node1][node2]; apm[node2][node1] = v[node2][node1];
      count += 2;
    }
  }
  return apm;
}

vector<vector<float> > unionGraphs(vector<vector<float> > &v1, vector<vector<float> > &v2)
{
  vector<vector<float> > u(v1.size(),vector<float> (v1.size(),0));
  for (int i=0;i<v1.size();++i)
  {
    for (int j=0;j<v1.size();++j)
    {
      if (v1[i][j] != 0)
      {
        u[i][j]=v1[i][j];
      }
      else if (v2[i][j] != 0)
      {
        u[i][j]=v2[i][j];
      }
    }
  }
  return u;
}

bool nodeHasNeighbour(vector<vector<float> > &v,int node)
{
  for (int i=0;i<v.size();++i)
  {
    if (v[node][i] != 0) return true;
  }
  return false;
}

vector<int> eularianPath(vector<vector<float> > v)
{
  vector<int> epath;
  stack<int> st;
  int n=v.size();
  int cur=0;

  while (!st.empty() || nodeHasNeighbour(v,cur))
  {
    if (!nodeHasNeighbour(v,cur))
    {
      epath.push_back(cur);
      cur = st.top();
      st.pop();
    }
    else
    {
      for (int i=0;i<v.size();++i)
      {
        if (v[cur][i] != 0)
        {
          st.push(cur);
          v[cur][i]=0;
          v[i][cur]=0;
          cur=i;
          break;
        }
      }
    }
  }
  return epath;
}

vector<int> hamiltonian(vector<int> &epath,int n)
{
  vector<bool> visited(n,false);
  vector<int> hm;
  for (int i=0;i<epath.size();++i)
  {
    if (!visited[epath[i]])
    {
      visited[epath[i]]=true;
      hm.push_back(epath[i]);
    }
  }
  hm.push_back(hm[0]);
  return hm;
}

float getPathCost(vector<int> &route, vector<vector<float> > &v,int i,int j)
{
  float pc=0;
  for (i=i+1;i<=j;++i)
  {
    pc += mat[route[i-1]][route[i]];
  }
  
  return pc;
}

vector<int> swap_two_opt(vector<int> &route, int i,int k)
{
  vector<int> newroute=route;
  reverse(newroute.begin()+i,newroute.begin()+(k+1));
  return newroute;
}

vector<int> two_opt(vector<int> &goodPath, vector<vector<float> > &v)
{
  vector<int> route=goodPath;
  bool improved=true;
  int ct;
  float currentCost, newCost;
  currentCost = getPathCost(route,v,0,route.size()-1);
  for (ct=0; ct < (route.size())*2 && improved;++ct)
  // while (improved)
  {
    improved = false;
    for (int i=1;i<route.size()-2;++i)
    {
      for (int j=i+1;j<route.size()-1;++j)
      {
        newCost = currentCost - v[route[i-1]][route[i]] - v[route[j]][route[j+1]] + v[route[i-1]][route[j]] + v[route[i]][route[j+1]];
        if (newCost < currentCost)
        {
          route = swap_two_opt(route,i,j);
          improved=true;
          currentCost = newCost;
          goto LOOPEND;
        }
      }
    }
    LOOPEND:
    continue;
  }
  return route;
}

int main(int argc, char const *argv[])
{
  string str;
  cin>>str;
  
  if (str=="euclidean")
    euclidean=true;
  else euclidean=false;
  cin>>N;
  cord.resize(N,make_pair(0,0));
  for (int i=0;i<N;++i) {
    cin>>cord[i].first>>cord[i].second;
  }
  MILESTONE;

  mat = vector<vector<float> > (N,vector<float> (N,0));
  for (int i=0;i<N;++i)
    for(int j=0;j<N;++j)
      cin >> mat[i][j];

  MILESTONE;
  // get minimum spanning tree
  vector<vector<float> > mst=prims(mat);
  MILESTONE;
  // odd vertices in mst
  vector<int> odd = odd_vertices(mst);
  MILESTONE;
  // get approx perfect matching
  vector<vector<float> > u = approxPerfectMatching(mat,mst,odd);
  MILESTONE;
  // union mst and approxPerfectMatching graph
  u = unionGraphs(mst,u);
  MILESTONE;
  // get eularian path
  vector<int> epath=eularianPath(u);
  MILESTONE;
  // create hamiltonian from eularian path
  vector<int> hm=hamiltonian(epath,N);
  MILESTONE;
  // apply 2-opt on hamiltonian path;
  vector<int> TOR = two_opt(hm,mat);
  MILESTONE;
  
  printVector(TOR," ");
  cout<<endl<<getPathCost(TOR,mat, 0,TOR.size()-1)<<endl;
  
  return 0;
}