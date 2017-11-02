#!/usr/bin/env python
# coding=utf-8

import os, sys
from commands import getstatusoutput

def run_with_check(cmd):
    status, output = getstatusoutput(cmd)
    if status:
        print cmd 
        print status
        print output
        raise Exception('Error', 'Error')

if __name__ == "__main__":
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    os.chdir("../")
    status, files = getstatusoutput('find test/ -maxdepth 1 -name "*.c"')

    tmp_dir = "test/tmp"
    if os.path.isdir(tmp_dir):
        run_with_check("rm -r %s" % tmp_dir)
    os.makedirs(tmp_dir)
    for line in files.split():
        basename = line[5:-2]
        print basename
        exec_file = "%s/%s.ee" % (tmp_dir, basename)
        out_data = "%s/%s.out" % (tmp_dir, basename)
        run_with_check('./a.out < %s > %s' \
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
