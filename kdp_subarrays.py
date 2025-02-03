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
        for j in range(1, rows):
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
    dp = [[[0]*1001 for _ in range(divisions)] for _ in range(3)]
    # for i in range(4, 1001):
    #     dp[1][0][i] = maxes1[0]
    dp[1][0][4] = maxes1[0]
    # for i in range(8, 1001):
    #     dp[2][0][i] = maxes1[0] + maxes2[0]
    dp[2][0][8] = maxes1[0] + maxes2[0]
    for i in range(1, divisions):
        for j in range(1, 1001):
            # dp[0][i][j] = max(dp[0][i][j], dp[0][i][j - 1])
            # dp[1][i][j] = max(dp[1][i][j], dp[1][i][j - 1])
            # dp[2][i][j] = max(dp[2][i][j], dp[2][i][j - 1])

            # dp[0]
            dp[0][i][j] = max(dp[0][i][j], dp[0][i - 1][j], dp[1][i - 1][j], dp[2][i - 1][j])

            # dp[1]
            if(j >= 6):
                dp[1][i][j] = max(dp[1][i][j], dp[0][i - 1][j - 6] + maxes1[i])
            
            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])):
                if(j >= 2):
                    dp[1][i][j] = max(dp[1][i][j], dp[1][i - 1][j - 2] + maxes1[i])
            elif((starts1[i - 1] == starts1[i]) and (ends1[i - 1] == ends1[i])):
                dp[1][i][j] = max(dp[1][i][j], dp[1][i - 1][j] + maxes1[i])
            else:
                if(j >= 4):
                    dp[1][i][j] = max(dp[1][i][j], dp[1][i - 1][j - 4] + maxes1[i])
            
            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])):
                if(j >= 2):
                    dp[1][i][j] = max(dp[1][i][j], dp[2][i - 1][j - 2] + maxes1[i])
            elif((starts1[i - 1] == starts1[i]) and (ends1[i - 1] == ends1[i])):
                dp[1][i][j] = max(dp[1][i][j], dp[2][i - 1][j] + maxes1[i])
            else:
                if(j >= 4):
                    dp[1][i][j] = max(dp[1][i][j], dp[2][i - 1][j - 4] + maxes1[i])

            # dp[2]
            if(j >= 10):
                dp[2][i][j] = max(dp[2][i][j], dp[0][i - 1][j - 10] + maxes1[i] + maxes2[i])

            if((starts1[i - 1] == starts1[i]) and (ends1[i - 1] == ends1[i])):
                if(j >= 6):
                    dp[2][i][j] = max(dp[2][i][j], dp[1][i - 1][j - 6] + maxes1[i] + maxes2[i])
            else:
                if(j >= 8):
                    dp[2][i][j] = max(dp[2][i][j], dp[1][i - 1][j - 8] + maxes1[i] + maxes2[i])

            if((starts1[i - 1] == starts1[i]) and (ends1[i - 1] == ends1[i])):
                if(j >= 6):
                    dp[2][i][j] = max(dp[2][i][j], dp[2][i - 1][j - 6] + maxes1[i] + maxes2[i])
            else:
                if(j >= 8):
                    dp[2][i][j] = max(dp[2][i][j], dp[2][i - 1][j - 8] + maxes1[i] + maxes2[i])
            

    optimum = max(dp[0][divisions - 1][1000], dp[1][divisions - 1][1000], dp[2][divisions - 1][1000])
    optimum = 0
    used = -1
    for i in range(1, 1001):
        if max(dp[0][divisions - 1][i], dp[1][divisions - 1][i], dp[2][divisions - 1][i]) >= optimum:
            optimum = max(dp[0][divisions - 1][i], dp[1][divisions - 1][i], dp[2][divisions - 1][i])
            used = i
    return optimum, dp, maxes1, maxes2, starts1, ends1, starts2, ends2, used

def getSubarrays(rows, dp, used, maxes1, maxes2, starts1, ends1):
    divisions = 10000//rows
    i = -1
    j = divisions - 1
    k = used
    selected = [-1 for _ in range(divisions)]
    while j >= 0:
        which = i
        if which == -1:
            if ((dp[0][j][k] >= dp[1][j][k]) and (dp[0][j][k] >= dp[2][j][k])):
                which = 0
            elif((dp[1][j][k] >= dp[0][j][k]) and (dp[1][j][k] >= dp[2][j][k])):
                which = 1
            else:
                which = 2

        selected[j] = which        

        if which == 0:
            if((dp[0][j - 1][k] >= dp[1][j - 1][k]) and (dp[0][j - 1][k] >= dp[2][j - 1][k])):
                i = 0
                j -= 1
            elif((dp[1][j - 1][k] >= dp[0][j - 1][k]) and (dp[1][j - 1][k] >= dp[2][j - 1][k])):
                i = 1
                j -= 1
            else:
                i = 2
                j -= 1

        elif which == 1:
            m = 0
            if k >= 6:
                m = max(m, dp[0][j - 1][k - 6] + maxes1[j])
            if((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])):
                if(k >= 2):
                    m = max(m, dp[1][j - 1][k - 2] + maxes1[j])
            elif((starts1[j - 1] == starts1[j]) and (ends1[j - 1] == ends1[j])):
                m = max(m, dp[1][j - 1][k] + maxes1[j])
            else:
                if(k >= 4):
                    m = max(m, dp[1][j - 1][k - 4] + maxes1[j])
            
            if((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])):
                if(k >= 2):
                    m = max(m, dp[2][j - 1][k - 2] + maxes1[j])
            elif((starts1[j - 1] == starts1[j]) and (ends1[j - 1] == ends1[j])):
                m = max(m, dp[2][j - 1][k] + maxes1[j])
            else:
                if(k >= 4):
                    m = max(m, dp[2][j - 1][k - 4] + maxes1[j])

            done = False
            if k >= 6:
                if m == dp[0][j - 1][k - 6] + maxes1[j]:
                    i = 0
                    j -= 1
                    k -= 6
                    done = True
            if not done:
                if((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])):
                    if(k >= 2):
                        if m == dp[1][j - 1][k - 2] + maxes1[j]:
                            i = 1
                            j -= 1
                            k -= 2
                            done = True
                elif((starts1[j - 1] == starts1[j]) and (ends1[j - 1] == ends1[j])):
                    if m == dp[1][j - 1][k] + maxes1[j]:
                        i = 1
                        j -= 1
                        k -= 2
                        done = True
                else:
                    if(k >= 4):
                        if m == dp[1][j - 1][k - 4] + maxes1[j]:
                            i = 1
                            j -= 1
                            k -= 4
                            done = True
            
            if not done:
                if((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])):
                    if(k >= 2):
                        if m == dp[2][j - 1][k - 2] + maxes1[j]:
                            i = 2
                            j -= 1
                            k -= 2
                            done = True
                elif((starts1[j - 1] == starts1[j]) and (ends1[j - 1] == ends1[j])):
                    if m == dp[2][j - 1][k] + maxes1[j]:
                        i = 2
                        j -= 1
                        done = True
                else:
                    if(k >= 4):
                        if m == dp[2][j - 1][k - 4] + maxes1[j]:
                            i = 2
                            j -= 1
                            k -= 4
                            done = True

        else:
            m = 0
            if(k >= 10):
                m = max(m, dp[0][j - 1][k - 10] + maxes1[j] + maxes2[j])

            if((starts1[j - 1] == starts1[j]) and (ends1[j - 1] == ends1[j])):
                if(k >= 6):
                    m = max(m, dp[1][j - 1][k - 6] + maxes1[j] + maxes2[j])
            else:
                if(k >= 8):
                    m = max(m, dp[1][j - 1][k - 8] + maxes1[j] + maxes2[j])

            if((starts1[j - 1] == starts1[j]) and (ends1[j - 1] == ends1[j])):
                if(k >= 6):
                    m = max(m, dp[2][j - 1][k - 6] + maxes1[j] + maxes2[j])
            else:
                if(k >= 8):
                    m = max(m, dp[2][j - 1][k - 8] + maxes1[j] + maxes2[j])

            done = False
            if(k >= 10):
                if m == dp[0][j - 1][k - 10] + maxes1[j] + maxes2[j]:
                    i = 0
                    j -= 1
                    k -= 10
                    done = True

            if not done:
                if((starts1[j - 1] == starts1[j]) and (ends1[j - 1] == ends1[j])):
                    if(k >= 6):
                        if m == dp[1][j - 1][k - 6] + maxes1[j] + maxes2[j]:
                            i = 1
                            j -= 1
                            k -= 6
                            done = True
                else:
                    if(k >= 8):
                        if m == dp[1][j - 1][k - 8] + maxes1[j] + maxes2[j]:
                            i = 1
                            j -= 1
                            k -= 8
                            done = True
            
            if not done:
                if((starts1[j - 1] == starts1[j]) and (ends1[j - 1] == ends1[j])):
                    if(k >= 6):
                        if m == dp[2][j - 1][k - 6] + maxes1[j] + maxes2[j]:
                            i = 2
                            j -= 1
                            k -= 6
                            done = True
                else:
                    if(k >= 8):
                        if m == dp[2][j - 1][k - 8] + maxes1[j] + maxes2[j]:
                            i = 2
                            j -= 1
                            k -= 8
                            done = True
    return selected

if __name__ == "__main__":
    filename = 'input05.txt'
    getGrid(filename)
    optimum, dp, maxes1, maxes2, starts1, ends1, starts2, ends2, used = getOptimalValue(40)
    totPos = allPositivesSum()
    selected = getSubarrays(40, dp, used, maxes1, maxes2, starts1, ends1)
    # for i in range(len(selected)):
    with open('subarrays.txt', 'w') as file:
        for i in range(250):
            file.write(f'{starts1[i]}, {ends1[i]}; {starts2[i]}, {ends2[i]} : {selected[i]}\n')
    ans = optimum
    verify = 0
    for i in range(250):
        if selected[i] == 1:
            verify += maxes1[i]
        elif selected[i] == 2:
            verify += maxes1[i] + maxes2[i]
    print(ans)
    print(verify)
    print(totPos)
    print((ans/totPos)*100, "%")