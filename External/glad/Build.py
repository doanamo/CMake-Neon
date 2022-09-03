import os
import sys
import stat
import shutil
import glob

# Helpers
vcdevcmd = os.popen("python ../vswhere/vcdevcmd.py").read().strip()
def run_vcdevcmd(cmds):
    shell = f"cmd.exe /c \"\"{vcdevcmd}\" 1>nul"
    for cmd in cmds:
        shell += " && " + cmd
    shell += "\""
    return os.system(shell)

def remove_dir(path):
    if not os.path.exists(path):
        return

    def make_writable(action, name, exc):
        os.chmod(name, stat.S_IWRITE)
        os.remove(name)

    shutil.rmtree(path, onerror=make_writable)

# Build project
remove_dir("Build")
result = run_vcdevcmd([
    "cmake . -B Build/Debug -G Ninja -D CMAKE_BUILD_TYPE=Debug",
    "cmake --build Build/Debug",
    "cmake . -B Build/Release -G Ninja -D CMAKE_BUILD_TYPE=Release",
    "cmake --build Build/Release",
])

# Save libraries
remove_dir("Library/Windows")
os.makedirs("Library/Windows/Debug")
os.makedirs("Library/Windows/Release")

shutil.copy(glob.glob("Build/Debug/**/glad.lib", recursive=True)[0], "Library/Windows/Debug/glad.lib")
shutil.copy(glob.glob("Build/Debug/**/glad.pdb", recursive=True)[0], "Library/Windows/Debug/glad.pdb")
shutil.copy(glob.glob("Build/Release/**/glad.lib", recursive=True)[0], "Library/Windows/Release/glad.lib")
