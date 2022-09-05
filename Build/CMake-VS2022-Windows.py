import os
import sys
import glob
import shutil

os.chdir(os.path.dirname(__file__))

output = "CMake-VS2022-Windows"
if os.path.exists(output):
    shutil.rmtree(output)

os.mkdir(output)
os.chdir(output)

result = os.system("cmake -G \"Visual Studio 17\" -A x64 \"../..\"")

if result != 0:
    sys.exit(result)

os.system("start " + glob.glob("*.sln")[0])
