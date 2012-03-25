#! /bin/sh

getConfigure() {
    key=$1
    file=$2
    echo `awk -F ' *= *| ' '$1=='\"${key}\"'{print $2}' ${file}`
}

#program=`getConfigure "APIPOOL_PROGRAM_NAME" "./apipoold.conf.sh"`
