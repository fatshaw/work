function proc
{
	dir=.
	for it in `ls "$dir"`
	do
		if [ -d $it ];
		then
			cd $it
			proc
			cd -
		fi
		local=`echo $it | sed -n 's/.*\.[h|cpp]\+$/a/gp'`
		if [ -n "$local" ];
		then
			astyle $it;
		fi
	done
}

proc

