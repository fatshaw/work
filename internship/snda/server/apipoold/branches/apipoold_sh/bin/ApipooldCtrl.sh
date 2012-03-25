#! /bin/sh

source ../tool/config_loader.sh

install_conf_file="./install.conf"


program_name=`getConfigure "PROGRAM_NAME" ${install_conf_file}`
install_name=`getConfigure "INSTALL_NAME" ${install_conf_file}`
apipool_conf_file=`getConfigure "CONF_FILE" ${install_conf_file}`

admin_ip=`getConfigure "admin_ip" ${apipool_conf_file}`
admin_port=`getConfigure "admin_port" ${apipool_conf_file}`

echo "program name: "${program_name}
echo "install name: "${install_name}
echo "apipool conf file: "${apipool_conf_file}
echo "admin ip: "${admin_ip}
echo "admin port: "${admin_port}

if [ -z ${program_name} ]; then
    echo "program name is empty!"
    exit
fi

if [ -z ${install_name} ]; then
    echo "install name is empty!"
    exit
fi

if [ -z ${apipool_conf_file} ]; then
    echo "apipool conf file is empty!"
    exit
fi

do_kill() {
        echo shutdown > /dev/tcp/${admin_ip}/${admin_port}
        sleep 2
        killall -9 ${install_name} >/dev/null 2>&1
}
do_stop() {
        do_kill
}
do_start() {
        tempdate=`date +%Y%m%d'-'%T`
#        cp -a ../log/*log ../log/logbak
    	cp ./${install_name} ./${install_name}.$tempdate
        \cp -f ${program_name} ${install_name}
        ./${install_name} ./${apipool_conf_file} &
}

do_loadcfg() {
        killall -USER1 ${install_name} >/dev/null 2>&1
}

case "$1" in
        'start')
                do_start
                ;;

        'restart')
		rm -rf ../log/*.log
                do_kill
                do_start
                ;;

        'stop')
                do_stop
                ;;

        'kill')
                do_kill
                ;;

				'loadcfg')
					do_loadcfg
					;;

        *)
                echo -n "usage :"$0 " start | stop | restart | loadcfg "
                echo
esac

