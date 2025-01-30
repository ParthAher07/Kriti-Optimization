import matplotlib.pyplot as plt
import numpy as np

def is_point_inside_polygon(polygon, point):
    """Check if a point (x, y) is inside or on the edge of a polygon using Ray-Casting Algorithm."""
    x, y = point
    n = len(polygon)
    inside = False

    p1x, p1y = polygon[0]
    for i in range(n + 1):
        p2x, p2y = polygon[i % n]

        # Check if the point is exactly on a polygon edge
        if (p1x <= x <= p2x or p2x <= x <= p1x) and (p1y <= y <= p2y or p2y <= y <= p1y):
            if (p2x - p1x) * (y - p1y) == (p2y - p1y) * (x - p1x):  # Collinear check
                return True  # The point is on the edge

        # Ray-casting logic
        if y > min(p1y, p2y):
            if y <= max(p1y, p2y):
                if x <= max(p1x, p2x):
                    if p1y != p2y:
                        xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                        if p1x == p2x or x <= xinters:
                            inside = not inside

        p1x, p1y = p2x, p2y

    return inside


def get_value(polygon, positive, value):

    if len(positive) != len(value):
        print("Invalid input: Lists must have the same length.")
        return 0 

    ans = 0
    for i in range(len(positive)): 
        x, y = positive[i]
        if is_point_inside_polygon(polygon, (x, y)):
            ans += value[i] 

    return ans


def read_points_from_file(filename):
    """Reads 2D points from a file where each line contains two floating-point numbers (x, y)."""
    points = set()
    with open(filename, 'r') as file:
        for line in file:
            x, y = map(float, line.strip().split())
            points.add((x, y))
    return points

def read_positive_from_file(filename):
    """Reads 2D points from a file where each line contains two floating-point numbers (x, y)."""
    pos = []
    val = []
    with open(filename, 'r') as file:
        for line in file:
            x, y, v = map(float, line.strip().split())
            pos.append((x, y))
            val.append(v)

    return pos, val

def find_starting_point(points):
    """Find the point with the minimum Euclidean distance (smallest x, then y)."""
    return min(points, key=lambda p: (p[0]**2 + p[1]**2, p[0], p[1]))

def traverse_polygon(points):
    """Traverse the points in alternating x and y directions to check for a closed polygon."""
    visited = []
    start = find_starting_point(points)
    current = start
    direction = 'x'  # Start moving along the x-direction
    
    while True:
        if current != start:
            visited.append(current)

        print("current =", current)

        if direction == 'x':
            candidates = sorted([p for p in points if p[1] == current[1] and p not in visited and p != current], 
                                key=lambda p: abs(p[0] - current[0]))
        else:
            candidates = sorted([p for p in points if p[0] == current[0] and p not in visited and p != current], 
                                key=lambda p: abs(p[1] - current[1]))

        if not candidates:
            break

        current = candidates[0]  # Move to the next closest point in the chosen direction
        direction = 'y' if direction == 'x' else 'x'  # Alternate direction
    
    visited.append(current)  # Add last point to the path
    is_closed = len(visited) == len(points) and current == start  # Check if polygon is closed
    return visited, is_closed  # Return visited path and closure status

def plot_polygon(points, path, is_closed,positive):
    """Plot the points and the polygon formed."""
    x_vals, y_vals = zip(*points)

    plt.figure(figsize=(6, 6))
    plt.scatter(x_vals, y_vals, color='blue', label="Points")
    
    # Mark the start and end points
    # plt.scatter(*path[0], color='green', label="Start", s=100, marker='o')
    # plt.scatter(*path[-1], color='red', label="End", s=100, marker='x')

    # Draw the polygon path
    if is_closed:
        path.append(path[0])  # Close the polygon
    plt.plot(*zip(*path), linestyle='-', color='purple', linewidth=2, label="Polygon Path") 

    point_inside,point_outside = [],[]

    for i in range(len(positive)): 
        x, y = positive[i]
        if is_point_inside_polygon(path, (x, y)):
            point_inside.append((x,y))
        else:
            point_outside.append((x,y))

    x_vals, y_vals = zip(*point_inside)
    plt.scatter(x_vals, y_vals, color='green', label="Points Inside")

    x_vals, y_vals = zip(*point_outside)
    plt.scatter(x_vals, y_vals, color='red', label="Points Outside")
    

    plt.xlabel("X-axis")
    plt.ylabel("Y-axis")
    plt.title("Rectilinear Polygon Visualization")
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    filename = "points.txt"  # Change this to your file
    points = read_points_from_file(filename)
    positive, values = read_positive_from_file("positive.txt")
    
    path, is_closed = traverse_polygon(points)

    if is_closed:
        print("The points can form a rectilinear polygon.")

        print("The value of the polygon is: ", get_value(path, positive,values))
    else:
        print("The points cannot form a rectilinear polygon.")

    plot_polygon(points, path, is_closed, positive)
