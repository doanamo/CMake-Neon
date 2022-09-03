import os
import sys
import stat
import shutil

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
    "--branch 3.4.0 https://gitlab.com/libeigen/eigen.git Repository"
])

if result != 0:
    sys.exit(result)

# Install library
remove_dir("Include")
shutil.copytree("Repository/Eigen", "Include/Eigen")
