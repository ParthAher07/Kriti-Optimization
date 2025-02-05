#include <bits/stdc++.h>
using namespace std;
#define int long long

long long Grid[10001][10001];
long long prefix[10001][10001];
vector<pair<int, vector<int>>> ans;
 
struct Edges
{
    pair<int,int> start;
    pair<int,int> end;
    bool direc;
};

void init()
{
    for (int i = 1; i <= 10000; i++)
    {
        for (int j = 1; j <= 10000; j++)
        {
            Grid[i][j] = 0;
            prefix[i][j] = 0;
        }
    }
    for (int i = 0; i <= 10000; i++)
    {
        prefix[0][i] = 0;
        prefix[i][0] = 0;
    }
}

bool comp(pair<int,Edges> p1, pair<int,Edges> p2)
{
    if(p1.first < p2.first) return 1;
    else if(p1.first > p2.first) return 0;

    if(p1.second.start.first<p2.second.start.first) return 1;
    else return 0;
}

void ExtendEdges(vector<pair<int,Edges>> &edges, int index)
{
    if(edges[index].second.direc == 0) return;
    int start_y = edges[index].second.start.second;
    int sz = edges.size();
    int end_y = 10000;
    for (int i = index+1; i < sz; i++)
    {
        // Here check if > works or >= according to the final model built
        if(edges[i].second.start.first > edges[index].second.end.first 
                        || edges[i].second.end.first < edges[index].second.start.first) continue;
        
        end_y = edges[i].second.start.second;
        break;
    }

    int req_y = start_y;
    int maxm = 0;
    // Again check < works or <= according to the final model built
    for (int i = start_y+1; i < end_y; i++)
    {
        int extra = prefix[edges[index].second.end.first] - prefix[edges[index].second.start.first - 1];
        if(extra > maxm)
        {
            req_y = i;
            maxm = extra;
        }
    }
    edges[index].second.start.second = req_y;
    edges[index].second.end.second = req_y;
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    init();

    vector<pair<int, int>> stellars, mines;
    int N;
    cin >> N;
    for (int i = 0; i < N; i++)
    {
        int x, y, value;
        cin >> x >> y >> value;
        stellars.push_back({x, y});
        Grid[x][y] = value;
    }
    int M;
    cin >> M;
    for (int i = 0; i < M; i++)
    {
        int x, y, value;
        cin >> x >> y >> value;
        value *= -1;
        mines.push_back({x, y});
        Grid[x][y] = value;
    }

    for (int i = 1; i < 10001; i++)
    {
        for (int j = 1; j < 10001; j++)
        {
            prefix[i][j] += prefix[i][j-1] + Grid[i][j];
        }
    }

    vector<pair<int,Edges>> edges;
    for (int i = 0; i < 500; i++)
    {
        pair<int,int> start,end;
        bool direc;
        cin>>start.first>>start.second;
        cin>>end.first>>end.second;
        cin>>direc;
        struct Edges obj;
        obj.direc = direc;
        obj.start = start;
        obj.end = end;
        edges.push_back(make_pair(start.second,obj));
    }
    
    sort(edges.begin(),edges.end(),comp);

    int sz = edges.size();
    for (int i = 0; i < sz; i++)
    {
        ExtendEdges(edges,i);
    }
}
