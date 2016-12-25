#!/usr/local/bin/fish

set SYSROOT "$HOME/Desktop/Stuff/Code/flax/build/sysroot"
set FLAXC "$SYSROOT/usr/local/bin/flaxc"

# usage:
# build.fish $YEAR $DAY

if test -z "$argv"
	echo "Argument required"
else
	# echo "Compiling..."
	eval time "$FLAXC" -sysroot $SYSROOT -O3 -run $argv[1]/"day"$argv[2]/prog.flx
	# echo -e "Compiled.\n"
	# eval $argv[1]/"day"$argv[2]/prog
end
