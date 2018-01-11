# miniC
## Make 

Packages which are required:
>flex, bison, c++11 package 

make easily

```
make 
```

or make faster

```
make -j32
```

## Tool

* minicc 
> minicc INPUTFILE (OUTPUTFILE)

OUTPUTFILE will be ``a.s`` if not set.

* scripts/RunRiscv.sh 

> ./RunRiscv.sh RISCV_FILE

require riscv-glibc-toolchain and qemu



## Build Eeyore

> ./build/CEeyore (0)

Read minic code from stdin. If argument is not set, then outputs eeyore code to stdout, else outputs eeyore code to ``result/gen.out.eeyore`` and generate AST to ``result/gen.ast``

## Build Tigger from Eeyore

> ./build/EeyoreTigger 

Read eeyore code from stdin, outputs tigger code to stdout.

## Build Riscv from Tigger

> ./build/TiggerRiscv 

Read tigger code from stdin, outputs riscv code to stdout.

## Build Tigger from miniC

> ./scripts/CTigger.sh INPUTFILE (OUTPUTFILE)

Read minic code from INPUTFILE, outputs tigger code to OUTPUTFILE, default ``a.t``

## TEST

~~put test data to ``test``, formatted as ``TEST.c``, ``TEST.in``, ``TEST.out``. And copy simulators(Eeyore, Tigger) to ``test``.~~
The test script will scan every sample from this dir, and build codes from scripts, which will be ran by the simulator and compare outputfile with the sample output.

>make testC

Check minic to Eeyore

>make testE

Check Eeyore to Tigger

>make testT 

Check Tigger to Riscv

## Other

>make tar

Make package based on Git.

>make parser

Make lex and yacc file.
