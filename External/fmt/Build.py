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

# Clone repository
remove_dir("Repository")
result = run_vcdevcmd([
    "git clone -c advice.detachedHead=false --depth 1 " +
    "--branch 9.1.0 https://github.com/fmtlib/fmt.git Repository"
])

if result != 0:
    sys.exit(result)

# Build library
cmake_cache_opts = (
    " -G Ninja"
    " -D FMT_MASTER_PROJECT=OFF"
    " -D FMT_INSTALL=ON"
)

remove_dir("Build")
result = run_vcdevcmd([  
  "cmake . -B Build/Debug -D CMAKE_BUILD_TYPE=Debug" + cmake_cache_opts,
  "cmake --build Build/Debug",
  "cmake . -B Build/Release -D CMAKE_BUILD_TYPE=Release" + cmake_cache_opts,
  "cmake --build Build/Release",
])

if result != 0:
    sys.exit(result)

# Install library
remove_dir("Library/Windows")
remove_dir("Include")

os.makedirs("Library/Windows/Debug")
os.makedirs("Library/Windows/Release")

shutil.copy(glob.glob("Build/Debug/**/fmtd.lib", recursive=True)[0], "Library/Windows/Debug/fmt.lib")
shutil.copy(glob.glob("Build/Debug/**/fmt.pdb", recursive=True)[0], "Library/Windows/Debug/fmt.pdb")
shutil.copy(glob.glob("Build/Release/**/fmt.lib", recursive=True)[0], "Library/Windows/Release/fmt.lib")
shutil.copy(glob.glob("Build/Release/**/fmt.pdb", recursive=True)[0], "Library/Windows/Release/fmt.pdb")
shutil.copytree("Repository/include", "Include")
