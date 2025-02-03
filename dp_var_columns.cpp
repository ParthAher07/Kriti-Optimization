#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
#define int long long

const int MAX_SIZE = 10000;
const int PRE_SIZE = 10001;
vector<vector<int>> grid(MAX_SIZE, vector<int>(MAX_SIZE, 0));
vector<vector<int>> pre(PRE_SIZE, vector<int>(PRE_SIZE, 0));

void getGrid( const string & filename ) {
    ifstream file(filename);
    int N;
    file >> N;
    for (int i = 0; i < N; ++i) {
        int x, y, value;
        file >> x >> y >> value;
        grid[x-1][y-1] += value;
    }
    int M;
    file >> M;
    for (int i = 0; i < M; ++i) {
        int x, y, value;
        file >> x >> y >> value;
        grid[x-1][y-1] -= value;
    }
    for (int i = 1; i <= MAX_SIZE; ++i) {
        for (int j = 1; j <= MAX_SIZE; ++j) {
            pre[i][j] = grid[i-1][j-1] + pre[i-1][j] + pre[i][j-1] - pre[i-1][j-1];
        }
    }
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
        max_sum = 0;
        start = 0;
        end = 0;
    }
}

int getOptimalValue() {
    int divisions = MAX_SIZE ;
    vector<int> starts1, ends1, starts2, ends2, maxes1, maxes2;

    for(int i = 0; i < MAX_SIZE; i += 1) {
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

    vector<vector<int>> dp0(divisions, vector<int>(1001, 0));
    vector<vector<int>> dp1(divisions, vector<int>(1001, 0));
    vector<vector<int>> dp2(divisions, vector<int>(1001, 0));

    for (int j = 4; j <= 1000; ++j) {
        dp1[0][j] = maxes1[0];
    }
    for (int j = 8; j <= 1000; ++j) {
        dp2[0][j] = maxes1[0] + maxes2[0];
    }

    for (int i = 1; i < divisions; ++i) {
        for (int j = 1; j <= 1000; ++j) {
            dp0[i][j] = max(dp0[i][j], dp0[i][j-1]);
            dp1[i][j] = max(dp1[i][j], dp1[i][j-1]);
            dp2[i][j] = max(dp2[i][j], dp2[i][j-1]);

            dp0[i][j] = max({dp0[i][j], dp0[i-1][j], dp1[i-1][j], dp2[i-1][j]});

            if (j >= 6) {
                dp1[i][j] = max(dp1[i][j], dp0[i-1][j-6] + maxes1[i]);
            }

            int prev_start = starts1[i-1];
            int curr_start = starts1[i];
            int prev_end = ends1[i-1];
            int curr_end = ends1[i];
            bool same_start = (prev_start == curr_start);
            bool same_end = (prev_end == curr_end);
            int cost = 0;

            if (same_start != same_end) {
                cost = 2;
            } else if (same_start && same_end) {
                cost = 0;
            } else {
                cost = 4;
            }

            if (j >= cost) {
                dp1[i][j] = max(dp1[i][j], dp1[i-1][j - cost] + maxes1[i]);
            }

            prev_start = starts1[i-1];
            curr_start = starts1[i];
            prev_end = ends1[i-1];
            curr_end = ends1[i];
            same_start = (prev_start == curr_start);
            same_end = (prev_end == curr_end);

            if (same_start != same_end) {
                cost = 2;
            } else if (same_start && same_end) {
                cost = 0;
            } else {
                cost = 4;
            }

            if (j >= cost) {
                dp1[i][j] = max(dp1[i][j], dp2[i-1][j - cost] + maxes1[i]);
            }

            if (j >= 10) {
                dp2[i][j] = max(dp2[i][j], dp0[i-1][j-10] + maxes1[i] + maxes2[i]);
            }

            prev_start = starts1[i-1];
            curr_start = starts1[i];
            prev_end = ends1[i-1];
            curr_end = ends1[i];
            same_start = (prev_start == curr_start);
            same_end = (prev_end == curr_end);

            if (same_start != same_end) {
                cost = 6;
            } else if (same_start && same_end) {
                cost = 4;
            } else {
                cost = 8;
            }

            if (j >= cost) {
                dp2[i][j] = max(dp2[i][j], dp1[i-1][j - cost] + maxes1[i] + maxes2[i]);
            }

            prev_start = starts1[i-1];
            curr_start = starts1[i];
            prev_end = ends1[i-1];
            curr_end = ends1[i];
            same_start = (prev_start == curr_start);
            same_end = (prev_end == curr_end);

            if (same_start != same_end) {
                cost = 6;
            } else if (same_start && same_end) {
                cost = 4;
            } else {
                cost = 8;
            }

            if (j >= cost) {
                dp2[i][j] = max(dp2[i][j], dp2[i-1][j - cost] + maxes1[i] + maxes2[i]);
            }
        }
    }

    return max({dp0.back()[1000], dp1.back()[1000], dp2.back()[1000]});
}

signed main() {
    string filename = "input04.txt";
    getGrid(filename);
    int ans = getOptimalValue(40);
    int ans1 = getOptimalValue(10);
    int totPos = allPositivesSum();
    cout << ans << endl;
    cout << ans1 << endl;
    cout << totPos << endl;
    printf("%.2f%%\n", (ans * 100.0) / totPos);
    printf("%.2f%%\n", (ans1 * 100.0) / totPos);
    return 0;
}