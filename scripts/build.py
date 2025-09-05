import subprocess
import os
import sys
import platform

def buildBlush(src, args=None):
    cpp_files = []
    for root, dirs, files in os.walk(src):
        for file in files:
            if file.endswith(".cpp"):
                cpp_files.append(os.path.join(root, file))

    build_dir = os.path.join(src, "..", "build", "debug-x64")
    os.makedirs(build_dir, exist_ok=True)

    system_platform = platform.system()
    if system_platform == "Windows":
        build_path = os.path.join(build_dir, "blush.exe")
    else:
        build_path = os.path.join(build_dir, "blush")

    # Windows: compile icon
    if system_platform == "Windows":
        from compile_icon import compile_icon
        res_file = compile_icon()
    else:
        res_file = None  #  linx doesnt use icon res

    compile_cmd = ["g++", "-std=c++23"]
    if system_platform == "Windows":
        compile_cmd += ["-static-libgcc", "-static-libstdc++", "-static"]

    compile_cmd += cpp_files
    if res_file:
        compile_cmd.append(res_file)
    compile_cmd += ["-o", build_path]

    print(f"Compiling for {system_platform}...")
    subprocess.run(compile_cmd, check=True)
    if res_file:
        os.remove(res_file)

    print(f"Build finished: {build_path}")
    return build_path
