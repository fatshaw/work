#find query product params
sed -n 's/.*QueryProductsParams\(.*\), sort.*/\1/gp' prodb_product_info.sql  > queryParam

#remove values of keys
#find keys
sed -n 's/\\"$v\\":\[[^{}]*\]//g;s/queryJSONConds=//gp' queryParam  > queryParamsKeys

#remove prefix [
#remove , after keys which before value.eg. removing , after "Product ID" in json string : "$k":"Product ID","$v":[1,2,3]
#replace \" with "
sed -n 's/ *\[\(.*\)/\1/g;s/\",/\"/g;s/\\//gp' queryParamsKeys > queryParamsKeysArranged

#sort params (not neccessary)
sort queryParamsKeysArranged > sortedQueryParamsKeysArranged

#counter keys 
awk -F= '{CMD[$1]++;count++;}END { for (a in CMD)printf "%s=%d\n",a,CMD[a]}' sortedQueryParamsKeysArranged > counterOfKeys 

#sort according to counter of key
sort -r -t= -k2 -n  counterOfKeys > sortedCounterOfKeys

