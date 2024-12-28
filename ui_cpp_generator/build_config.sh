#!/bin/bash

source external_config.sh

buildFolderPrefix="Build"
extraArg=" -DINCLUDES=${HOME}/Projects/Utils/include"
extraArgWin=$extraArg
extraArgMac=$extraArg
logArg=" -DLOG_ON=ON"
