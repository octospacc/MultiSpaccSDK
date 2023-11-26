#!/bin/bash

#[ "$(whoami)" != root ] && {
	echo "This script must probably run as root".
#	exit -1
#}

mkdir -p /tmp /opt/Sdk

apt update
apt install -y \
	make wine curl p7zip-full tar xz-utils python3 python3-pil \
	gcc mingw-w64 cc65 emscripten \
	libsdl1.2-dev libsdl-image1.2-dev libsdl-ttf2.0-dev libsdl-mixer1.2-dev \
	libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev \
;

curl -o /tmp/mingw32.7z https://hlb0.octt.eu.org/Drive/Misc/mingw32-9.2.0.7z.sfx.exe
7z x /tmp/mingw32.7z || true
cp ./mingw32/bin/*.dll ./mingw32/libexec/gcc/mingw32/9.2.0/
mv ./mingw32 /opt/Sdk/mingw32

curl -o /tmp/devkitpro.tar.xz https://hlb0.octt.eu.org/Drive/Misc/devkitpro-SDK-Linux-glibc-amd64.tar.xz
tar xf /tmp/devkitpro.tar.xz
mv ./opt/devkitpro /opt/devkitpro

#curl -o /tmp/dkp.sh https://apt.devkitpro.org/install-devkitpro-pacman
#yes | bash /tmp/dkp.sh
#dkp-pacman -Sy nds-dev

cat > .env << [EOF]
export DEVKITPRO=/opt/devkitpro
export DEVKITA64=/opt/devkitpro/devkitA64
export DEVKITARM=/opt/devkitpro/devkitARM
export DEVKITPPC=/opt/devkitpro/devkitPPC
[EOF]

source .env
