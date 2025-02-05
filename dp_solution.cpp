#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
#define int long long

// TO BE CHANGED TO 10001
const int MAX_SIZE = 10000;
// const int PRE_SIZE = 10001;
vector<vector<int>> grid(MAX_SIZE, vector<int>(MAX_SIZE, 0));
vector<vector<int>> copygrid(MAX_SIZE, vector<int>(MAX_SIZE, 0));
// vector<vector<int>> pre(PRE_SIZE, vector<int>(PRE_SIZE, 0));

void getGrid(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }
    int N;
    file >> N;
    for (int _ = 0; _ < N; ++_) {
        int x, y, value;
        file >> x >> y >> value;
        grid[x-1][y-1] += value;
    }
    int M;
    file >> M;
    for (int _ = 0; _ < M; ++_) {
        int x, y, value;
        file >> x >> y >> value;
        grid[x-1][y-1] -= value;
    }
    // for (int i = 1; i <= MAX_SIZE; ++i) {
    //     for (int j = 1; j <= MAX_SIZE; ++j) {
    //         pre[i][j] = grid[i-1][j-1] + pre[i-1][j] + pre[i][j-1] - pre[i-1][j-1];
    //     }
    // }
}

int allPositivesSum() {
    int ans = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if (grid[i][j] > 0) {
                ans += grid[i][j];
            }
        }
    }
    return ans;
}

void kadane(const vector<int>& arr, int& start, int& end, int& max_sum) {
    int current_max = 0;
    max_sum = 0;
    start = 0;
    end = -1;
    int temp_start = 0;

    for (int i = 0; i < arr.size(); ++i) {
        current_max += arr[i];
        if (current_max > max_sum) {
            max_sum = current_max;
            start = temp_start;
            end = i;
        }
        if (current_max < 0) {
            current_max = 0;
            temp_start = i + 1;
        }
    }

    if (end == -1) { // All elements were negative
        max_sum = -1e8;
        start = 0;
        end = 0;
    }
}

int checkans(int rows, vector<pair<int,int>> &segments, vector<int> &indices){
    
    int verified=0;
    int n = segments.size();

    for(int i=0; i<n; i++){
        int start = segments[i].first;
        int end = segments[i].second;
        int x = indices[i];
        int temp=0;

        for(int j=rows*x; j < (x+1)*rows; j++){
            for(int k=start; k<=end; k++){
                temp += copygrid[j][k];
            }
        }
        verified += temp;
    }

    return verified;
}


vector<int> starts1, ends1, starts2, ends2, maxes1, maxes2;
vector<vector<int>> dp0;
vector<vector<int>> dp1;
vector<vector<int>> dp2;

pair<int,int> getOptimalValue(int rows) {
    int divisions = MAX_SIZE / rows;
    dp1.assign(divisions, vector<int>(1001, 0));
    dp2.assign(divisions, vector<int>(1001, 0));
    dp0.assign(divisions, vector<int>(1001, 0));
    
    for (int i = 0; i < MAX_SIZE; i += rows) {
        vector<vector<int>> subgrid(rows, vector<int>(MAX_SIZE, 0));
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < MAX_SIZE; ++c) {
                subgrid[r][c] = grid[i + r][c];
            }
        }

        for (int r = 1; r < rows; ++r) {
            for (int c = 0; c < MAX_SIZE; ++c) {
                subgrid[r][c] += subgrid[r-1][c];
            }
        }

        int start, end, max_sum;
        kadane(subgrid.back(), start, end, max_sum);
        starts1.push_back(start);
        ends1.push_back(end);
        maxes1.push_back(max_sum);

        for (int c = start; c <= end; ++c) {
            subgrid.back()[c] = INT_MIN;
        }

        kadane(subgrid.back(), start, end, max_sum);
        starts2.push_back(start);
        ends2.push_back(end);
        maxes2.push_back(max_sum);
    }

    // vector<vector<int>> dp0(divisions, vector<int>(1002, 0));
    // vector<vector<int>> dp1(divisions, vector<int>(1002, 0));
    // vector<vector<int>> dp2(divisions, vector<int>(1002, 0));


    dp1[0][0] = dp1[0][1] = dp1[0][2] = dp1[0][3] = -1e8;
    dp2[0][0] = dp2[0][1] = dp2[0][2] = dp2[0][3] = dp2[0][4] = dp2[0][5] = dp2[0][6] = dp2[0][7] = -1e8;
    // for (int j = 4; j <= 1000; ++j) {
    //     dp1[0][j] = maxes1[0];
    // }
    dp1[0][4] = maxes1[0];
    // for (int j = 8; j <= 1000; ++j) {
    //     dp2[0][j] = maxes1[0] + maxes2[0];
    // }
    dp2[0][8] = maxes1[0] + maxes2[0];

    for (int i = 1; i < divisions; ++i) {
        for (int j = 1; j <= 1000; ++j) {
            // dp0[i][j] = max(dp0[i][j], dp0[i][j-1]);
            // dp1[i][j] = max(dp1[i][j], dp1[i][j-1]);
            // dp2[i][j] = max(dp2[i][j], dp2[i][j-1]);

            // dp0
            dp0[i][j] = max({dp0[i][j], dp0[i-1][j], dp1[i-1][j], dp2[i-1][j]});


            // dp1
            // From dp0[i - 1]
            if (j >= 6) {
                dp1[i][j] = max(dp1[i][j], dp0[i-1][j-6] + maxes1[i]);
            }

            // From dp1[i - 1]
            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])){
                if(j >= 2) dp1[i][j] = max(dp1[i][j], dp1[i - 1][j - 2] + maxes1[i]);
            }
            else if((starts1[i - 1] == starts1[i]) && (ends1[i - 1] == ends1[i])){
                dp1[i][j] = max(dp1[i][j], dp1[i - 1][j] + maxes1[i]);
            }
            else{
                if(j >= 4) dp1[i][j] = max(dp1[i][j], dp1[i - 1][j - 4] + maxes1[i]);
            }

            // From dp2[i - 1]
            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])){
                if(j >= 2) dp1[i][j] = max(dp1[i][j], dp2[i - 1][j - 2] + maxes1[i]);
            }
            else if((starts1[i - 1] == starts1[i]) && (ends1[i - 1] == ends1[i])){
                dp1[i][j] = max(dp1[i][j], dp2[i - 1][j] + maxes1[i]);
            }
            else{
                if(j >= 4) dp1[i][j] = max(dp1[i][j], dp2[i - 1][j - 4] + maxes1[i]);
            }



            // dp2
            // From dp0[i - 1]
            if (j >= 10) {
                dp2[i][j] = max(dp2[i][j], dp0[i-1][j-10] + maxes1[i] + maxes2[i]);
            }

            // From dp1[i - 1]
            if(starts1[i - 1] == starts1[i] && ends1[i - 1] == ends1[i]){
                if(j >= 6) dp2[i][j] = max(dp2[i][j], dp1[i - 1][j - 6] + maxes1[i] + maxes2[i]);
            }
            else{
                if(j >= 8) dp2[i][j] = max(dp2[i][j], dp1[i - 1][j - 8] + maxes1[i] + maxes2[i]);
            }

            // From dp2[i - 1]
            if(starts1[i - 1] == starts1[i] && ends1[i - 1] == ends1[i]){
                if(j >= 6) dp2[i][j] = max(dp2[i][j], dp2[i - 1][j - 6] + maxes1[i] + maxes2[i]);
            }
            else{
                if(j >= 8) dp2[i][j] = max(dp2[i][j], dp2[i - 1][j - 8] + maxes1[i] + maxes2[i]);
            }
        }
    }

    int optimum = 0;
    int used = -1;
    for (int i=1; i<=1000; i++){
        int temp = max({dp0[divisions - 1][i], dp1[divisions - 1][i], dp2[divisions - 1][i]});
        if (temp >= optimum){
            optimum = temp;
            used = i;
        }
    }

    return {optimum,used};
}

vector<pair<int, int>> getSubarrays(int rows, int used) {
    int divisions = 10000 / rows;
    int i = -1, j = divisions - 1, k = used;
    vector<pair<int, int>> selected(divisions);
    int mx = max({dp0[j][k], dp1[j][k], dp2[j][k]});
    if(mx == dp0[j][k]) i = 0;
    else if(mx == dp1[j][k]) i = 1;
    else i = 2;
    
    while (j >= 0) { // >= ki jagah > krdiya handle the case of j==0 separtely
        // cout<<j<<endl;
        int which = i;
        
        selected[j] = {i, k};
        if(j == 0){
            break;
        }

        if (which == 0) {
            int m = dp0[j][k];
            if (m == dp0[j - 1][k]) {
                i = 0;
            } else if (m == dp1[j - 1][k]) {
                i = 1;
            } else {
                i = 2;
            }
            j -= 1;
            continue;
        }
        
        else if (which == 1){
            int m = dp1[j][k];

            // From dp0[j - 1] ???
            if (k >= 6 && m == dp0[j - 1][k - 6] + maxes1[j]){
                i = 0; j -= 1; k -= 6;
                continue;
            }
            // From dp1[j - 1] ???
            if ((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])) {
                if (k >= 2 && m == dp1[j - 1][k - 2] + maxes1[j]){
                    i = 1; j -= 1; k -= 2;
                    continue;
                }
            }
            else if (starts1[j - 1] == starts1[j] && ends1[j - 1] == ends1[j]) {
                if(m == dp1[j - 1][k] + maxes1[j]){
                    i = 1; j -= 1;
                    continue;
                }
            }
            else {
                if (k >= 4 && m == dp1[j - 1][k - 4] + maxes1[j]){
                    i = 1; j -= 1; k -= 4;
                    continue;
                }                
            }
            // From dp2[j - 1] ???
            if ((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])) {
                if (k >= 2 && m == dp2[j - 1][k - 2] + maxes1[j]){
                    i = 2; j -= 1; k -= 2;
                    continue;
                }
            }
            else if (starts1[j - 1] == starts1[j] && ends1[j - 1] == ends1[j]) {
                if(m == dp2[j - 1][k] + maxes1[j]){
                    i = 2; j -= 1;
                    continue;
                }
            }
            else {
                if (k >= 4 && m == dp2[j - 1][k - 4] + maxes1[j]){
                    i = 2; j -= 1; k -= 4;
                    continue;
                }                
            }
        }
        else {
            int m = dp2[j][k];

            // From dp0[j - 1] ???
            if (k >= 10 && m == dp0[j - 1][k - 10] + maxes1[j] + maxes2[j]) {
                i = 0; j -= 1; k -= 10;
                continue;
            }
            // From dp1[j - 1] ???
            if (starts1[j - 1] == starts1[j] && ends1[j - 1] == ends1[j]) {
                if (k >= 6 && m == dp1[j - 1][k - 6] + maxes1[j] + maxes2[j]){
                    i = 1; j -= 1; k -= 6;
                    continue;
                }
            }
            else {
                if (k >= 8 && m == dp1[j - 1][k - 8] + maxes1[j] + maxes2[j]){
                    i = 1; j -= 1; k -= 8;
                    continue;
                }
            }
            // From dp2[j - 1] ???
            if (starts1[j - 1] == starts1[j] && ends1[j - 1] == ends1[j]) {
                if (k >= 6 && m == dp2[j - 1][k - 6] + maxes1[j] + maxes2[j]){
                    i = 2; j -= 1; k -= 6;
                    continue;
                }
            }
            else {
                if (k >= 8 && m == dp2[j - 1][k - 8] + maxes1[j] + maxes2[j]){
                    i = 2; j -= 1; k -= 8;
                    continue;
                }
            }
        }
    }
    return selected;

}


signed main(){
    string filename = "input09.txt";
    getGrid(filename);

    copygrid = grid;



    pair<int,int> temp = getOptimalValue(40);
    int ans = temp.first;
    int used = temp.second;
    cout << used << "\n";
    vector<pair<int, int>> selected = getSubarrays(40,used);
    // vector<int> selected(40);

    ofstream out("subarrays.txt");
    if (!out) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    for(int i = 0; i < 250; i++){
        out << starts1[i] << ", " << ends1[i] << "; " << starts2[i] << ", " << ends2[i] << " : " << selected[i].first << "\n";
    }

    int totPos = allPositivesSum();

    int veri=0;
    for(int i = 0; i < selected.size(); ++i){
        if(selected[i].first == 1) veri += maxes1[i];
        // if(veri == ans){
        //     cout << i << "\n";
        //     break;
        // }
        else if(selected[i].first == 2){
            veri += maxes1[i];
            // if(veri == ans){
            //     cout << i << "\n";
            //     break;
            // }
            veri += maxes2[i];
            // if(veri == ans){
            //     cout << i << "\n";
            //     break;
            // }
        }
    }

    cout << ans << endl;
    cout << veri << endl;

    cout<<  (ans * 100.0) / totPos<<endl;
    cout<<  (veri * 100.0) / totPos<<endl;

    cout << totPos << endl;

    // Open a file to write the output
    // ofstream outputFile("outputcpp.txt");
    // if (outputFile.is_open()) {
    //     outputFile << "starts1 array:\n";
    //     for (int start : starts1) {
    //         outputFile << start << " ";
    //     }
    //     outputFile << "\nends1 array:\n";
    //     for (int end : ends1) {
    //         outputFile << end << " ";
    //     }
    //     outputFile.close();
    //     cout << "Output written to output.txt" << endl;
    // } else {
    //     cerr << "Unable to open file for writing" << endl;
    // }
    
    return 0;
}