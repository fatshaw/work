#!/usr/bin/perl -w

use strict;
#use Proc::ProcessTable;
#use LWP::UserAgent;
#use Shell::GetEnv;

my $ip;
my $progname = "apipoold";
my $prognameCtrl = "./ApipooldCtrl.sh";
my $workpath = "/opt/app/server/apipoold/bin";
my $sendsms="/opt/app/php5/bin/php ../tool/send_sms.php";
my $pidfile = "./pid";
chdir "$workpath" or die "cannot chdir to $workpath: $!!";
mkdir "./core";
my $LOG = "Monitor_Svr.log";
open(STDOUT, ">>$LOG") or die "Failed to re-open STDOUT to $LOG";
open(STDERR, ">>$LOG") or die "Failed to re-open STDERR to STDOUT";

sub getTime()
{
	#----------Time Begin---------------------
	my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime();
	$mon=$mon+1;
	$year=1900+$year;
	#my $timestr = "$year-$mon-$mday $hour:$min";
	my $timestr = "$hour:$min:$sec";
	#print $timestr."\n";
	return $timestr;
	#----------Time Finish---------------------
}

sub alert_sms{
#    my $mobile = "15000355399;13621681551";
        my $alert_msg=$_[0];
       	my $timestr = getTime();
        my $amsg=$timestr."-".$ip."-".$progname." ".$_[0];
        my $mobile = "13524515129;13671563320;13482027618";
        #my $send_url="http://api.monitor.sdo.com/sendsms.php?username=fanxing";
        #my $url = $send_url."&message=".$amsg."&phone=".$mobile;
        #my $res = LWP::UserAgent->new->request(new HTTP::Request GET=>$url);
        #print $res->content;
        system("$sendsms \"$mobile\" \"$amsg\"");
        #print $url
        print $amsg." Send Sms\n";
}

sub get_ip{
   my $ipstr = `ifconfig  | grep 'inet addr:'| grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print \$1}' | awk '{print \$1}'  | tail -1`;
   chomp $ipstr;
   return $ipstr;
}

$ip = get_ip();
print $ip."\n";
#sub alert_sms(){
#   my $alert_msg="[$ip][gamed] Found [".$_[0]."] Core File \n";
#   print $alert_msg."\n";
#   my $mobile = "13621681551";
#   my $send_url="http://alarm.51.com/smsAlert/smsAlert.php?user=zhuxy";
#   my $url = $send_url."&msg=".$alert_msg."&receiver=".$mobile;
#   my $res = LWP::UserAgent->new->request(new HTTP::Request GET=>$url);
#   print $res->content;
#   print $ip." Send Sms\n";
#}

print  "Start Checking.....\n";
sleep 2;
my $alive = 0;

while (1)
{
    printf "xxxxxxxxxxx";
    my $is_die = "false";
   my $pid=`ps -d |grep $progname | awk '{print \$1}'` or $is_die="true";
   my $pid_retry1=`ps -d |grep $progname | awk '{print \$1}'`;
   my $pid_retry2=`ps -d |grep $progname | awk '{print \$1}'`;
   chomp $pid;
   chomp $pid_retry1;
   chomp $pid_retry2;
   if(($is_die eq "true") && ($pid eq "") && ($pid_retry1 eq "") && ($pid_retry2 eq ""))
   {
   	 my $is_die = "false";
   	 my $pid = `cat < $pidfile`	;
   	 `ls /proc/$pid` or $is_die = "true";
   	 print "check proc dir";
   	 if($is_die eq "true"){
   	 		$alive= 0;
     		print "$progname die id:$pid $pid_retry1 $pid_retry2\n";	
   	 }
   	 else{
   	 		$alive = 1;
   	 		print "$progname alive \n";
   	 }
   }else{
   	 `echo $pid > $pidfile`;
     $alive= 1;
     print "$progname alive \n";
   }
   my $tempdate=`date +%Y%m%d'-'%T`;
   chomp $tempdate;
   if($alive == 0){
     print "[$tempdate] I Will Restart $progname\n";
     system("$prognameCtrl restart");
     my $restart_msg="server down, restart\n";
     alert_sms($restart_msg);
     sleep 5;
   }
   else
   {
     print "[$tempdate] process work ok $progname, id:$pid $pid_retry1 $pid_retry2\n";
   }

   #check core file and send sms
   my $coreNum = `ls core.* 2>/dev/null  | wc -l `;
   chomp $coreNum;
   
   if($coreNum == 0 ){
      print "Not Found Core Files\n";
   }else{
      print "Found [$coreNum] core files \n";
      my $core_msg="Found [".$coreNum."] Core File \n";
      alert_sms($core_msg);
      #alert_sms($coreNum);
      my $ret = `/bin/mv core.* core/`;
   }
   sleep 1;
}



close(STDOUT);
close(STDERR);
close($LOG);
