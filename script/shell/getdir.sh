>direc
for name in `ls`
do
	if [ -d $name ];then
		echo "$name" >> direc
	fi
done
