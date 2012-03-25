#! /bin/sh

install_file_list='
web/ApiPoolAccessCtrl.php
web/error.php
web/index.php
web/modify.php
'

install_path=$2

do_install () {

    if ! test -d ${install_path}; then
         mkdir ${install_path}
    fi

    if ! test -d ${install_path}; then
        echo "create ${instal_path} failed\n"
        exit
    fi

    for install_file in ${install_file_list};
    do 
        echo "copy file: " ${install_file}
        \cp -i ./${install_file} ${install_path}/${install_file}
    done
}

do_pack () {
    tar -zcvf ${install_path}/admin.tar.gz ${install_file_list}
}

case "$1" in
    'install')
        do_install
        ;;

    'pack')
        do_pack
        ;;
    *)
        echo -n "usage :"$0 "install | pack "
        echo
esac


