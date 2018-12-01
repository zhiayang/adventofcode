#!/usr/local/bin/fish

set SYSROOT "$HOME/Desktop/Stuff/Code/flax/build/sysroot"
set FLAXC "$SYSROOT/usr/local/bin/flaxc"

# usage:
# build.fish $YEAR $DAY

if test -z "$argv"
	echo "Argument required"
else
	# echo "Compiling..."
	eval clang++ -O3 -g -std=c++17 -o prog "day"$argv[1]/prog.cpp; and time ./prog
end
