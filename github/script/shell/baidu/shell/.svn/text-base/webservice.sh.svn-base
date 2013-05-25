#!bin/sh
BEGIN{FS="="}
{
	date="date +%s";
	if($1~/messageID/){
		split($2,b,":");
		if(b[2] > date-30){
			if(a[b[1]]==1){
				a[b[1]]+=1;
			}
		}
	}
}
END{
	fail_request_times=0;
	for(i in a){
		if(a[i]!=2 && a[i]!=0){
			printf("messgeID=%s show times = %s\n",i,a[i]);
			fail_request_times+=1;
		}
	}
	printf("fail_request_times=%d\n",fail_request_times);
}
