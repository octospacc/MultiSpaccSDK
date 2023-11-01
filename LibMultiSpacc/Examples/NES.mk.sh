#!/dev/null
set -e
SdkRoot="${ProjectRoot}/../../.."

sh "${SdkRoot}/Tools/python3.sh" "${SdkRoot}/Tools/pilbmp2nes.py" -i "${ProjectRoot}/CHARS.png" -o ./CHARS.chr

for File in *.c
do cc65 -Oirs --target nes ${File} ${Defines}
done

for File in *.s 
do ca65 --target nes ${File}
done

ld65 -v -o ${AppName}.nes *.o *.lib nes.lib --config NROM256.cfg -DNES_MAPPER=0 -DNES_PRG_BANKS=2 -DNES_CHR_BANKS=1 -DNES_MIRRORING=0
