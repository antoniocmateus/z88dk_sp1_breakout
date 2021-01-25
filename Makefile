# vim: set ts=4 sw=4 noet:
#
#####################################################################################################
#                                                                                                   #
# Makefile for Z88DK running in docker.                                                             #
# Make sure the following files exist in this directory.                                            #
# .env                                                                                              #
# z88dk.yml                                                                                         #
#                                                                                                   #
#                                                                                                   #
#                                                                                                   #
# :set ts=4 // Set tab stop to 4 chars                                                              #
# :set noet // Do not expand the tab to spaces                                                      #
# :%retab! // If existing document that already has expanded tabs                                   #
# :set list // Make hidden chars visible                                                            #
#                                                                                                   #
#                                                                                                   #
#                                            _|                            _|                       #
#              _|      _|      _|    _|_|    _|  _|      _|_|_|  _|_|_|                             #
#              _|      _|      _|  _|    _|  _|_|      _|    _|  _|    _|  _|                       #
#                _|  _|  _|  _|    _|    _|  _|  _|    _|    _|  _|    _|  _|                       #
#                  _|      _|        _|_|    _|    _|    _|_|_|  _|    _|  _|                       #
#                                                                                                   #
#####################################################################################################

MV = mv -u
CAT = cat
MKDIR = mkdir -p
COMPOSE = docker-compose run
COMPOSE_FLAGS = --rm z88dk
BAS2TAP = utils/bas2tap
CMD = sh -c
CC = zcc
APPMAKE = appmake
CFLAGS = +zx -vn -m
MAXFLAGS = -SO3 --max-allocs-per-node200000 --fsigned-char
SDCC0  = -startup=0 -clib=sdcc_iy # IO channels enabled without control codes
SDCC1  = -startup=1 -clib=sdcc_iy # IO channels enabled with control codes
SDCC4  = -startup=4 -clib=sdcc_iy # 64 column mode
SDCC31 = -startup=31 -DWFRAMES=3 -clib=sdcc_iy # Most minimal one with no IO channels enabled


GAME = z88dk-breakout

TAPE = $(GAME:%=%.tap)

all: $(TAPE)

# The line below allows you to type `make z88dk-breakout` and it would automatically build the z88dk-breakout.tap recipe.
$(GAME): %: %.tap

z88dk-breakout.tap: src/$(*:%.c,%.h) gfx/$(*:%.asm,loading.scr)
	$(COMPOSE) $(COMPOSE_FLAGS) $(CC) $(CFLAGS) $(SDCC31) $(MAXFLAGS) "@zproject.lst" -pragma-include:zpragma.inc -o build/z88dk-breakout.bin
	$(BAS2TAP) -a10 -sz88break src/loader.bas build/loader.tap
	$(COMPOSE) $(COMPOSE_FLAGS) $(APPMAKE) +zx -b gfx/loading.scr -o gfx/loading.tap --blockname screen --org 16384 --noloader
	$(COMPOSE) $(COMPOSE_FLAGS) $(APPMAKE) +zx -b build/z88dk-breakout.bin -o build/game.tap --blockname z88dk-breakout --org 25124 --noloader
	$(CAT) build/loader.tap gfx/loading.tap build/game.tap > build/z88dk-breakout.tap
	$(RM) build/game.tap build/loading.tap build/loader.tap build/*.bin
	$(MKDIR) maps
	$(MV) build/z88dk-breakout.map maps/z88dk-breakout.map

.PHONY: clean $(GAME)

clean:
	$(RM) *.bin *.i *.asm *.map *.tap *.op* *.o *~ zcc_opt.def *.reloc

