#!/usr/bin/perl

use strict;
use FileHandle;
use IPC::SysV qw(IPC_PRIVATE IPC_RMID S_IRWXU IPC_CREAT );
use LWP::UserAgent;
use DBI;
use Switch;

`source /etc/profile`;

#----------Time Begin---------------------
my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime();
$mon=$mon+1;
$year=1900+$year;
#my $timestr = "$year-$mon-$mday $hour:$min";
my $timestr = "$hour:$min:$sec";
print $timestr."\n";
#----------Time Finish---------------------

#my $path = '/home/caihong/flashd/bin';
#my $ip = $ARGV[0];

my $mobile = $ARGV[0];
my $warnstr =  $ARGV[1];


print "mobile is [".$mobile."]\n";
print "warn info is [".$warnstr."]\n";

my $ip = get_ip();
chomp($ip);

alert_sms($warnstr,$mobile);


sub alert_sms(){
	my $alert_msg=$_[0];
	my $amsg=$timestr."-".$ip." ".$alert_msg;
	#my $amsg=$ip."-".$alert_msg;
	my $mobile = $_[1];
	#my $send_url="http://alarm.51.com/smsAlert/smsAlert.php?user=zhuxy";
	my $send_url="http://api.monitor.sdo.com/sendsms.php?username=fanxing";
	my $url = $send_url."&message=".$amsg."&phone=".$mobile;
	my $res = LWP::UserAgent->new->request(new HTTP::Request GET=>$url);
	print $res->content;
	#print $url
	print $amsg." Send Sms\n";
}



sub get_ip(){
   #my $ipstr = `ifconfig  | grep 'inet addr:'| grep '192.168' | cut -d: -f2 | awk '{ print \$1}' |awk -F'.' '{print \$3"."\$4}'  | head -n 1`;
   my $ipstr = `/sbin/ifconfig  | grep 'inet addr:'| grep '10.10' | cut -d: -f2 | awk '{ print \$1}'|awk -F'.' '{print \$3"."\$4}'`;
print "ip is ".$ipstr;
   return $ipstr;
}

