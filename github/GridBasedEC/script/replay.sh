#!/bin/sh
l=1
if [ $# -eq 2 ];then
	l=$2	
fi
case "$1" in
	bt)cd bt;sh bt.sh $l;cd -;;
	ed)cd ed;sh ed.sh $l;cd -;;
	pptv)cd pptv;sh pptv.sh $l;cd -;;
	maze)cd maze;sh maze.sh $l;cd -;;
	skype)cd skype;sh skype.sh $l;cd -;;
	http)cd http;sh http.sh $l;cd -;;
	sdbot)cd sdbot;sh sdbot.sh $l;cd -;;
	peacomm)cd peacomm;sh peacomm.sh $l;cd -;;
	all) 
		cd bt;sh bt.sh $l;cd -;
		cd ed;sh ed.sh $l;cd -;
		cd pptv;sh pptv.sh $l;cd -;
        	cd maze;sh maze.sh $l;cd -;
	        cd skype;sh skype.sh $l;cd -;
		cd peacomm;sh peacomm.sh $l;cd -;
		cd http;sh http.sh $l;cd -;
	;;
esac
