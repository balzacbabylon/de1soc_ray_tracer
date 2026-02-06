import sys
import math

# --- Vector Helper Functions ---
def normalize(v):
    mag = math.sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2])
    if mag == 0: return (0.0, 0.0, 0.0)
    return (v[0]/mag, v[1]/mag, v[2]/mag)

def cross_product(a, b):
    return (
        a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]
    )

def add_vectors(v1, v2):
    return (v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2])

def parse_and_process(filename):
    raw_vertices = []
    faces = [] 
    
    # 1. Parse the File
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if not parts: continue

            if parts[0] == 'v':
                raw_vertices.append((float(parts[1]), float(parts[2]), float(parts[3])))
            
            elif parts[0] == 'f':
                face_idxs = []
                for p in parts[1:4]: 
                    idx_str = p.split('/')[0]
                    face_idxs.append(int(idx_str) - 1) 
                faces.append(face_idxs)

    # --- NEW: Find Max Radius Vertex ---
    max_dist_sq = -1.0
    max_idx = -1
    max_v = (0,0,0)

    for i, v in enumerate(raw_vertices):
        dist_sq = v[0]*v[0] + v[1]*v[1] + v[2]*v[2]
        if dist_sq > max_dist_sq:
            max_dist_sq = dist_sq
            max_idx = i
            max_v = v

    print(f"\n--- Max Radius Analysis ---")
    if max_idx != -1:
        # Convert to fixed point
        fx = int(max_v[0] * 65536)
        fy = int(max_v[1] * 65536)
        fz = int(max_v[2] * 65536)
        
        real_radius = math.sqrt(max_dist_sq)
        fixed_radius = int(real_radius * 65536)

        print(f"Vertex Index: {max_idx}")
        print(f"Float Coords: {max_v}")
        print(f"Fixed Point Pair: {{ {fx}, {fy}, {fz} }}")
        print(f"Real Radius: {real_radius}")
        print(f"Fixed Radius: {fixed_radius}")
    print("---------------------------\n")


    # 2. Initialize Accumulators for Vertex Normals
    # List of [0,0,0] with same length as vertices
    acc_normals = [(0.0, 0.0, 0.0)] * len(raw_vertices)

    # 3. Pass 1: Accumulate Face Normals
    for face in faces:
        v0 = raw_vertices[face[0]]
        v1 = raw_vertices[face[1]]
        v2 = raw_vertices[face[2]]

        # Calculate Edge Vectors
        edge1 = (v1[0]-v0[0], v1[1]-v0[1], v1[2]-v0[2])
        edge2 = (v2[0]-v0[0], v2[1]-v0[1], v2[2]-v0[2])

        # Calculate Face Normal
        fn = cross_product(edge1, edge2)

        # Add this normal to all 3 vertices of the face
        acc_normals[face[0]] = add_vectors(acc_normals[face[0]], fn)
        acc_normals[face[1]] = add_vectors(acc_normals[face[1]], fn)
        acc_normals[face[2]] = add_vectors(acc_normals[face[2]], fn)

    # 4. Pass 2: Normalize to get Final Vertex Normals
    final_vertex_normals = []
    for n in acc_normals:
        final_vertex_normals.append(normalize(n))

    return raw_vertices, faces, final_vertex_normals

def generate_c(vertices, faces, normals, name):
    output_filename = "out.h"
    
    with open(output_filename, "w") as f:
        f.write(f"#ifndef {name.upper()}_H\n")
        f.write(f"#define {name.upper()}_H\n")
        f.write('#include "common.h"\n') 

        # 1. Combined Vertex Array (Position + Normal)
        f.write(f"\n#define {name.upper()}_NUM_VERTS {len(vertices)}\n")
        f.write(f"const Vertex {name}_vertices[] = {{\n")
        
        for i in range(len(vertices)):
            v = vertices[i]
            n = normals[i]
            
            # Fixed point conversion (1.0 = 65536)
            vx, vy, vz = int(v[0]*65536), int(v[1]*65536), int(v[2]*65536)
            nx, ny, nz = int(n[0]*65536), int(n[1]*65536), int(n[2]*65536)
            
            f.write(f"    {{ {vx}, {vy}, {vz}, {nx}, {ny}, {nz} }},\n")
        
        f.write("};\n")

        # 2. Indices (The Connectivity) - UPDATED to Triangle Struct format
        f.write(f"\n#define {name.upper()}_NUM_TRIANGLES {len(faces)}\n")
        f.write(f"const Triangle {name}_indices[] = {{\n")
        for face in faces:
            # Writes: { 0, 1, 2 },
            f.write(f"    {{ {face[0]}, {face[1]}, {face[2]} }},\n")
        f.write("};\n")

        # 3. Color Array
        f.write(f"\nconst short int {name}_colorarray[] = {{\n")
        for i in range(len(faces)):
            f.write("    GRAY,\n")
        f.write("};\n")

        f.write(f"#endif\n")
    
    print(f"Successfully wrote {len(vertices)} vertices and {len(faces)} triangles to {output_filename}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python obj_to_c.py <file.obj>")
        sys.exit(1)

    vertices, faces, vertex_normals = parse_and_process(sys.argv[1])
    name = sys.argv[1].split('.')[0]
    generate_c(vertices, faces, vertex_normals, name)