import matplotlib.pyplot as plt # type: ignore

# Function to read the file and extract coordinates
def read_lines_from_file(filename):
    lines = []
    with open(filename, 'r') as file:
        for line in file:
            try:
                x1, y1, x2, y2 = map(float, line.split())  # Convert to float
                lines.append(((x1, y1), (x2, y2)))  # Store as tuples
            except ValueError:
                print(f"Skipping invalid line: {line.strip()}")
    return lines

# Function to plot the lines
def plot_lines(lines):
    plt.figure(figsize=(8, 8))
    for (x1, y1), (x2, y2) in lines:
        plt.plot([x1, x2], [y1, y2], linestyle='-', color='b')  # Draw lines

    plt.xlabel("X-axis")
    plt.ylabel("Y-axis")
    plt.title("Line Segments from File")
    plt.grid(True)
    plt.show()

# Read file and plot
filename = "edges.txt"  # Change this to your actual file name
lines = read_lines_from_file(filename)
plot_lines(lines)


