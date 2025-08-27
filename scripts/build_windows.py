import subprocess
import os

def buildBlushWindows(src, args=None):
    cpp_files = []
    for root, dirs, files in os.walk(src):
        for file in files:
            if file.endswith(".cpp"):
                cpp_files.append(os.path.join(root, file))

    build_path = os.path.join(src, "..", "build", "debug-x64", "blush")
    subprocess.run(
        ["g++", "-std=c++17"] + cpp_files + ["-o", build_path],
        cwd=src,
        check=True
    )
