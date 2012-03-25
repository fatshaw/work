#!/usr/bin/bash

GCC_ROOT=/home/jinsongj/srcs
L_PATH=${GCC_ROOT}/gcc-binutils-2.21/bin:${GCC_ROOT}/binutils-install/bin/
#L_LIB_PATH=/home/jinsongj/srcs/gcc-install/lib
L_LIB_PATH=${GCC_ROOT}/libs/lib
OLD_LD_LIBRARY_PATH=${LD_LIBRARY_PATH}
OLD_PATH=${PATH}
LIB_PATH_SET=0
PATH_SET=0

function setup_latest_gcc()
{
    echo $PATH | grep "${L_PATH}" > /dev/null
    if [ "$?" -ne "0" ]
    then 
        OLD_PATH=${PATH}
        export PATH=${L_PATH}:$PATH
        echo "NEW PATH: " $PATH   
        PATH_SET=1
    fi
    echo $LD_LIBRARY_PATH | grep "${L_LIB_PATH}" > /dev/null
    if [ "$?" -ne "0" ]
    then 
        OLD_LD_LIBRARY_PATH=${LD_LIBRARY_PATH}
        export LD_LIBRARY_PATH=${L_LIB_PATH}:${LD_LIBRARY_PATH}
        echo "NEW LD_LIBRARY_PATH: " $LD_LIBRARY_PATH   
        LIB_PATH_SET=1
    fi
}

function unsetup_latest_gcc()
{
    if [ "${PATH_SET}" == "1" ]
    then
        KEY=`echo ${L_PATH} | sed 's/\//\\\\\//g'`
        OLD_PATH=`echo ${OLD_PATH} | sed "s/${KEY}://g"`

        export PATH=${OLD_PATH}
        PATH_SET=0
    fi
    echo "NEW PATH: " $PATH   

    if [ "${LIB_PATH_SET}" == "1" ]
    then
        KEY=`echo ${L_LIB_PATH} | sed 's/\//\\\\\//g'`
        OLD_LD_LIBRARY_PATH=`echo ${OLD_LD_LIBRARY_PATH} | sed "s/${KEY}://g"`

        export LD_LIBRARY_PATH=${OLD_LD_LIBRARY_PATH}
        LIB_PATH_SET=0
    fi
    echo "NEW LD_LIBRARY_PATH: " $LD_LIBRARY_PATH   
}

function changegcc()
{
    if [ "${PATH_SET}" == "1" ]
    then
        unsetup_latest_gcc
        echo "Now using machine's gcc."
    else
        setup_latest_gcc
        echo "Now using latest gcc."
    fi
}

function egcc()
{
    cd ${GCC_ROOT}
}

function ec()
{
	cd ~
}
