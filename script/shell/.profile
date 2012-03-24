alias vi='vim'
alias rm='mv -i -t $HOME/trash/ $1'
alias ec='echo $1'
alias screen='screen -L'

source ~/script/tools

HOME=/home/fatshaw
ME=`whoami`
HOST=`hostname | cut -d. -f1`
export PS1='[`whoami`@\H $PWD]\$ '
export ME HOST PS1
export LS_COLORS='di=1;36:ln=31:pi=5:so=5:bd=5:cd=5:or=31:mi=0:ex=32:*.rpm=90:*.tar=01;31:*.tgz=01;31:*.gz=01;31:*.zip=01;31:*.sh=32:*.c=32:*.cpp=32:*.java=32:*.php=32'
export PATH=$PATH:$HOME/install/global/bin:$HOME/install/apache-ant-1.8.2/bin
export CLASSPATH=.:$HOME/java_lib/junit-4.10.jar
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib/boost/lib


