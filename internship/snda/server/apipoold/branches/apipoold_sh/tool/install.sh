#! /bin/sh

source ./config_loader.sh

install_conf_file="./install.conf"

install_file_list='
accessloader 
apipoold 
apipoold.conf 
ApipooldCtrl.sh 
config_loader.pl 
config_loader.sh 
curst.pl 
install.conf 
install.sh 
logclean.sh 
logicWatch.sh 
process_check.pl 
processWatch.sh 
rpcreport.php 
send_sms.php 
Statistic.inc.php 
visit_ctl.conf 
VisitCtrlDB.pm 
visit_ctrl_db_loader.pl 
visit_ctrl_db_writer.pl 
visit_ctrl_db_init.pl 
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

    install_log_path=${install_path}/../log
    if ! test -d ${install_log_path}; then
        mkdir ${install_log_path}
    fi

    if ! test -d ${install_log_path}; then
        echo "create ${instal_log_path} failed\n"
        exit
    fi
    for install_file in ${install_file_list};
    do 
        echo "copy file: " ${install_file}
        \cp -i ./${install_file} ${install_path}/${install_file}
    done
}

do_pack () {
    tar -zcvf ${install_path}/bin.tar.gz ${install_file_list}
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


