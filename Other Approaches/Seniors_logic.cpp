#include <bits/stdc++.h>
using namespace std;
#define int long long

long long Grid[10001][10001];
long long prefix[10001][10001];
vector<pair<int, vector<int>>> ans;

void init()
{
    for (int i = 1; i <= 10000; i++)
    {
        for (int j = 1; j <= 10000; j++)
        {
            Grid[i][j] = 0;
        }
    }
    for (int i = 0; i <= 10000; i++)
    {
        for (int j = 0; j <= 10000; j++)
        {
            prefix[i][j] = 0;
        }
    }
}

void Calculate(int x, int y, int mul)
{
    int add = mul * 16;
    int calc = 0;
    calc = prefix[x + add][y + add] - prefix[x + add][y - 1] - prefix[x - 1][y + add] + prefix[x - 1][y - 1];
    if (calc > 0)
        ans.push_back({calc, {x, y, add}});
}
bool Check(vector<int> &arr1, vector<int> &arr2)
{
    if (arr1[0] >= arr2[0] && arr1[1] >= arr2[1] && arr1[0] <= arr2[0] + arr2[2] && arr1[1] <= arr2[1] + arr2[2])
    {
        return false;
    }
    else if (arr1[0] + arr1[2] >= arr2[0] && arr1[1] >= arr2[1] && arr1[0] + arr1[2] <= arr2[0] + arr2[2] && arr1[1] <= arr2[1] + arr2[2])
    {
        return false;
    }
    else if (arr1[0] + arr1[2] >= arr2[0] && arr1[1] + arr1[2] >= arr2[1] && arr1[0] + arr1[2] <= arr2[0] + arr2[2] && arr1[1] + arr1[2] <= arr2[1] + arr2[2])
    {
        return false;
    }
    else if (arr1[0] >= arr2[0] && arr1[1] + arr1[2] >= arr2[1] && arr1[0] <= arr2[0] + arr2[2] && arr1[1] + arr1[2] <= arr2[1] + arr2[2])
    {
        return false;
    }
    return true;
};

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
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] + Grid[i][j] - prefix[i - 1][j - 1];
        }
    }

    for (int i = 1; i <= 4; i++)
    {
        for (int x = 1; x <= 10000; x++)
        {
            for (int y = 1; y <= 10000; y++)
            {
                if (x + 16 * i <= 10000 && y + 16 * i <= 10000)
                    Calculate(x, y, i);
            }
        }
    }
    sort(ans.begin(), ans.end());
    reverse(ans.begin(), ans.end());

    int index = 0;
    int final = 0;
    vector<vector<int>> ans1;
    for (auto &&i : ans)
    {
        if (index == 100)
            break;
        int sz = ans1.size();
        int flag = 1;
        for (int j = 0; j < sz; j++)
        {
            flag = Check(i.second, ans1[j]);
            if (!flag)
                break;
        }
        if (flag)
        {
            index++;
            ans1.push_back(i.second);
            final += i.first;
        }
    }
    cout << final << '\n';
}

