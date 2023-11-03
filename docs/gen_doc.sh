#!/usr/bin/sh

rm -rf html
./collect_sources.sh
cd Src
doxygen ../doxyfile.txt
rm -rf Src
cd ..
