#!/bin/bash

################################################################
# File Name: ./scripts/CTigger.sh
# Author: gaoyu
# Mail: gaoyu14@pku.edu.cn
# Created Time: 2017-12-02 15:43
################################################################

set -e

inf=$1
if [ $# -eq 2 ]; then
    ouf=$2
else
    ouf="a.t"
fi

build_dir="$(cd "$(dirname $0)" && pwd)/../build/"

toolA=$build_dir/CEeyore
toolB=$build_dir/EeyoreTigger

$toolA < $inf | $toolB > $ouf
