#!/usr/bin/env python
# coding=utf-8

import os, sys
from commands import getstatusoutput
import argparse

tmp_dir = "test/tmp"

def run_with_check(cmd):
    status, output = getstatusoutput(cmd)
    if status:
        print "\033[33m", cmd, "\033[0m"
        print status
        print "\033[31m", output, "\033[0m"
        raise Exception('Error', 'Error')

def score_task1(line):
    basename = line.replace('test/', "")
    basename = basename.replace(".c", "")
    print basename
    exec_file = "%s/%s.ee" % (tmp_dir, basename)
    out_data = "%s/%s.e_out" % (tmp_dir, basename)
    run_with_check('./build/CEeyore < %s > %s' \
                   % (line, exec_file))
    run_with_check('./test/Eeyore %s < test/%s.in > %s' \
                   % (exec_file, basename, out_data))
    try:
        run_with_check('diff -w test/%s.out %s' \
                   % (basename, out_data))
    except:
        print "\033[31mTest <%s> wrong\033[0m" % basename
    else:
        print "\033[32mTest <%s> accept\033[0m" % basename

def score_task2(line):
    basename = line.replace("test/", "")
    basename = basename.replace(".c", "")
    print basename 
    exec_file = "%s/%s.t" % (tmp_dir, basename)
    out_data = "%s/%s.t_out" % (tmp_dir, basename)
    run_with_check("./scripts/CTigger.sh %s %s" \
                   % (line, exec_file))
    run_with_check("./test/Tigger %s < test/%s.in > %s" \
                   % (exec_file, basename, out_data))
    try:
        run_with_check('diff -w test/%s.out %s' \
                   % (basename, out_data))
    except:
        print "\033[31mTest <%s> wrong\033[0m" % basename
    else:
        print "\033[32mTest <%s> accept\033[0m" % basename

score_method = [None ,score_task1, score_task2]

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("--task", type=int, required = True, nargs = '+')
    args = p.parse_args()

    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    os.chdir("../")
    status, files = getstatusoutput('find test/ -maxdepth 1 -name "*.c"')

    if os.path.isdir(tmp_dir):
       # run_with_check("rm -r %s" % tmp_dir)
       pass
    else:
        os.makedirs(tmp_dir)
    for line in files.split():
        for i in args.task:
            score_method[i](line)
