import matplotlib.pyplot as plt
import numpy as np

def read_edges(filename):
    edges = []
    with open(filename, 'r') as file:
        for line in file:
            x1, y1, x2, y2 = map(float, line.split())
            edges.append(((x1, y1), (x2, y2)))
    return edges

def extract_polygon(edges):
    points = set()
    for edge in edges:
        points.add(edge[0])
        points.add(edge[1])
    ordered_points = sorted(points, key=lambda p: np.arctan2(p[1], p[0]))  # Sorting based on angle
    return ordered_points

def draw_polygon(filename):
    edges = read_edges(filename)
    polygon = extract_polygon(edges)
    
    x, y = zip(*polygon)  # Extract x and y coordinates
    x, y = list(x) + [x[0]], list(y) + [y[0]]  # Close the polygon
    
    plt.figure(figsize=(6, 6))
    plt.fill(x, y, color='lightblue', alpha=0.6, edgecolor='black', linewidth=2)
    plt.plot(x, y, 'ko-')  # Black outline with markers
    
    plt.xlabel('X-axis')
    plt.ylabel('Y-axis')
    plt.title('Filled Polygon')
    plt.grid(True)
    plt.show()

# Example usage
filename = "edges.txt"  # Replace with your actual file name
draw_polygon(filename)
