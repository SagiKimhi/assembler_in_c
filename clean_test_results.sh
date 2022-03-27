#!/bin/tcsh

if ( "$1" != "" ) then
    set testDir = "$1"
else
    set testDir = "tests"
endif

rm ${testDir}/*.am ${testDir}/*.ob ${testDir}/*.ent ${testDir}/*.ext ${testDir}/*.err
