.PHONY: all clean

all: crt0.o neslib2.lib

neslib2.lib: lz4vram.o pad.o rand.o memfill.o vram_read.o vram_unrle.o oam_meta_spr_clip.o oam_meta_spr_pal.o oam_meta_spr.o oam_spr.o oam_clear_fast.o split.o splitxy.o
	ar65 a neslib2.lib $^

%.o: %.s $(wildcard *.s *.sinc)
	cl65 -t nes -Oisr -g -c $*.s 

clean:
	rm -f *.o *.lst neslib2.lib
