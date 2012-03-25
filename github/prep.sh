find "$1" -type f -size -1024k> a;
sed '/svn/d' a > b;
sh git.sh "$1" < b;
rm -f a b
