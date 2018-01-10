#!/bin/bash

################################################################
# File Name: scripts/CRiscv.sh
# Author: gaoyu
# Mail: gaoyu14@pku.edu.cn
# Created Time: 2018-01-10 17:06
################################################################

set -e

inf=$1
if [ $# -eq 2 ]; then
    ouf=$2
else
    ouf="result/out.s"
fi

build_dir="$(cd "$(dirname $0)" && pwd)/../build/"

toolA=$build_dir/CEeyore
toolB=$build_dir/EeyoreTigger
toolC=$build_dir/TiggerRiscv

$toolA < $inf | $toolB | $toolC > $ouf
