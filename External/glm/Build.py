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
    "https://github.com/g-truc/glm.git Repository " +
    "&& cd Repository " +
    "&& git fetch origin cc98465e3508535ba8c7f6208df934c156a018dc " +
    "&& git checkout FETCH_HEAD"
])

if result != 0:
    sys.exit(result)

# Install library
remove_dir("Include")
shutil.copytree("Repository/glm", "Include/glm")
