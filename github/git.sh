while read i
do
	if [ -f "$i" ];then
		git add "$i"
	fi
done
git commit -m "$1"
git push -u origin master

