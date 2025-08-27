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

    assets_dir = os.path.join(src, "..", "assets")
    os.makedirs(assets_dir, exist_ok=True)
    ico_file = os.path.join(assets_dir, "blush.ico")
    rc_file = os.path.join(assets_dir, "icon.rc")
    res_file = os.path.join(assets_dir, "icon.o")

    if not os.path.exists(ico_file):
        raise FileNotFoundError(f"Icon not found: {ico_file}")

    ico_file_fixed = os.path.abspath(ico_file).replace("\\", "/")
    with open(rc_file, "w") as f:
        f.write(f'IDI_ICON1 ICON "{ico_file_fixed}"')

    subprocess.run(["windres", rc_file, "-O", "coff", "-o", res_file], check=True)

    subprocess.run(["g++", "-std=c++23"] + cpp_files + [res_file, "-o", build_path], check=True)

    os.remove(rc_file)
    print(f"Build finished: {build_path}")
