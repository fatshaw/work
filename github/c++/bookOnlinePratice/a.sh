while read i
do
	if [ -d "$i" ]; then
		rmdir $i
	else
		rm $i
	fi	
done
