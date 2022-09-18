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
    "git clone -c advice.detachedHead=false --depth 1"
    + " --branch v1.6.38 https://github.com/glennrp/libpng.git Repository"
)

if result != 0:
    sys.exit(result)

# Build libraries
cmake_cache_opts = (
    "-D PNG_SHARED=OFF "
    "-D PNG_STATIC=ON "
    "-D PNG_EXECUTABLES=OFF "
    "-D PNG_TESTS=OFF "
    "-D PNG_BUILD_ZLIB=ON "
)

remove_dir("Build")
result = os.system(
    "cmake . -B Build -Wno-dev " + cmake_cache_opts
    + " && cmake --build Build --config Debug --target png_static"
    + " && cmake --build Build --config Release --target png_static"
)

if result != 0:
    sys.exit(result)

# Install libraries
remove_dir("Library/Windows")
os.makedirs("Library/Windows/Debug")
os.makedirs("Library/Windows/Release")
shutil.copy(glob.glob("Build/**/Debug/libpng16_staticd.lib", recursive=True)[0], "Library/Windows/Debug/libpng.lib")
shutil.copy(glob.glob("Build/**/Debug/libpng16_staticd.pdb", recursive=True)[0], "Library/Windows/Debug/libpng.pdb")
shutil.copy(glob.glob("Build/**/Release/libpng16_static.lib", recursive=True)[0], "Library/Windows/Release/libpng.lib")
shutil.copy(glob.glob("Build/**/Release/libpng16_static.pdb", recursive=True)[0], "Library/Windows/Release/libpng.pdb")

# Install includes
remove_dir("Include")
os.makedirs("Include")
shutil.copy(glob.glob("Repository/png.h", recursive=True)[0], "Include/png.h")
shutil.copy(glob.glob("Repository/pngconf.h", recursive=True)[0], "Include/pngconf.h")
shutil.copy(glob.glob("Build/**/pnglibconf.h", recursive=True)[0], "Include/pnglibconf.h")
