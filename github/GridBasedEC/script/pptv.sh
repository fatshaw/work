l=1
if [ $# -gt 0 ];then
	l=$1
fi

for i in `ls *.pcap`
do
	tcpreplay --topspeed -l $l -i eth0 $i >/dev/null 2>&1 &
done
