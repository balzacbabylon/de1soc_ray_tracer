import math
import sys

def find_max_radius_vertex(obj_filename):
    max_dist_sq = -1.0
    best_index = -1
    best_vertex = (0.0, 0.0, 0.0)
    
    vertex_count = 0

    try:
        with open(obj_filename, 'r') as f:
            for line in f:
                parts = line.strip().split()
                
                if not parts:
                    continue
                
                # Check for vertex definition ('v')
                if parts[0] == 'v':
                    vertex_count += 1
                    
                    try:
                        x = float(parts[1])
                        y = float(parts[2])
                        z = float(parts[3])
                        
                        dist_sq = (x * x) + (y * y) + (z * z)
                        
                        if dist_sq > max_dist_sq:
                            max_dist_sq = dist_sq
                            best_index = vertex_count # 1-based index
                            best_vertex = (x, y, z)
                            
                    except (ValueError, IndexError):
                        continue

        if best_index != -1:
            radius = math.sqrt(max_dist_sq)
            print(f"--- Results for {obj_filename} ---")
            print(f"Total Vertices Scanned: {vertex_count}")
            print(f"Max Radius Vertex Index: {best_index}")
            print(f"Coordinates: {best_vertex}")
            print(f"Distance from Origin (Radius): {radius}")
        else:
            print("No vertices found in the file.")

    except FileNotFoundError:
        print(f"Error: File '{obj_filename}' not found.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
    # Check if filename argument is provided
    if len(sys.argv) < 2:
        print("Usage: python script.py <filename.obj>")
    else:
        find_max_radius_vertex(sys.argv[1])