ip=$1
port=$2
wget http://10.46.99.15:8745/services?wsdl -O wsdl_online
wget http://$1:$2/services?wsdl -O wsdl
diff -ur wsdl_online wsdl > wsdl.diff
