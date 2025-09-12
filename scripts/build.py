import subprocess
import os
import platform

def buildBlush(src, args=None):
    cpp_files = []
    for root, dirs, files in os.walk(src):
        if any(part in ["installer"] for part in root.split(os.sep)):
            continue
        for file in files:
            if file.endswith(".cpp"):
                cpp_files.append(os.path.join(root, file))

    build_dir = os.path.join(src, "..", "build", "debug-x64")
    os.makedirs(build_dir, exist_ok=True)

    system_platform = platform.system()
    build_path = os.path.join(build_dir, "blush.exe" if system_platform == "Windows" else "blush")

    res_file = None
    if system_platform == "Windows":
        from compile_icon import compile_icon
        res_file = compile_icon()

    compile_cmd = ["g++", "-std=c++23", "-Wall", "-static-libgcc", "-static-libstdc++"]
    if system_platform == "Windows":
        compile_cmd += ["-static"]

    compile_cmd += cpp_files
    if res_file:
        compile_cmd.append(res_file)

    if system_platform != "Windows":
        compile_cmd += ["-lcurl"]

    compile_cmd += ["-o", build_path]

    subprocess.run(compile_cmd, check=True)

    if res_file:
        os.remove(res_file)

    return build_path
