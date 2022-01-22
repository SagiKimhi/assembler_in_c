#!/bin/tcsh

set testDir = "tests"

foreach file ( tests/* )
    echo -n "Do you wish to proceed to test ${file} (y/n)? "
    set usrAns = $<
    if ( "${usrAns}" == " " || "${usrAns}" == "y" || "${usrAns}" == "Y" ) then
        echo "\n`time ./asmake ${file}`\n"
    else
        echo "\nskipping to next test\n"
    endif
end
