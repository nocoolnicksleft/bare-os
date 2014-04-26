###############################################################################
#	makefile
#	 by Alex Chadwick
#
#	A makefile script for generation of raspberry pi kernel images.
###############################################################################

# The toolchain to use. arm-none-eabi works, but there does exist 
# arm-bcm2708-linux-gnueabi.
ARMGNU ?= arm-none-eabi

#EABILIB = /home/bseip/gcc-arm-none-eabi-4_8-2013q4/lib/gcc/arm-none-eabi/4.8.3
EABILIB = /home/bseip/arm-2008q3/arm-none-eabi/lib/
#LIBRARIES = $(EABILIB)/libgcc.a $(EABILIB)/libgcov.a
#LIBRARIES = /home/bseip/arm-none-eabi-src/build/newlib-1.20.0/arm-none-eabi/newlib/libc.a 
#LIBRARIES = /home/bseip/gcc-arm-none-eabi-4_8-2013q4/lib/gcc/arm-none-eabi/4.8.3/libgcc.a
#LIBRARIES += /home/bseip/gcc-arm-none-eabi-4_8-2013q4/lib/gcc/arm-none-eabi/4.8.3/libgcov.a

LIBPATH1 := /home/bseip/MentorGraphics/Sourcery_CodeBench_Lite_for_ARM_EABI/arm-none-eabi/lib
LIBPATH2 := /home/bseip/MentorGraphics/Sourcery_CodeBench_Lite_for_ARM_EABI/lib/gcc/arm-none-eabi/4.8.1
LIBRARIES := --library-path=$(LIBPATH1) --library-path=$(LIBPATH2) -lgcc -lc -lcs3 -lcs3unhosted -lcs3arm -lstdc++
LIBRARIES += /home/bseip/MentorGraphics/Sourcery_CodeBench_Lite_for_ARM_EABI/lib/gcc/arm-none-eabi/4.8.1/crti.o	
LIBRARIES += /home/bseip/MentorGraphics/Sourcery_CodeBench_Lite_for_ARM_EABI/lib/gcc/arm-none-eabi/4.8.1/crtbegin.o	
LIBRARIES += /home/bseip/MentorGraphics/Sourcery_CodeBench_Lite_for_ARM_EABI/lib/gcc/arm-none-eabi/4.8.1/crtend.o	

# The intermediate directory for compiled object files.
BUILD = build/

# The directory in which source files are stored.
SOURCE = source/

# The name of the output file to generate.
TARGET = kernel.img

# The name of the assembler listing file to generate.
LIST = kernel.list

# The name of the map file to generate.
MAP = kernel.map

# The name of the linker script to use.
LINKER = kernel.ld


msubdirs := source/ source/fs/ source/gapi/ source/fonts/ source/fatfs/ source/mmc/
VPATH := source/:source/fs/:source/gapi/:source/fonts/:source/fatfs/:source/mmc/
	
msourcesa := $(wildcard $(addsuffix *.cpp,$(msubdirs)))
msourcesb := $(wildcard $(addsuffix *.c,$(msubdirs)))
msourcesc := $(wildcard $(addsuffix *.s,$(msubdirs)))

msourcesall := $(msourcesa) $(msourcesb) $(msourcesc)
	
mobjectsa := $(patsubst $(SOURCE)%.cpp,$(BUILD)%.o,$(msourcesa))
mobjectsb := $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(msourcesb))
mobjectsc := $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(msourcesc))
	
mobjectsall := $(mobjectsa) $(mobjectsb) $(mobjectsc)
	
	
# The names of all object files that must be generated. Deduced from the 
# assembly code files in source.
OBJECTS := $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))
OBJECTS += $(patsubst $(SOURCE)%.cpp,$(BUILD)%.o,$(wildcard $(SOURCE)*.cpp))
OBJECTS += $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))

# Rule to make everything.
all: $(TARGET) $(LIST)

# Rule to remake everything. Does not include clean.
rebuild: all

# Rule to make the listing file.
$(LIST) : $(BUILD)output.elf
	$(ARMGNU)-objdump -d $(BUILD)output.elf > $(LIST)

# Rule to make the image file.
$(TARGET) : $(BUILD)output.elf
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET) 

# Rule to make the elf file.
$(BUILD)output.elf : $(mobjectsall) $(LINKER)
	$(ARMGNU)-ld -v  --start-group $(mobjectsall) $(LIBRARIES) --end-group -Map $(MAP) -o $(BUILD)output.elf -T $(LINKER)

# Rule to make the object files.
%.o: %.s $(BUILD)
	$(ARMGNU)-as -I $(SOURCE) $< -o $@
	
# Rule to compile c files	
BASEFLAGS   := -Wall -O2 -nostdlib -nostartfiles -ffreestanding -marm -mcpu=arm1176jzf-s

$(BUILD)%.o: $(SOURCE)%.c | $(BUILD)
	$(ARMGNU)-gcc $(BASEFLAGS) -I $(SOURCE) -c $< -o $@
	
$(BUILD)%.o: %.cpp | $(BUILD)
	$(ARMGNU)-g++ $(BASEFLAGS) -I $(SOURCE) -c $< -o $@
	
$(BUILD)%.o: %.s | $(BUILD)
	$(ARMGNU)-as -I $(SOURCE) $< -o $@
	
$(BUILD):
	mkdir $@
	mkdir $@/mmc
	mkdir $@/gapi
	mkdir $@/fonts
	mkdir $@/fatfs
	@echo Objects 
	@echo $(OBJECTS)
	@echo MSubdirs
	@echo $(msubdirs)
	@echo MSources
	@echo $(msourcesall)
	@echo MObjects 
	@echo $(mobjectsall)
	
	
# Rule to clean files.
clean : 
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(LIST)
	-rm -f $(MAP)
