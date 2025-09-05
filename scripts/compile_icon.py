import os
import subprocess
import platform

def compile_icon():
    if platform.system() != "Windows":
        return None

    ICO_DIR = os.path.join(os.path.dirname(__file__), "..", "assets")
    ICO_FILE = os.path.abspath(os.path.join(ICO_DIR, "blush.ico")).replace("\\", "/")
    RC_PATH = os.path.abspath(os.path.join(ICO_DIR, "icon.rc")).replace("\\", "/")
    RES_PATH = os.path.abspath(os.path.join(ICO_DIR, "icon.o")).replace("\\", "/")

    os.makedirs(ICO_DIR, exist_ok=True)
    if not os.path.exists(ICO_FILE):
        raise FileNotFoundError(f"Icon not found: {ICO_FILE}")

    with open(RC_PATH, "w") as f:
        f.write(f'IDI_ICON1 ICON "{ICO_FILE}"')

    subprocess.run(['windres', RC_PATH, '-O', 'coff', '-o', RES_PATH], check=True)
    os.remove(RC_PATH)

    return RES_PATH
