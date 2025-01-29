grid = [[0] * 10000 for _ in range(10000)]

def getGrid(filename):
    with open(filename, 'r') as file:
        N = int(file.readline().strip())
        for _ in range(N):
            x, y, value = map(int, file.readline().strip().split())
            grid[x - 1][y - 1] += value
        M = int(file.readline().strip())
        for _ in range(M):
            x, y, value = map(int, file.readline().strip().split())
            grid[x - 1][y - 1] -= value
    return grid

def allPositivesSum():
    ans = 0
    for i in range(10000):
        for j in range(10000):
            if grid[i][j] > 0:
                ans += grid[i][j]
    return ans

def getOptimalValue(rows):
    def copyRows(startRow):
        newGrid = [[0]*10000 for _ in range(rows)]
        for i in range(rows):
            newGrid[i] = grid[startRow + i][:]
        return newGrid
    divisions = 10000//rows
    starts1 = list()
    ends1 = list()
    starts2 = list()
    ends2 = list()
    maxes1 = list()
    maxes2 = list()
    for i in range(0, 10000, rows):
        subgrid = copyRows(i)
        for j in range(rows):
            for k in range(10000):
                subgrid[j][k] += subgrid[j - 1][k]
        start = 0
        end = 0
        tempStart = 0
        currMax = 0
        currOpt = 0
        for j in range(10000):
            currMax += subgrid[rows - 1][j]
            if currMax > currOpt:
                currOpt = currMax
                start = tempStart
                end = j
            if currMax < 0:
                tempStart = j + 1
                currMax = 0
        starts1.append(start)
        ends1.append(end)
        maxes1.append(currOpt)
        for j in range(start, end + 1):
            subgrid[rows - 1][j] = -1e18
        start = 0
        end = 0
        tempStart = 0
        currMax = 0
        currOpt = 0
        for j in range(10000):
            currMax += subgrid[rows - 1][j]
            if currMax > currOpt:
                currOpt = currMax
                start = tempStart
                end = j
            if currMax < 0:
                tempStart = j + 1
                currMax = 0
        starts2.append(start)
        ends2.append(end)
        maxes2.append(currOpt)
    dp0 = [[0]*1001 for _ in range(divisions)]
    dp1 = [[0]*1001 for _ in range(divisions)]
    dp2 = [[0]*1001 for _ in range(divisions)]
    for i in range(4, 1001):
        dp1[0][i] = maxes1[0]
    for i in range(8, 1001):
        dp2[0][i] = maxes1[0] + maxes2[0]
    for i in range(1, divisions):
        for j in range(1, 1001):
            dp0[i][j] = max(dp0[i][j], dp0[i][j - 1])
            dp1[i][j] = max(dp1[i][j], dp1[i][j - 1])
            dp2[i][j] = max(dp2[i][j], dp2[i][j - 1])

            # dp0
            dp0[i][j] = max(dp0[i - 1][j], dp1[i - 1][j], dp2[i - 1][j])

            # dp1
            if(j >= 6):
                dp1[i][j] = max(dp1[i][j], dp0[i - 1][j - 6] + maxes1[i])
            
            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])):
                if(j >= 2):
                    dp1[i][j] = max(dp1[i][j], dp1[i - 1][j - 2] + maxes1[i])
            elif((starts1[i - 1] == starts1[i]) and (ends1[i - 1] == ends1[i])):
                dp1[i][j] = max(dp1[i][j], dp1[i - 1][j] + maxes1[i])
            else:
                if(j >= 4):
                    dp1[i][j] = max(dp1[i][j], dp1[i - 1][j - 4] + maxes1[i])
            
            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])):
                if(j >= 2):
                    dp1[i][j] = max(dp1[i][j], dp2[i - 1][j - 2] + maxes1[i])
            elif((starts1[i - 1] == starts1[i]) and (ends1[i - 1] == ends1[i])):
                dp1[i][j] = max(dp1[i][j], dp2[i - 1][j] + maxes1[i])
            else:
                if(j >= 4):
                    dp1[i][j] = max(dp1[i][j], dp2[i - 1][j - 4] + maxes1[i])

            # dp2
            if(j >= 12):
                dp2[i][j] = max(dp2[i][j], dp0[i - 1][j - 12] + maxes1[i] + maxes2[i])

            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])):
                if(j >= 8):
                    dp2[i][j] = max(dp2[i][j], dp1[i - 1][j - 8] + maxes1[i] + maxes2[i])
            elif((starts1[i - 1] == starts1[i]) and (ends1[i - 1] == ends1[i])):
                if(j >= 6):
                    dp2[i][j] = max(dp2[i][j], dp1[i - 1][j - 6] + maxes1[i] + maxes2[i])
            else:
                if(j >= 10):
                    dp2[i][j] = max(dp2[i][j], dp1[i - 1][j - 10] + maxes1[i] + maxes2[i])

            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])):
                if(j >= 8):
                    dp2[i][j] = max(dp2[i][j], dp2[i - 1][j - 8] + maxes1[i] + maxes2[i])
            elif((starts1[i - 1] == starts1[i]) and (ends1[i - 1] == ends1[i])):
                if(j >= 6):
                    dp2[i][j] = max(dp2[i][j], dp2[i - 1][j - 6] + maxes1[i] + maxes2[i])
            else:
                if(j >= 10):
                    dp2[i][j] = max(dp2[i][j], dp2[i - 1][j - 10] + maxes1[i] + maxes2[i])
            

    optimum = max(dp0[divisions - 1][1000], dp1[divisions - 1][1000], dp2[divisions - 1][1000])
    return optimum

if __name__ == "__main__":
    filename = 'input09.txt'
    getGrid(filename)
    ans = getOptimalValue(40)
    ans = max(ans, getOptimalValue(10))
    totPos = allPositivesSum()
    print(ans)
    print(totPos)
    print((ans/totPos)*100, "%")