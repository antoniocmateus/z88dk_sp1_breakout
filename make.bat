del deploy\yab.tap
cls
zcc +zx -v -startup=31 -DWFRAMES=3  -clib=sdcc_iy -SO3 --max-allocs-per-node200000 --fsigned-char "@zproject.lst" -pragma-include:zpragma.inc -o deploy/yab -m
utils\bas2tap -a10 -sYAB src\loader.bas deploy\loader.tap
cd deploy
ren yab.map yab_CODE.map
z88dk-appmake +zx -b yab_CODE.bin -o game.tap --blockname yab --org 30000 --noloader
copy /b loader.tap + loading.tap + game.tap yab.tap
del game.tap
del loader.tap
del yab_*
cd ..

