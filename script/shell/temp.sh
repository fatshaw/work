awk '{if($2~/mysql/)print $1}' tmp.log | awk -F, '{print substr($3,7)}'  | sort -u  | awk '{print $1}' > ip.log
awk '{if($2~/mysql/)print $4}' tmp.log | sort -u > interface.log
awk 'BEGIN{total = 0;}{time =  substr($3,index($3,"<")+1,index($3,">") - index($3,"<") - 1);total += time;}END{print total;}' tmp.log 
