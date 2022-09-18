import os
import sys
import stat
import shutil
import glob

# Helpers
def remove_dir(path):
    if not os.path.exists(path):
        return

    def make_writable(action, name, exc):
        os.chmod(name, stat.S_IWRITE)
        os.remove(name)

    shutil.rmtree(path, onerror=make_writable)

# Clone repository
remove_dir("Repository")
result = os.system(
    "git clone -c advice.detachedHead=false --depth 1 " +
    "--branch v1.2.12 https://github.com/madler/zlib.git Repository"
)

if result != 0:
    sys.exit(result)

# Build libraries
remove_dir("Build")
result = os.system(
    "cmake . -B Build -Wno-dev && " +
    "cmake --build Build --config Debug --target zlibstatic && " +
    "cmake --build Build --config Release --target zlibstatic"
)

if result != 0:
    sys.exit(result)

# Install libraries
remove_dir("Library/Windows")
os.makedirs("Library/Windows/Debug")
os.makedirs("Library/Windows/Release")
shutil.copy(glob.glob("Build/**/Debug/zlibstaticd.lib", recursive=True)[0], "Library/Windows/Debug/zlib.lib")
shutil.copy(glob.glob("Build/**/Debug/zlibstaticd.pdb", recursive=True)[0], "Library/Windows/Debug/zlib.pdb")
shutil.copy(glob.glob("Build/**/Release/zlibstatic.lib", recursive=True)[0], "Library/Windows/Release/zlib.lib")
shutil.copy(glob.glob("Build/**/Release/zlibstatic.pdb", recursive=True)[0], "Library/Windows/Release/zlib.pdb")

# Install includes
remove_dir("Include")
os.makedirs("Include")
shutil.copy(glob.glob("Repository/zlib.h", recursive=True)[0], "Include/zlib.h")
shutil.copy(glob.glob("Build/**/zconf.h", recursive=True)[0], "Include/zconf.h")
