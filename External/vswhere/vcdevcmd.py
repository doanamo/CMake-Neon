import os

swd = os.path.dirname(__file__)
vswhere = "vswhere.exe"
version = "-version [17.0,18.0) -latest"
property = "-property installationPath"
path = os.popen(f"{swd}\\{vswhere} {version} {property}").read().strip()
print(f"{path}\\VC\\Auxiliary\\Build\\vcvars64.bat")
