import subprocess
import os

os.system(
    'cd "c:\\Blush\\src\\" && g++ blush.cpp -o ..\\build\\debug-x64\\blush'
)

def buildBlushWindows(src, args=None):
    subprocess.run(
        ["g++", "blush.cpp", "-o", "..\\build\\debug-x64\\blush"],
        cwd=src,
        check=True
    )
