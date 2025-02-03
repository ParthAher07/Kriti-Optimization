#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> grid(10000, vector<int>(10000, 0));

void getGrid(const string& filename) {
    ifstream file(filename);
    int N, M, x, y, value;
    file >> N;
    for (int i = 0; i < N; ++i) {
        file >> x >> y >> value;
        grid[x - 1][y - 1] += value;
    }
    file >> M;
    for (int i = 0; i < M; ++i) {
        file >> x >> y >> value;
        grid[x - 1][y - 1] -= value;
    }
}

int allPositivesSum() {
    int ans = 0;
    for (int i = 0; i < 10000; ++i) {
        for (int j = 0; j < 10000; ++j) {
            if (grid[i][j] > 0) {
                ans += grid[i][j];
            }
        }
    }
    return ans;
}

tuple<int, vector<vector<vector<int>>>, vector<int>, vector<int>, vector<int>, vector<int>, vector<int>, vector<int>, int>
getOptimalValue(int rows) {
    auto copyRows = [&](int startRow) {
        vector<vector<int>> newGrid(rows, vector<int>(10000, 0));
        for (int i = 0; i < rows; ++i) {
            newGrid[i] = grid[startRow + i];
        }
        return newGrid;
    };

    int divisions = 10000 / rows;
    vector<int> starts1, ends1, starts2, ends2, maxes1, maxes2;

    for (int i = 0; i < 10000; i += rows) {
        auto subgrid = copyRows(i);
        for (int j = 1; j < rows; ++j) {
            for (int k = 0; k < 10000; ++k) {
                subgrid[j][k] += subgrid[j - 1][k];
            }
        }

        int start = 0, end = 0, tempStart = 0, currMax = 0, currOpt = 0;
        for (int j = 0; j < 10000; ++j) {
            currMax += subgrid[rows - 1][j];
            if (currMax > currOpt) {
                currOpt = currMax;
                start = tempStart;
                end = j;
            }
            if (currMax < 0) {
                tempStart = j + 1;
                currMax = 0;
            }
        }
        starts1.push_back(start);
        ends1.push_back(end);
        maxes1.push_back(currOpt);

        for (int j = start; j <= end; ++j) {
            subgrid[rows - 1][j] = -1e9;
        }

        start = 0, end = 0, tempStart = 0, currMax = 0, currOpt = 0;
        for (int j = 0; j < 10000; ++j) {
            currMax += subgrid[rows - 1][j];
            if (currMax > currOpt) {
                currOpt = currMax;
                start = tempStart;
                end = j;
            }
            if (currMax < 0) {
                tempStart = j + 1;
                currMax = 0;
            }
        }
        starts2.push_back(start);
        ends2.push_back(end);
        maxes2.push_back(currOpt);
    }

    vector<vector<vector<int>>> dp(3, vector<vector<int>>(divisions, vector<int>(1001, 0)));
    dp[1][0][4] = maxes1[0];
    dp[2][0][8] = maxes1[0] + maxes2[0];

    for (int i = 1; i < divisions; ++i) {
        for (int j = 1; j <= 1001; ++j) {
            dp[0][i][j] = max({dp[0][i][j], dp[0][i - 1][j], dp[1][i - 1][j], dp[2][i - 1][j]});

            if (j >= 6) {
                dp[1][i][j] = max(dp[1][i][j], dp[0][i - 1][j - 6] + maxes1[i]);
            }

            if ((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])) {
                if (j >= 2) {
                    dp[1][i][j] = max(dp[1][i][j], dp[1][i - 1][j - 2] + maxes1[i]);
                }
            } else if ((starts1[i - 1] == starts1[i]) && (ends1[i - 1] == ends1[i])) {
                dp[1][i][j] = max(dp[1][i][j], dp[1][i - 1][j] + maxes1[i]);
            } else {
                if (j >= 4) {
                    dp[1][i][j] = max(dp[1][i][j], dp[1][i - 1][j - 4] + maxes1[i]);
                }
            }

            if ((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])) {
                if (j >= 2) {
                    dp[1][i][j] = max(dp[1][i][j], dp[2][i - 1][j - 2] + maxes1[i]);
                }
            } else if ((starts1[i - 1] == starts1[i]) && (ends1[i - 1] == ends1[i])) {
                dp[1][i][j] = max(dp[1][i][j], dp[2][i - 1][j] + maxes1[i]);
            } else {
                if (j >= 4) {
                    dp[1][i][j] = max(dp[1][i][j], dp[2][i - 1][j - 4] + maxes1[i]);
                }
            }

            if (j >= 10) {
                dp[2][i][j] = max(dp[2][i][j], dp[0][i - 1][j - 10] + maxes1[i] + maxes2[i]);
            }

            if ((starts1[i - 1] == starts1[i]) && (ends1[i - 1] == ends1[i])) {
                if (j >= 6) {
                    dp[2][i][j] = max(dp[2][i][j], dp[1][i - 1][j - 6] + maxes1[i] + maxes2[i]);
                }
            } else {
                if (j >= 8) {
                    dp[2][i][j] = max(dp[2][i][j], dp[1][i - 1][j - 8] + maxes1[i] + maxes2[i]);
                }
            }

            if ((starts1[i - 1] == starts1[i]) && (ends1[i - 1] == ends1[i])) {
                if (j >= 6) {
                    dp[2][i][j] = max(dp[2][i][j], dp[2][i - 1][j - 6] + maxes1[i] + maxes2[i]);
                }
            } else {
                if (j >= 8) {
                    dp[2][i][j] = max(dp[2][i][j], dp[2][i - 1][j - 8] + maxes1[i] + maxes2[i]);
                }
            }
        }
    }

    int optimum = max({dp[0][divisions - 1][1000], dp[1][divisions - 1][1000], dp[2][divisions - 1][1000]});
    optimum = 0;
    int used = -1;
    for (int i = 1; i <= 1001; ++i) {
        if (max({dp[0][divisions - 1][i], dp[1][divisions - 1][i], dp[2][divisions - 1][i]}) >= optimum) {
            optimum = max({dp[0][divisions - 1][i], dp[1][divisions - 1][i], dp[2][divisions - 1][i]});
            used = i;
        }
    }
    return make_tuple(optimum, dp, maxes1, maxes2, starts1, ends1, starts2, ends2, used);
}

vector<int> getSubarrays(int rows, vector<vector<vector<int>>>& dp, int used, vector<int>& maxes1, vector<int>& maxes2, vector<int>& starts1, vector<int>& ends1) {
    int divisions = 10000 / rows;
    int i = -1, j = divisions - 1, k = used;
    vector<int> selected(divisions, -1);

    while (j >= 0) {
        int which = i;
        if (which == -1) {
            if ((dp[0][j][k] >= dp[1][j][k]) && (dp[0][j][k] >= dp[2][j][k])) {
                which = 0;
            } else if ((dp[1][j][k] >= dp[0][j][k]) && (dp[1][j][k] >= dp[2][j][k])) {
                which = 1;
            } else {
                which = 2;
            }
        }

        selected[j] = which;

        if (which == 0) {
            if ((dp[0][j - 1][k] >= dp[1][j - 1][k]) && (dp[0][j - 1][k] >= dp[2][j - 1][k])) {
                i = 0;
                j -= 1;
            } else if ((dp[1][j - 1][k] >= dp[0][j - 1][k]) && (dp[1][j - 1][k] >= dp[2][j - 1][k])) {
                i = 1;
                j -= 1;
            } else {
                i = 2;
                j -= 1;
            }
        } else if (which == 1) {
            int m = 0;
            if (k >= 6) {
                m = max(m, dp[0][j - 1][k - 6] + maxes1[j]);
            }
            if ((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])) {
                if (k >= 2) {
                    m = max(m, dp[1][j - 1][k - 2] + maxes1[j]);
                }
            } else if ((starts1[j - 1] == starts1[j]) && (ends1[j - 1] == ends1[j])) {
                m = max(m, dp[1][j - 1][k] + maxes1[j]);
            } else {
                if (k >= 4) {
                    m = max(m, dp[1][j - 1][k - 4] + maxes1[j]);
                }
            }

            if ((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])) {
                if (k >= 2) {
                    m = max(m, dp[2][j - 1][k - 2] + maxes1[j]);
                }
            } else if ((starts1[j - 1] == starts1[j]) && (ends1[j - 1] == ends1[j])) {
                m = max(m, dp[2][j - 1][k] + maxes1[j]);
            } else {
                if (k >= 4) {
                    m = max(m, dp[2][j - 1][k - 4] + maxes1[j]);
                }
            }

            bool done = false;
            if (k >= 6) {
                if (m == dp[0][j - 1][k - 6] + maxes1[j]) {
                    i = 0;
                    j -= 1;
                    k -= 6;
                    done = true;
                }
            }
            if (!done) {
                if ((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])) {
                    if (k >= 2) {
                        if (m == dp[1][j - 1][k - 2] + maxes1[j]) {
                            i = 1;
                            j -= 1;
                            k -= 2;
                            done = true;
                        }
                    }
                } else if ((starts1[j - 1] == starts1[j]) && (ends1[j - 1] == ends1[j])) {
                    if (m == dp[1][j - 1][k] + maxes1[j]) {
                        i = 1;
                        j -= 1;
                        k -= 2;
                        done = true;
                    }
                } else {
                    if (k >= 4) {
                        if (m == dp[1][j - 1][k - 4] + maxes1[j]) {
                            i = 1;
                            j -= 1;
                            k -= 4;
                            done = true;
                        }
                    }
                }
            }
            if (!done) {
                if ((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])) {
                    if (k >= 2) {
                        if (m == dp[2][j - 1][k - 2] + maxes1[j]) {
                            i = 2;
                            j -= 1;
                            k -= 2;
                            done = true;
                        }
                    }
                } else if ((starts1[j - 1] == starts1[j]) && (ends1[j - 1] == ends1[j])) {
                    if (m == dp[2][j - 1][k] + maxes1[j]) {
                        i = 2;
                        j -= 1;
                        done = true;
                    }
                } else {
                    if (k >= 4) {
                        if (m == dp[2][j - 1][k - 4] + maxes1[j]) {
                            i = 2;
                            j -= 1;
                            k -= 4;
                            done = true;
                        }
                    }
                }
            }
        } else {
            int m = 0;
            if (k >= 10) {
                m = max(m, dp[0][j - 1][k - 10] + maxes1[j] + maxes2[j]);
            }

            if ((starts1[j - 1] == starts1[j]) && (ends1[j - 1] == ends1[j])) {
                if (k >= 6) {
                    m = max(m, dp[1][j - 1][k - 6] + maxes1[j] + maxes2[j]);
                }
            } else {
                if (k >= 8) {
                    m = max(m, dp[1][j - 1][k - 8] + maxes1[j] + maxes2[j]);
                }
            }

            if ((starts1[j - 1] == starts1[j]) && (ends1[j - 1] == ends1[j])) {
                if (k >= 6) {
                    m = max(m, dp[2][j - 1][k - 6] + maxes1[j] + maxes2[j]);
                }
            } else {
                if (k >= 8) {
                    m = max(m, dp[2][j - 1][k - 8] + maxes1[j] + maxes2[j]);
                }
            }

            bool done = false;
            if (k >= 10) {
                if (m == dp[0][j - 1][k - 10] + maxes1[j] + maxes2[j]) {
                    i = 0;
                    j -= 1;
                    k -= 10;
                    done = true;
                }
            }
            if (!done) {
                if ((starts1[j - 1] == starts1[j]) && (ends1[j - 1] == ends1[j])) {
                    if (k >= 6) {
                        if (m == dp[1][j - 1][k - 6] + maxes1[j] + maxes2[j]) {
                            i = 1;
                            j -= 1;
                            k -= 6;
                            done = true;
                        }
                    }
                } else {
                    if (k >= 8) {
                        if (m == dp[1][j - 1][k - 8] + maxes1[j] + maxes2[j]) {
                            i = 1;
                            j -= 1;
                            k -= 8;
                            done = true;
                        }
                    }
                }
            }
            if (!done) {
                if ((starts1[j - 1] == starts1[j]) && (ends1[j - 1] == ends1[j])) {
                    if (k >= 6) {
                        if (m == dp[2][j - 1][k - 6] + maxes1[j] + maxes2[j]) {
                            i = 2;
                            j -= 1;
                            k -= 6;
                            done = true;
                        }
                    }
                } else {
                    if (k >= 8) {
                        if (m == dp[2][j - 1][k - 8] + maxes1[j] + maxes2[j]) {
                            i = 2;
                            j -= 1;
                            k -= 8;
                            done = true;
                        }
                    }
                }
            }
        }
    }
    return selected;
}

int main(){

    string filename = "input02.txt";
    getGrid(filename);
    
    // Assuming getOptimalValue and getSubarrays are defined and return appropriate values
    int optimum, used;
    vector<vector<vector<int>>> dp;
    vector<int> maxes1, maxes2, starts1, ends1, starts2, ends2;

    tie(optimum, dp, maxes1, maxes2, starts1, ends1, starts2, ends2, used) = getOptimalValue(40);
    int totPos = allPositivesSum();
    vector<int> selected = getSubarrays(40, dp, used, maxes1, maxes2, starts1, ends1);
    
    ofstream file("subarrays.txt");
    for (int i = 0; i < 250; ++i) {
        file << starts1[i] << ", " << ends1[i] << "; " << starts2[i] << ", " << ends2[i] << " : " << selected[i] << "\n";
    }
    
    int ans = optimum;
    int verify = 0;
    for (int i = 0; i < 250; ++i) {
        if (selected[i] == 1) {
            verify += maxes1[i];
        } else if (selected[i] == 2) {
            verify += maxes1[i] + maxes2[i];
        }
    }
    
    cout << ans << endl;
    cout << verify << endl;
    cout << totPos << endl;
    cout << (static_cast<double>(ans) / totPos) * 100 << "%" << endl;

    return 0;
    
}