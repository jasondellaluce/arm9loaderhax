#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

#---------------------------------------------------------------------------------
export TARGET 	:= payload_installer
EXPLOITDIR		:= brahma2
LOADERDIR		:= installer

# Targets
.PHONY: clean all

all:
	@echo build ...
	@cd $(LOADERDIR) && make OUTPUT=../../$(EXPLOITDIR)/data/starter
	@cd $(EXPLOITDIR) && make OUTPUT=../../$(TARGET)
	@cp $(EXPLOITDIR)/data/starter.bin $(TARGET).bin
	@rm -f *.elf $(TARGET).elf
	@echo done!!

clean:
	@echo clean ...
	@cd $(LOADERDIR) && make clean OUTPUT=../../$(EXPLOITDIR)/data/starter
	@cd $(EXPLOITDIR) && make clean OUTPUT=../../$(TARGET)
	@rm -f *.elf *.3dsx *.smdh