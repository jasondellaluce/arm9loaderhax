.PHONY : all hax firm0 firm1 sector stage2 installer clean

TARGET		=	arm9loaderhax
PYTHON 		=	python
INDIR		=	data_input
OUTDIR		=	data_output

all : $(OUTDIR) hax installer

hax : $(OUTDIR) firm0 firm1 sector stage2

$(OUTDIR):
	@[ -d $(OUTDIR) ] || mkdir -p $(OUTDIR)

firm0 :
	@cd payload_stage1 && make
	@cp $(INDIR)/new3ds90.firm $(OUTDIR)/firm0.bin
	@dd if=payload_stage1/payload_stage1.bin of=$(OUTDIR)/firm0.bin bs=512 seek=1922 conv=notrunc
	@echo FIRM0 done!

firm1 :	
	@cp $(INDIR)/new3ds10.firm $(OUTDIR)/firm1.bin
	@echo FIRM1 done!

sector :
	@$(PYTHON) common/sector_generator.py $(INDIR)/secret_sector.bin $(INDIR)/otp.bin $(OUTDIR)/sector.bin
	@echo SECTOR done!

stage2:
	@cd payload_stage2 && make
	@dd if=payload_stage2/payload_stage2.bin of=$(OUTDIR)/firm1.bin bs=512 seek=1936 conv=notrunc
	@cp payload_stage2/payload_stage2.bin $(OUTDIR)/stage0x5C000.bin 

installer:
	@cp $(OUTDIR)/sector.bin payload_installer/brahma2/data/sector.bin
	@cp $(OUTDIR)/firm0.bin payload_installer/brahma2/data/firm0.bin
	@cp $(OUTDIR)/firm1.bin payload_installer/brahma2/data/firm1.bin
	@cp $(OUTDIR)/stage0x5C000.bin  payload_installer/brahma2/data/stage2.bin
	@cd payload_installer && make TARGET=../$(OUTDIR)/$(TARGET)
	@echo INSTALLER done!
	
clean:
	@echo clean...
	@cd payload_stage1 && make clean
	@cd payload_stage2 && make clean
	@cd payload_installer && make clean TARGET=../$(TARGET)
	@rm -fr $(OUTDIR) payload_installer/brahma2/data/*.bin
	