set -x
>abc;
while read i
do
	for merchant in `cat merchant.txt`
	do
		str=$(echo $i | sed "s/\`[a-zA-Z_0-9]*\(_product[_a-zA-Z]*\)/\`$merchant\1/g")
		echo $str >> abc;
	done
done
set +x
