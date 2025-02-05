#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <climits>
#include <chrono> // For timing


using namespace std;

int gridsize;
int valuelimit;


struct KadaneResult {
    int maxSum;
    int start;
    int end;
};

KadaneResult kadane(const vector<int>& arr) {
    int max_so_far = 0;  // Start with 0 to ignore negative sums
    int max_ending_here = 0;
    int start = 0, end = 0;
    int temp_start = 0;

    for (int i = 0; i < arr.size(); ++i) {  // Start from i=0
        if (max_ending_here + arr[i] > 0) {
            max_ending_here += arr[i];
        } else {
            max_ending_here = 0;
            temp_start = i + 1;  // Reset start position
        }

        if (max_ending_here > max_so_far) {
            max_so_far = max_ending_here;
            start = temp_start;
            end = i;
        }
    }

    return {max_so_far, start, end};
}

bool allPositivesSum(const string& filename, int& result) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }
    int N;
    file >> N;
    int ans = 0;
    for (int i = 0; i < N; ++i) {
        int x, y, value;
        file >> x >> y >> value;
        if (value > 0) {
            ans += value;
        }
    }
    int M;
    file >> M;
    for (int i = 0; i < M; ++i) {
        int x, y, value;
        file >> x >> y >> value;
    }
    file.close();
    result = ans;
    return true;
}

bool getGrid(const string& filename, vector<vector<int>>& grid) {
    grid = vector<vector<int>>(10000, vector<int>(10000, 0));
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }
    int N;
    file >> N;
    for (int i = 0; i < N; ++i) {
        int x, y, value;
        file >> x >> y >> value;
        grid[x - 1][y - 1] += value;
    }
    int M;
    file >> M;
    for (int i = 0; i < M; ++i) {
        int x, y, value;
        file >> x >> y >> value;
        grid[x - 1][y - 1] -= value;
    }
    file.close();
    return true;
}

void processGrid(const vector<vector<int>>& grid, vector<int>& localMaxValues) {
    
    for (int i = 0; i + gridsize <= 10000; i += gridsize) {
        vector<vector<int>> subgrid(gridsize, vector<int>(10000));
        for (int row = 0; row < gridsize; ++row) {
            subgrid[row] = grid[i + row];
        }

        for (int j = 0; j < gridsize; ++j) {
            int prev_row = (j - 1 + gridsize) % gridsize;
            for (int k = 0; k < 10000; ++k) {
                subgrid[j][k] += subgrid[prev_row][k];
            }
        }

        vector<int> lastRow = subgrid[gridsize-1];
        vector<int> tempRow = lastRow;

        for (int iter = 0; iter < 5; ++iter) {
            KadaneResult result = kadane(tempRow);
            localMaxValues.push_back(result.maxSum);

            for (int idx = result.start; idx <= result.end; ++idx) {
                if (idx >= 0 && idx < 10000) {
                    tempRow[idx] = INT_MIN;
                }
            }
        }
    }
}

int main() {
    for (int i = 0; i < 10; ++i) {

        auto start = std::chrono::high_resolution_clock::now();

        ostringstream filenameStream;
        filenameStream << "input" << setw(2) << setfill('0') << i << ".txt";
        string filename = filenameStream.str();

        int fileMaxPos;
        if (!allPositivesSum(filename, fileMaxPos)) {
            cerr << "Skipping file: " << filename << endl;
            continue;
        }

        vector<vector<int>> grid;
        if (!getGrid(filename, grid)) {
            cerr << "Skipping file: " << filename << endl;
            continue;
        }

        int maxSumTop = 0;  // To store the maximum sumTop across all iterations

        for (int iter = 0; iter < 5; ++iter) {
             gridsize = 20 + iter * ((200 - 20) / 9);  // Vary gridsize between 60 and 10000
             valuelimit = (1000 - 3 * (10000/(2*gridsize))) / 4;     // Calculate valuelimit based on size
             valuelimit = max(valuelimit,166);

            vector<int> localMaxValues;
            processGrid(grid, localMaxValues);

            // Process results for this iteration
            sort(localMaxValues.rbegin(), localMaxValues.rend());
            int sumTop = 0;
            int limit = min(valuelimit, (int)localMaxValues.size());
            for (int j = 0; j < limit; ++j) {
                sumTop += localMaxValues[j];
            }

            // Update maxSumTop if the current sumTop is greater
            if (sumTop > maxSumTop) {
                maxSumTop = sumTop;
            }
        }

        double percentage = (static_cast<double>(maxSumTop) / fileMaxPos) * 100;

        cout << "File: " << filename << "\n";
        cout << "Sum of all positive values: " << fileMaxPos << "\n";
        cout << "Optimal value : " << maxSumTop << "\n";
        cout << "Percentage: " << fixed << setprecision(2) << percentage << "%\n";

        // Stop measuring time
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate the duration in milliseconds
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Time taken: " << duration.count()/1000 << " s" << std::endl;
        cout<<endl;
    }

    return 0;
}