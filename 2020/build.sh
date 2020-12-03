#!/usr/bin/env sh

# usage:
# build.sh $DAY

if test -z $@; then
	echo "Argument required"
else
	# echo "Compiling..."
	pushd "day"$1 > /dev/null
	clang++ -O3 -std=c++17 -o prog -I../../libs prog.cpp && time ./prog
	popd > /dev/null
fi
