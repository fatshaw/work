#! /bin/sh

do_kill() {
        killall -9 phpfcgiagentd >/dev/null 2>&1
}
do_stop() {
        do_kill
}
do_start() {
        tempdate=`date +%Y%m%d'-'%T`
#        rm ../log/* -rf
#	cp ./phpfcgiagentd ./phpfcgiagentd.$tempdate
       ./phpfcgiagentd ./phpfcgiagentd.cfg &
}

do_loadcfg() {
        killall -USER1 phpfcgiagentd >/dev/null 2>&1
}

case "$1" in
        'start')
                do_start
                ;;

        'restart')
                do_kill
		rm -rf ../log/*.log
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

