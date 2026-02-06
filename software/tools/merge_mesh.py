import trimesh
import os
import argparse
import glob

def merge_obj_files(input_folder, output_path):
    # 1. Verify input directory exists
    if not os.path.isdir(input_folder):
        print(f"Error: The directory '{input_folder}' does not exist.")
        return

    # 2. Find all .obj files in the folder
    # We use os.path.join to ensure it works on both Windows and Mac/Linux
    search_pattern = os.path.join(input_folder, "*.obj")
    files = glob.glob(search_pattern)

    if not files:
        print(f"No .obj files found in '{input_folder}'.")
        return

    print(f"Found {len(files)} OBJ files. Loading...")

    meshes_to_merge = []

    # 3. Load meshes
    for file_path in files:
        try:
            # trimesh.load can return a Scene or a Trimesh
            loaded = trimesh.load(file_path, force='mesh') 
            meshes_to_merge.append(loaded)
            print(f" - Loaded: {os.path.basename(file_path)}")
        except Exception as e:
            print(f" ! Failed to load {os.path.basename(file_path)}: {e}")

    # 4. Concatenate and Export
    if meshes_to_merge:
        print("Merging meshes...")
        merged_mesh = trimesh.util.concatenate(meshes_to_merge)
        
        # Ensure the output directory exists
        output_dir = os.path.dirname(output_path)
        if output_dir and not os.path.exists(output_dir):
            os.makedirs(output_dir)

        merged_mesh.export(output_path)
        print(f"Success! Merged file saved to: {output_path}")
    else:
        print("No valid meshes were loaded. Nothing to merge.")

if __name__ == "__main__":
    # Set up command line arguments
    parser = argparse.ArgumentParser(description="Merge all OBJ files in a specific folder.")
    
    parser.add_argument(
        "--input", "-i", 
        required=True, 
        help="Path to the folder containing OBJ files"
    )
    parser.add_argument(
        "--output", "-o", 
        required=True, 
        help="Full path and filename for the output (e.g., C:/output/merged.obj)"
    )

    args = parser.parse_args()

    merge_obj_files(args.input, args.output)