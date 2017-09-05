#!/bin/bash

for f in `git diff --name-only|grep '[h|c]pp$'`
do
	echo formatting $f
	clang-format -style=file -i ${f}
done
