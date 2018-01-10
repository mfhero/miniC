#!/bin/bash

################################################################
# File Name: scripts/RunRiscv.sh
# Author: gaoyu
# Mail: gaoyu14@pku.edu.cn
# Created Time: 2018-01-10 17:15
################################################################

set -e

project_dir=$(cd $(dirname $0) && pwd)/../

GCC=/opt/riscv-glibc/bin/riscv64-unknown-linux-gnu-gcc 
LIBS=$project_dir/lib/miniCIO.s
QEMU=/opt/riscv-qemu/bin/qemu-riscv64
LINKD=/opt/riscv-glibc/sysroot/
EXE=$project_dir/result/a.out

$GCC -o $EXE -static $1 $LIBS
$QEMU -L $LINKD $EXE
