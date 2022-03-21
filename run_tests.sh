#!/bin/tcsh

set testDir = "tests"

foreach file ( "${testDir}"/*.as )
    echo -n "Do you wish to proceed to test ${file:r} (y/n/stop)? "
    set usrAns = $<
    if ( "${usrAns}" == "" || "${usrAns}" == "y" || "${usrAns}" == "Y" ) then
        echo "\n`time ./assembler ${file:r}`\n"
    else if ( "${usrAns}" == "stop" ) then
        echo "\nAborting...\n"
        exit
    else
        echo "\nskipping to next test\n"
    endif
end
