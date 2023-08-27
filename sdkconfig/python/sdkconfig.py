import os

menuconfig_cmd = 'menuconfig ../Kconfig'
auto_header_cmd = 'python ../kconfig.py ../Kconfig .config ../../src/include/config/autoconfig.h log.txt .config'

os.system(menuconfig_cmd)
os.system(auto_header_cmd)

