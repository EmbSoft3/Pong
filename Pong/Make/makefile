################################################################################
# Path
################################################################################

SOURCES_PATH := ../src
INCLUDES_PATH := ../include
INCLUDES_API_PATH := ../../Mk/Mk/Includes
WORKING_DIRECTORY_PATH := .

################################################################################
# List
################################################################################

# Directories listing
INCLUDE_DIRECTORIES := $(shell dir "$(INCLUDES_PATH)" /s /b /o:n /a:d) $(INCLUDES_PATH) \
                       $(shell dir "$(INCLUDES_API_PATH)" /s /b /o:n /a:d) $(INCLUDES_API_PATH)
SOURCE_DIRECTORIES  := $(shell dir "$(SOURCES_PATH)" /s /b /o:n /a:d) $(SOURCES_PATH)

# List of header files in 'include' directory
INCLUDE_FILES_LIST := $(foreach dir, $(INCLUDE_DIRECTORIES), $(wildcard $(dir)/*.h)) \
					  $(foreach dir, $(INCLUDE_DIRECTORIES), $(wildcard $(dir)/*.asm))

# List of source files in 'src' directory
SOURCE_FILES_LIST  := $(foreach dir, $(SOURCE_DIRECTORIES), $(wildcard $(dir)/*.c))

# List of assembly files in 'src' directory
ASM_FILES_LIST     := $(foreach dir, $(SOURCE_DIRECTORIES), $(wildcard $(dir)/*.asm))

# List of object files
OBJECT_FILES_LIST  := $(patsubst %.c,%.o, $(SOURCE_FILES_LIST)) $(patsubst %.asm,%.o, $(ASM_FILES_LIST))

# List of files to removed (.su)
SU_REMOVE_FILES_LIST := $(foreach dir, $(SOURCE_DIRECTORIES), $(if $(wildcard $(subst \,/, $(addsuffix /*.su, $(dir)))), \
					 	$(subst \,/, $(addsuffix /*.su, $(dir))),))

# List of files to removed (.o)
O_REMOVE_FILES_LIST := $(foreach dir, $(SOURCE_DIRECTORIES), $(if $(wildcard $(subst \,/, $(addsuffix /*.o, $(dir)))), \
				       $(subst \,/, $(addsuffix /*.o, $(dir))),))

# List of files to removed (.d)
D_REMOVE_FILES_LIST := $(foreach dir, $(SOURCE_DIRECTORIES), $(if $(wildcard $(subst \,/, $(addsuffix /*.d, $(dir)))), \
				       $(subst \,/, $(addsuffix /*.d, $(dir))),))
					  
################################################################################
# Toolchain Path
################################################################################

TOOLCHAIN_PATH := C:/STM32Toolchain/gnu-arm/10.2021.10/bin

CC := @$(TOOLCHAIN_PATH)/arm-none-eabi-gcc
LL := @$(TOOLCHAIN_PATH)/arm-none-eabi-g++

STRIP  := @$(TOOLCHAIN_PATH)/arm-none-eabi-strip
OBJCPY := @$(TOOLCHAIN_PATH)/arm-none-eabi-objcopy
PRINT  := @$(TOOLCHAIN_PATH)/arm-none-eabi-size

CAT := @cat
CPY := @copy /y
RM  := @rm -rf

################################################################################
# Artefacts
################################################################################

BUILD_ARTIFACT_NAME := pongGameRelease
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT := $(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

################################################################################
# Linker Script
################################################################################

LD_SCRIPT := -T "../Make/linker.ld"

################################################################################
# Compiler/Linker Options
################################################################################

# Options Controlling C Dialect 
CDIAL_OPTIONS := -std=c18 -fno-asm -fno-builtin -fno-cond-mismatch -ffreestanding -fsigned-bitfields -fsigned-char -fsso-struct=little-endian

# Arm Options
ARM_OPTIONS := -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mno-unaligned-access -mlittle-endian

# Static Analyzer Options
ANALYZER_OPTIONS := -fanalyzer

# Debugging Options
DEBUG_OPTIONS :=
ifeq ($(MAKECMDGOALS),Debug)
DEBUG_OPTIONS := -g3 -ggdb -fno-eliminate-unused-debug-symbols
endif

#Strip Options
STRIP_OPTIONS := -s

# Optimization Options
OPTIMIZATION_OPTIONS := -Ofast -fno-inline-functions -fno-short-enums -fno-move-loop-invariants
ifeq ($(MAKECMDGOALS),Debug)
OPTIMIZATION_OPTIONS := -O0 -fno-inline-functions -fno-short-enums -fno-move-loop-invariants
endif

# Options to Request or Suppress Warnings 
WARN_OPTIONS := -Wpedantic -pedantic-errors -Werror -Wall -Wextra -Wdouble-promotion -Wnull-dereference -Wshift-negative-value -Wswitch-default -Wswitch-enum -Wswitch-bool \
 -Wswitch-unreachable -Wstrict-overflow=5 -Wduplicated-branches -Wduplicated-cond -Wdiscarded-qualifiers -Wdiscarded-array-qualifiers -Wincompatible-pointer-types -Wint-conversion \
 -Wint-to-pointer-cast -Wpointer-to-int-cast -Wdiv-by-zero -Wsystem-headers -Wtrampolines -Wfloat-equal -Wno-traditional -Wdeclaration-after-statement -Wshadow \
 -Wshadow=local -Wunsafe-loop-optimizations -Wpointer-compare -Wno-undef -Wunused-macros -Wbad-function-cast -Wno-cast-qual -Wcast-align -Wcast-align=strict -Wwrite-strings \
 -Wconversion -Wjump-misses-init -Waddress-of-packed-member -Wlogical-op -Waggregate-return -Waggressive-loop-optimizations -Wpointer-arith -Wattributes \
 -Wstrict-prototypes -Wold-style-definition --warn-missing-parameter-type -Wmissing-prototypes -Wmissing-declarations -Wmultichar -Wnormalized=none -Wattribute-warning \
 -Wdeprecated -Wdeprecated-declarations -Woverflow -Woverride-init-side-effects -Wpacked -Wno-packed-bitfield-compat -Wpadded -Wredundant-decls -Wnested-externs -Winline \
 -Wno-long-long -Wvla -Woverlength-strings

# Pic Options
PIC_OPTIONS := -fPIC -mlong-calls

# Linking Options
LD_OPTIONS := -nostartfiles -nodefaultlibs -nolibc -nostdlib -shared -z max-page-size=65536 

#C flags
C_FLAGS := -x c $(CDIAL_OPTIONS) $(ARM_OPTIONS) $(ANALYZER_OPTIONS) $(OPTIMIZATION_OPTIONS) $(WARN_OPTIONS) $(PIC_OPTIONS) -fmessage-length=0 -nostdinc -fstack-usage

#Asm flags
ASM_FLAGS := -x assembler-with-cpp $(ARM_OPTIONS) $(ANALYZER_OPTIONS) $(OPTIMIZATION_OPTIONS) $(WARN_OPTIONS) $(PIC_OPTIONS) -fmessage-length=0 -nostdinc -fstack-usage

#h flags
H_FLAGS = $(foreach dir,$(INCLUDE_DIRECTORIES), $(addprefix -I", $(addsuffix ", $(dir))))

#Linker flags
LD_FLAGS := $(LD_OPTIONS) $(LD_SCRIPT) -Wl,-Map,"$(BUILD_ARTIFACT_NAME).map" -o "$(BUILD_ARTIFACT)"

################################################################################
# Clean Target
################################################################################	

clean: 
	
# SU files
ifneq ($(strip $(SU_REMOVE_FILES_LIST)),)
	$(RM) $(SU_REMOVE_FILES_LIST)
endif	

# O files
ifneq ($(strip $(O_REMOVE_FILES_LIST)),)
	$(RM) $(O_REMOVE_FILES_LIST)
endif

# D files
ifneq ($(strip $(D_REMOVE_FILES_LIST)),)
	$(RM) $(D_REMOVE_FILES_LIST)
endif	

# MAP files
ifneq ($(strip $(wildcard *.map)),)
	$(RM) *.map
endif

# ELF files
ifneq ($(strip $(wildcard *.elf)),)
	$(RM) *.elf
endif

# SREC files
ifneq ($(strip $(wildcard *.srec)),)
	$(RM) *.srec
endif

# Text files
ifneq ($(strip $(wildcard *.txt)),)
	$(RM) *.txt 
endif

################################################################################
# Init Target
################################################################################

init: 

#	Create objects listing
	$(file > $(WORKING_DIRECTORY_PATH)/objects_listing.txt, $(subst \,/, $(OBJECT_FILES_LIST)))
#	Create empty file (echo. not working)
	@echo. > $(WORKING_DIRECTORY_PATH)/ln.txt

################################################################################
# Finit Target
################################################################################

finit: 

#	Clean Working Directory (keep object files)
	$(RM) ln.txt
	$(RM) objects_listing.txt
	$(RM) output.txt

################################################################################
# C Source Target
################################################################################

%.o: %.c $(INCLUDE_FILES_LIST)
	@echo Building file: $< 
	@echo Invoking: Cross ARM GNU C Compiler
	@echo $(C_FLAGS) $(DEBUG_OPTIONS) 
	$(CC) $(C_FLAGS) $(DEBUG_OPTIONS) $(H_FLAGS) -o $@ -c $<
	@echo Finished building: $<
	$(CAT) $(WORKING_DIRECTORY_PATH)/ln.txt
	
################################################################################
# ASM Source Target
################################################################################

%.o: %.asm $(INCLUDE_FILES_LIST)

	@echo Building file: $< 
	@echo Invoking: Cross ARM GNU Assembler 
	@echo $(ASM_FLAGS) $(DEBUG_OPTIONS)
	$(CC) $(ASM_FLAGS) $(DEBUG_OPTIONS) $(H_FLAGS) -o $@ -c $<
	@echo Finished building: $<
	$(CAT) $(WORKING_DIRECTORY_PATH)/ln.txt

################################################################################
# Link Target
################################################################################

$(BUILD_ARTIFACT) : $(OBJECT_FILES_LIST) 
	
#   Link file	
	@echo Building target: $@
	@echo Invoking: Cross ARM GNU C++ Linker
	@echo $(LD_FLAGS) $(DEBUG_OPTIONS) @$(WORKING_DIRECTORY_PATH)/objects_listing.txt
	$(LL) $(LD_FLAGS) $(DEBUG_OPTIONS) @$(WORKING_DIRECTORY_PATH)/objects_listing.txt
	@echo Finished building target: $(BUILD_ARTIFACT)
	$(CAT) $(WORKING_DIRECTORY_PATH)/ln.txt

################################################################################
# Print Target
################################################################################

print:

	@echo Target: $@
	@echo Invoking: Cross ARM GNU Print Size
	$(PRINT) --format=berkeley --totals $(BUILD_ARTIFACT)
	@echo Finished printing target: $(BUILD_ARTIFACT)
	$(CAT) $(WORKING_DIRECTORY_PATH)/ln.txt

################################################################################
# Strip Target
################################################################################

strip: 

	@echo Target: $@	
#	Remove all symbols
	@echo Invoking: Cross ARM GNU Strip Flash Image
	$(STRIP) -s $(BUILD_ARTIFACT)
	@echo Finished building target: $(BUILD_ARTIFACT)
	$(CAT) $(WORKING_DIRECTORY_PATH)/ln.txt

################################################################################
# All Target
################################################################################

all: init $(BUILD_ARTIFACT) print strip finit

################################################################################
# Release Target
################################################################################

Release: init $(BUILD_ARTIFACT) print strip finit

################################################################################
# Debug Target
################################################################################

Debug: init $(BUILD_ARTIFACT) print finit

################################################################################
# Others
################################################################################	

.PHONY: all clean Debug Release init finit link strip print
.NOTPARALLEL: all Release Debug	
	
