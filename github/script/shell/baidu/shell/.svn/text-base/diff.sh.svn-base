set -x
version="$1"
cd ~/pic-core/conf;
svn up;
cd $version;
mkdir -p ~/tmp/onlineconf/cq01_new;
mkdir -p ~/tmp/onlineconf/m1_new;
cp -fr cq01-prodb-core00.cq01/* ~/tmp/onlineconf/cq01_new/
cp -fr m1-prodb-core00.m1/* ~/tmp/onlineconf/m1_new/ 
cd ~/tmp/onlineconf/cq01_new/;
find . -name ".svn" -exec rm -fr {} \;
cd ~/tmp/onlineconf/m1_new/;
find . -name ".svn" -exec rm -fr {} \;
cd ..;
source ~/script/shell/tools.sh
scpcq01;
scpm1;
cd ..;
diff -urN cq01 cq01_new > cq01.diff;
diff -urN m1 m1_new > m1.diff;
set +x
