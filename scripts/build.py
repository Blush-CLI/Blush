import subprocess
import os
import platform
import sys

def buildBlush(src, profile="debug"):
    cpp_files = []
    for root, dirs, files in os.walk(src):
        if "installer" in root.split(os.sep):
            continue
        for file in files:
            if file.endswith(".cpp"):
                cpp_files.append(os.path.join(root, file))

    system_platform = platform.system()
    arch = "x64" if platform.machine() in ["x86_64", "AMD64"] else "arm64"
    plat_name = "windows" if system_platform == "Windows" else "linux"

    out_dir = os.path.join(src, "..", "build", profile)
    os.makedirs(out_dir, exist_ok=True)
    build_path = os.path.join(out_dir, f"blush-{plat_name}-{arch}")

    res_file = None
    if profile == "release" and system_platform == "Windows":
        from compile_icon import compile_icon
        res_file = compile_icon()

    compile_cmd = ["g++", "-std=c++23"] + cpp_files + ["-o", build_path]

    if profile == "release":
        compile_cmd += ["-O2", "-static", "-static-libgcc", "-static-libstdc++", "-lcurl", "-lssl", "-lcrypto", "-lz", "-ldl", "-pthread"]
        if res_file:
            compile_cmd.append(res_file)
    else:
        compile_cmd += ["-O0", "-lcurl", "-lssl", "-lcrypto", "-lz", "-ldl", "-pthread"]

    subprocess.run(compile_cmd, check=True)
    if res_file:
        os.remove(res_file)

    return build_path

if __name__ == "__main__":
    profile = "debug"
    if "--bprofile" in sys.argv:
        idx = sys.argv.index("--bprofile")
        if idx + 1 < len(sys.argv):
            profile = sys.argv[idx + 1]
    SRC_DIR = os.path.join(os.path.dirname(__file__), "..", "src")
    path = buildBlush(SRC_DIR, profile)
    print(f"Built at {path}")
