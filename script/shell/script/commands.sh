#!/usr/bin/bash

# Some script to make the building process easier

USER_HOME=/home/xfguan
DEV_ROOT=${USER_HOME}/dev
TPO_ROOT=${DEV_ROOT}/tpo
TOBEY_ROOT=${DEV_ROOT}/tobey
XLC_PATH=/gsa/tlbgsa/projects/x/xlcmpbld/run/vacpp/111/sles10_ppc/daily/latest/bin
XLF_PATH=/gsa/tlbgsa/projects/x/xlcmpbld/run/xlf/dev/sles10_ppc/daily/latest/bin
WC_PATH=/gsa/tlbgsa/projects/x/xlcmpbld/run/wct.dev/sles10_ppc/daily/latest/bin
CHECK_OUT_SH_DIR=/nfs/basilisk/home/jinsongj/bin
CHECK_OUT_SH_DIR=/nfs/rugen/home/jinsongj/bin
CHECK_OUT_SH=${CHECK_OUT_SH_DIR}/create_tobey_builddir.pl
XLC=${XLC_PATH}/xlc
XLF2003=${XLF_PATH}/xlf2003_r
EXEC=${XLC}
FED=xlCcode
TMP_SH=tmprun.sh
TMP_DSM=tmpd.asm
THIS_FILE=${USER_HOME}/dev/scripts/commands.sh
DEBUGGER=cgdb
CMVC_ID=xfguan
WORK_DIR=${TOBEY_ROOT}

USER_LIB=${USER_HOME}/lib

PT_DIR=${USER_HOME}/practise/tobey

# Export some variable
echo ${PATH} | grep "${XLC_PATH}" > /dev/null
if [ "$?" -ne "0" ]
then 
    export PATH=${XLC_PATH}:${PATH}
fi
echo ${PATH} | grep "${XLF_PATH}" > /dev/null
if [ "$?" -ne "0" ]
then 
    export PATH=${XLF_PATH}:${PATH}
fi
echo ${LD_LIBRARY_PATH} | grep "${XLC_PATH}" > /dev/null
if [ "$?" -ne "0" ]
then 
    export LD_LIBRARY_PATH=${USER_LIB}:${LD_LIBRARY_PATH}
fi
echo ${PATH} | grep "${WC_PATH}" > /dev/null
if [ "$?" -ne "0" ]
then 
    export PATH=${WC_PATH}:${PATH}
fi

export CMVC_FAMILY=aix
export CMVC_AUTH_METHOD=PW
export CMVC_USERID=${CMVC_ID}

# Cleanup lanaguage perference
unset LANG

GCC_SH=${DEV_ROOT}/scripts/usegcc.sh

# Setup ultility functions
test -e ${GCC_SH} \
         && echo "GCC script is set." && . ${GCC_SH} \
         || echo "GCC path setting fails."

function editusegcc()
{
    vim ${GCC_SH}
    resource
}
         
# Enter TOBEY directory
function etb()
{
    cd ${TOBEY_ROOT}
}

function replacedbg()
{

    if [ -e "xlccode" ]
    then
        rm -f xlCcode
        rm -f xlfcode
        cp xlccode xlCcode
        cp xlFcode xlfcode
    fi
}

function ept()
{
    cd ${PT_DIR}
}

# Build TOBEY 
function ltb()
{
    rm -f xlCcode xlfcode
    ln -s dev.cw_main xlCcode 
    ln -s dev.cw_main xlfcode 
}

function btb()
{
    T_DIR=${TOBEY_ROOT}
    [ "$1" != "" ] && [ -d "$1" ] && T_DIR="$1"
	cd ${T_DIR} 
    make PROCS=6 BUILD_PLATFORM=sles10 dev.cw_main 
    ltb
    cd -
}

alias btbpwd='btb `pwd`'

# Clean TOBEY
function ctb()
{
    cd ${TOBEY_ROOT} 
    make clobber BUILD_PLATFORM=sles10
    cd -
}

# Sanity Test
function sanitytb()
{
#    mkdir ~/tobey.sanity
    /tobeytest/bin/runsanity -dev -p${TOBEY_ROOT}
}

# Enter TPO directory
function etpo()
{
	cd ${TPO_ROOT}
}

# Build TPO
function btpo()
{
	cd ${TPO_ROOT} && make -j4 && cd -
}

# Clean TOBEY
function ctpo()
{
	cd ${TPO_ROOT} && make clean && cd -
}

# Sanity Test
function sanitytpo()
{
    cd ${TPO_ROOT}
    cd -
}

function debugtb()
{
    if [ "$1" == "" ]
    then
        echo "No parameter.";
    else
        TMP_FILE=.tmp_$$
        INIT=.gdbinit

        # Build binary
        ${XLC} -tb -B${TOBEY_ROOT}/ $@ |   \
                grep -v "^rm\ " | tee ./${TMP_SH} | \
                grep "^${TOBEY_ROOT}/xlCcode" \
                > $TMP_FILE
        if [ "$?" != "0" ]
        then
            rm -f ${TMP_FILE}
            return
        fi
        chmod +x ${TMP_SH}
        sh ${TMP_SH}
        rm ${TMP_SH}

        COMMAND=`cat ${TMP_FILE} | awk '{print $1}'`
        ARGS=`cat ${TMP_FILE} | sed 's/^.*xlCcode\ //g'`
        TARGET=`echo $1 |sed "s/\.cpp/\.o/g" |sed "s/\.[c|C]/\.o/g"`
        rm -f ${TMP_FILE}
        echo "file ${COMMAND}" > ${INIT}
        echo "set args " $ARGS >> ${INIT}
        echo "break main" >> ${INIT}
        if [ "$2" != "" ]
        then
            cat $2 >> ${INIT}
        fi
        cgdb
        rm -f ${INIT}
    fi
}

alias dbgtb='debugtb'

function debugtobey()
{
    if [ "$1" == "" ]
    then
        echo "No parameter.";
    else
        TMP_FILE=.tmp_${FED}
        BAK_FILE=.${FED}.bak
        echo "#!/usr/bin/ksh" > ${TMP_FILE}
        echo "${DEBUGGER} --args \`dirname \$0\`/dev.cw_main  \$*" >> ${TMP_FILE}
        chmod +x ${TMP_FILE}
        rm ${WORK_DIR}/${FED}

        mv ${TMP_FILE} ${WORK_DIR}/${FED}
        ${EXEC} -tb -B${WORK_DIR}/ -V $@
	    cd ${WORK_DIR} && ltb && cd -
        
        rm -f ${TMP_FILE}
    fi
}

function debugftobey()
{
    FED=xlfcode
    EXEC=${XLF2003}
    debugtobey $@
    EXEC=${XLC}
    FED=xlCcode
}

function debugtobeypwd()
{
    WORK_DIR=`pwd`
    debugtobey $@
    WORK_DIR=${TOBEY_ROOT}
}

function debugftobeypwd()
{
    EXEC=${XLF2003}
    WORK_DIR=`pwd`
    debugftobey $@
    WORK_DIR=${TOBEY_ROOT}
    EXEC=${XLC}
}

function debugtobey64()
{
    DEBUGGER=gdb64
    debugtobey $@
    DEBUGGER=cgdb
}

function xlcdebug()
{
    if [ "$1" == "" ]
    then
        echo "No parameter.";
    else
        TARGET=`echo $1 |sed "s/\.cpp//g" |sed "s/\.[c|C]//g"`
        ${XLC} -tb -B${TOBEY_ROOT}/ -O0 -g $1 -o ${TARGET}
        if [ "$?" == "0" ]
        then
            gdb ${TARGET}
        fi
    fi
}

alias xldbg='xlcdebug'

function catmaps()
{
    PROC=`ps -e |grep $1`
    LINE=`echo $PROC | wc -l | awk '{print $1}'`
    I=1
    while [ "$I" -le "$LINE" ]
    do
        NAME=`echo  $PROC | head -$I | tail -1 | awk '{print $4}'`
        PID=`echo $PROC | head -$I | tail -1 | awk '{print $1}'`
        echo "PROCESS: " $NAME " PID: " $PID
        echo "============================="
        if [ "$NAME" == "$1" ]
        then
            cat /proc/$PID/maps
        fi
        I=$(($I + 1))
    done
}

function countdsm()
{
    if [ "$1" != "" ]
    then
        objdump -d $1 | awk '{print $6}' > ${TMP_DSM}
        if [ "$2" != "" ]
        then
            count.py ${TMP_DSM} > $2
        else 
            count.py ${TMP_DSM} | more
        fi
        rm -f ${TMP_DSM}
    fi
}

function applynewpath()
{
    find . -type f | \
    while read i; 
    do 
        grep "/.../torolab.ibm.com/fs/projects/vabld" $i >/dev/null && \
        sed "s/\/\.\.\.\/torolab\.ibm\.com\/fs\/projects\/vabld/\/gsa\/tlbgsa\/projects\/x\/xlcmpbld/g" $i >$i.tmp && \
        mv $i.tmp $i && \
        echo "$i is replaced"; 
    done
}

function sstr()
{
    find . -name "*.[c|h]*" -exec grep "$*" "{}" -nH \;
}

function sstri()
{
    find . -name "*.[c|h]*" -exec grep "$*" "{}" -niH \;
}

function remake()
{
    make clean && make
}

function gentags()
{
    find . -name "*.ch" -o -name "*.[chC]" -o -name "*.cc" > cscope.files

    cscope -bkq -i cscope.files

    ctags -R
}

function resource()
{
    source ${THIS_FILE}
}

function editsource()
{
    vim ${THIS_FILE}
    resource
}

alias checkout=${CHECK_OUT_SH}

