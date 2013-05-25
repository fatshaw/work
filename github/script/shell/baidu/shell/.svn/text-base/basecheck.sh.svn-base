set -x
olddate=`date +%Y%m%d --date='2 days ago'`
olddate=$olddate"*_0_"
newdate=`date +%Y%m%d --date='1 days ago'`"*_";
today=`date +%Y%m%d`"*_"
speed=5000k
taskid=1
cd pa_1;
ftpaddr="/usr/bin/wget  --limit-rate=$speed -nc ftp://cq01-prodb-core00.cq01.baidu.com/home/work/dataio/pushfiles/pa_1/";
/bin/sh -c "$ftpaddr$olddate$taskid"
/bin/sh -c "$ftpaddr$newdate$taskid"
/bin/sh -c "$ftpaddr$today$taskid"
cd ../iad_1;
taskid=2
ftpaddr="/usr/bin/wget  --limit-rate=$speed -nc ftp://cq01-prodb-core00.cq01.baidu.com/home/work/dataio/pushfiles/iad_1/";
/bin/sh -c "$ftpaddr$olddate$taskid"
/bin/sh -c "$ftpaddr$newdate$taskid"
/bin/sh -c "$ftpaddr$today$taskid"
taskid=3
cd ../tc_web_1;
ftpaddr="/usr/bin/wget  --limit-rate=$speed -nc ftp://cq01-prodb-core00.cq01.baidu.com/home/work/dataio/pushfiles/tc_web_1/";
/bin/sh -c "$ftpaddr$olddate$taskid"
/bin/sh -c "$ftpaddr$newdate$taskid"
/bin/sh -c "$ftpaddr$today$taskid"

taskid=4
cd ../pa_search;
ftpaddr="/usr/bin/wget  --limit-rate=$speed -nc ftp://cq01-prodb-core00.cq01.baidu.com/home/work/dataio/pushfiles/pa_search/";
/bin/sh -c "$ftpaddr$olddate$taskid"
/bin/sh -c "$ftpaddr$newdate$taskid"
/bin/sh -c "$ftpaddr$today$taskid"
cd /home/disk1/cjxiao/pic-core_basecheck/output/;
sh basecheck.sh;
set +x
