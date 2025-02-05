#include <bits/stdc++.h>
using namespace std;

int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);cout.tie(NULL);

    freopen("random_testcase.txt" , "w" , stdout);

    vector< pair< pair<long long , long long> , long long >> stellars;
    vector< pair< pair<long long , long long> , long long >> mines;

    for( int i = 0; i < 1001; i++ ){ 
        for (int j = 0; j < 1001; j++)
        {
            if((i+j)%2 == 0) {
                stellars.push_back({{i , j} , 1});
            }
            else{
                mines.push_back({{i , j} , 1});
            }
        }
        
    }

    cout<<stellars.size()<<endl;
    for (auto &&i : stellars)
    {
        cout << i.first.first  << " " << i.first.second << " " << i.second << endl;
    }
    cout<<mines.size()<<endl;
    for (auto &&i : mines)
    {
        cout << i.first.first  << " " << i.first.second << " " << i.second << endl;
    }
    
    
    return 0;
}