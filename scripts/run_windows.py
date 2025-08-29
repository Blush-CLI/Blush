import subprocess
import sys
import os
from build_windows import buildBlushWindows

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # C:\Blush
SRC_DIR = os.path.join(BASE_DIR, "src")
BUILD_DIR = os.path.join(BASE_DIR, "build", "debug-x64")
BUILD_PATH = os.path.join(BUILD_DIR, "blush.exe")

os.makedirs(BUILD_DIR, exist_ok=True)

rebuild = "--rebuild" in [arg.lower() for arg in sys.argv[1:]]
args_to_pass = [arg for arg in sys.argv[1:] if arg.lower() != "--rebuild"]

if rebuild or not os.path.exists(BUILD_PATH):
    buildBlushWindows(SRC_DIR)

if not os.path.exists(BUILD_PATH):
    raise FileNotFoundError(f"Exe not found: {BUILD_PATH}")

subprocess.run([BUILD_PATH] + args_to_pass, shell=True)
