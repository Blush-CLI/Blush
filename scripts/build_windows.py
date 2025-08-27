import subprocess
import os

def buildBlushWindows(src, args=None):
    cpp_files = []
    for root, dirs, files in os.walk(src):
        for file in files:
            if file.endswith(".cpp"):
                cpp_files.append(os.path.join(root, file))

    build_dir = os.path.join(src, "..", "build", "debug-x64")
    os.makedirs(build_dir, exist_ok=True)
    build_path = os.path.join(build_dir, "blush.exe")
    res_path = os.path.join(src, "..", "assets", "icon.res").replace("\\","/")

    subprocess.run(
        ["g++", "-std=c++17"] + cpp_files + ["-o", build_path, res_path],
        check=True
    )
