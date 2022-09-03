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
    "--branch 3.3.8 https://github.com/glfw/glfw.git Repository"
])

if result != 0:
    sys.exit(result)

# Build configurations
cmake_cache_opts = (
    "-G Ninja "
    "-D GLFW_BUILD_EXAMPLES=OFF "
    "-D GLFW_BUILD_TESTS=OFF "
    "-D GLFW_BUILD_DOCS=OFF "
    "-D GLFW_INSTALL=OFF"
)

remove_dir("Build")
result = run_vcdevcmd([
    "cmake Repository -B Build/Debug -D CMAKE_BUILD_TYPE=Debug " + cmake_cache_opts,
    "cmake --build Build/Debug",
    "cmake Repository -B Build/Release -D CMAKE_BUILD_TYPE=Release " + cmake_cache_opts,
    "cmake --build Build/Release",
])

if result != 0:
    sys.exit(result)

# Install configurations
remove_dir("Library/Windows")
remove_dir("Include")

os.makedirs("Library/Windows/Debug")
os.makedirs("Library/Windows/Release")

shutil.copy(glob.glob("Build/Debug/**/glfw3.lib", recursive=True)[0], "Library/Windows/Debug/glfw3.lib")
shutil.copy(glob.glob("Build/Debug/**/glfw.pdb", recursive=True)[0], "Library/Windows/Debug/glfw3.pdb")
shutil.copy(glob.glob("Build/Release/**/glfw3.lib", recursive=True)[0], "Library/Windows/Release/glfw3.lib")
shutil.copytree("Repository/include", "Include")
