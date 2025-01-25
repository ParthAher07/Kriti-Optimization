def getGrid(filename):
    grid = [[0] * 10000 for _ in range(10000)]
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

if __name__ == "__main__":
    filename = 'input.txt'
    grid = getGrid(filename)
    print(grid)