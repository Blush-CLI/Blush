import subprocess
import sys
import os
import platform
from build import buildBlush

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIR = os.path.join(BASE_DIR, "src")
BUILD_DIR = os.path.join(BASE_DIR, "build", "debug-x64")

system_platform = platform.system()
if system_platform == "Windows":
    BUILD_PATH = os.path.join(BUILD_DIR, "blush.exe")
else:
    BUILD_PATH = os.path.join(BUILD_DIR, "blush")

os.makedirs(BUILD_DIR, exist_ok=True)

rebuild = "--rebuild" in [arg.lower() for arg in sys.argv[1:]]
args_to_pass = [arg for arg in sys.argv[1:] if arg.lower() != "--rebuild"]

if rebuild or not os.path.exists(BUILD_PATH):
    BUILD_PATH = buildBlush(SRC_DIR)

if not os.path.exists(BUILD_PATH):
    raise FileNotFoundError(f"Executable not found: {BUILD_PATH}")

subprocess.run([BUILD_PATH] + args_to_pass, shell=(system_platform=="Windows"))
