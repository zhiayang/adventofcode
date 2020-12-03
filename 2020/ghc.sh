#!/usr/bin/env sh

# usage:
# ghc.sh $DAY

if test -z $@; then
	echo "Argument required"
else
	# echo "Compiling..."
	pushd "day"$1 > /dev/null
	ghc -i../../libs prog.hs && time ./prog
	popd > /dev/null
fi
