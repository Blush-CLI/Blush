import subprocess
import sys
import os
from build_windows import buildBlushWindows

src_dir = "c:/Blush/src"
build_path = "../build/debug-x64/blush.exe"
build_dir = "../build/debug-x64"

if not os.path.exists(build_dir):
    os.makedirs(build_dir)

rebuild = "--rebuild" in [arg.lower() for arg in sys.argv[1:]]
args_to_pass = [arg for arg in sys.argv[1:] if arg.lower() != "--rebuild"]

if rebuild or not os.path.exists(build_path):
    buildBlushWindows(src_dir)

subprocess.run([os.path.abspath(build_path)] + args_to_pass)
