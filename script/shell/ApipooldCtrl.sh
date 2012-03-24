#! /bin/sh

ip_name='127.0.0.1'
do_kill() {
        echo shutdown > /dev/tcp/${ip_name}/8014
        sleep 2
        killall -9 apipoold >/dev/null 2>&1
}
do_stop() {
        do_kill
}
do_start() {
        tempdate=`date +%Y%m%d'-'%T`
#        cp -a ../log/*log ../log/logbak
    	cp ./apipoold ./apipoold.$tempdate
       ./apipoold ./apipoold.conf &
}

do_loadcfg() {
        killall -USER1 apipoold >/dev/null 2>&1
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

