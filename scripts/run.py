import subprocess
import sys
import os
import platform
from build import buildBlush

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIR = os.path.join(BASE_DIR, "src")

profile = "debug"
if "--bprofile" in sys.argv:
    idx = sys.argv.index("--bprofile")
    if idx + 1 < len(sys.argv):
        profile = sys.argv[idx + 1].lower()

BUILD_PATH = os.path.join(BASE_DIR, "build", profile, f"blush-{platform.system().lower()}-{'x64' if platform.machine() in ['x86_64','AMD64'] else 'arm64'}")

rebuild = "--rebuild" in [arg.lower() for arg in sys.argv[1:]]

args_to_pass = [arg for arg in sys.argv[1:] if arg.lower() not in ["--rebuild", "--bprofile", profile]]

if rebuild or not os.path.exists(BUILD_PATH):
    BUILD_PATH = buildBlush(SRC_DIR, profile)

if not os.path.exists(BUILD_PATH):
    raise FileNotFoundError(f"Executable not found: {BUILD_PATH}")

subprocess.run([BUILD_PATH] + args_to_pass, shell=(platform.system() == "Windows"))