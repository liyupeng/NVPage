M5_PATH=../system/
export M5_PATH

OPT_BIN=./build/X86/gem5.opt

DRAM_SIZE=512MB
PRAM_SIZE=512MB

PRAMIMAGE=512MB.pram.image
KERNEL=x86_64-vmlinux-3.4.91

DEBUG_FLAGS='Ethernet'

all: x86-opt

x86-opt:
	@echo "Generate X86 gem5.opt..."
	@scons build/X86/gem5.opt -j 8

run: atomic

atomic:
	@$(OPT_BIN) configs/example/fs.py --cpu-type=atomic --mem-size=$(DRAM_SIZE) --pram-size=$(PRAM_SIZE) --pram-image=$(PRAMIMAGE) --kernel=$(KERNEL) 

timing:
	@$(OPT_BIN) configs/example/fs.py --cpu-type=timing --mem-size=$(DRAM_SIZE) --pram-size=$(PRAM_SIZE) --pram-image=$(PRAMIMAGE) --kernel=$(KERNEL) 
