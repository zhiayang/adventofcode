#!/usr/local/bin/fish

set SYSROOT "$HOME/Desktop/Stuff/Orion\ Industries/flax/build/sysroot"
set FLAXC "$SYSROOT/usr/local/bin/flaxc"

if test -z $argv
	echo "Argument required"
else
	eval "$FLAXC" -sysroot $SYSROOT -run -Ox $argv[1]/prog.flx
end
