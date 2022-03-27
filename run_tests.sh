#!/bin/tcsh

if ( "$1" != "" ) then
    set testDir = "$1"
else
    set testDir = "tests"
endif

foreach file ( "${testDir}"/*.as )

    echo -n "Do you wish to proceed to test ${file:r} (y/n/stop)? "
    set usrAns = $<

    if ( "${usrAns}" == "" || "${usrAns}" == "y" || "${usrAns}" == "Y" ) then
        echo "\n`./assembler ${file:r} > ${file:r}.err`\n"

    else if ( "${usrAns}" == "stop" ) then
        echo "\nAborting...\n"
        exit

    else
        echo "\nskipping to next test\n"
    endif

end
