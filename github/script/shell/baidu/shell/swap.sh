set -x
for((;;))
do
	ind=`grep loc 353_20120411143124685.xml.1 | tail -n 1 | awk -F_ '{print $2+1}'`
	sed -i "s/\(.*bbbb_\)\([0-9]*\)\(.*\)/\1$ind\3/g" 353_20120411143124685.xml.1
	#/bin/cp -f 353_20120411143124685.xml.1 353_20120411143124685.xml
	ln -s -f 353_20120411143124685.xml.1 353_20120411143124685.xml
	sleep 15
	#/bin/cp -f 353_20120411143124685.xml.2 353_20120411143124685.xml
	ln -s -f 353_20120411143124685.xml.2 353_20120411143124685.xml
	sleep 15
done
set +x
