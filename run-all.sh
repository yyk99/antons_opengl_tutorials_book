#!/bin/bash

for i in $(/bin/find ??_* -name \*.exe) ; do
	echo $i
	exe=$(realpath $i)
	workdir=$(dirname $i)
	(
		cd $workdir
		$exe 
	)
done
