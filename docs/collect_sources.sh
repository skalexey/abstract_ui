#!/usr/bin/sh

rm -rf Src

mkdir Src

includePath="../include/"
srcPath="../src/"

cd Src
	cp -r "../$includePath" abstract_ui
	cd abstract_ui
		cp -r "../../$srcPath" src
	cd ..
cd ..
