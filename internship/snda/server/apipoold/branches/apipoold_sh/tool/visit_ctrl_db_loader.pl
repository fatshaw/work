#!/usr/bin/perl

push(@INC, `pwd`);

use VisitCtrlDB;

use Net::Telnet();
#use Time::HiRes qw(gettimeofday);
#use DateTime;

require("./config_loader.pl");

my $install_conf_file = "../bin/install.conf";

my $db_server = &getConfigure("ACCESS_DB_SERVER", $install_conf_file);
my $db_database = &getConfigure("ACCESS_DB_DATABASE", $install_conf_file);
my $db_port = &getConfigure("ACCESS_DB_PORT", $install_conf_file);
my $db_user = &getConfigure("ACCESS_DB_USER", $install_conf_file);
my $db_pwd = &getConfigure("ACCESS_DB_PWD", $install_conf_file);
my $db_update_interval = &getConfigure("ACCESS_DB_UPDATE_INTERVAL", $install_conf_file);

my $apipool_conf_file = &getConfigure("CONF_FILE", $install_conf_file);
$apipool_conf_file = "../bin/".$apipool_conf_file;

my $config_file = "../bin/visit_ctl.conf";

my $telnet_server = &getConfigure("admin_ip", $apipool_conf_file);
my $telnet_port = &getConfigure("admin_port", $apipool_conf_file);

my $access_db_ip = &getConfigure("ACCESS_DB_IP", $install_conf_file);

if ($access_db_ip eq "") {
    $access_db_ip = &GetLocalIp();
}

if ($db_update_interval eq "") {
    $db_update_interval = 60;
}

if ($db_database eq "") {
    print "no database\n";
    die;
}

print "db server: ".$db_server."\n";
print "db port: ".$db_port."\n";
print "db user: ".$db_user."\n";
print "db pwd: "."\n";#$db_pwd;
print "admin ip: ".$telnet_server."\n";
print "admin port: ".$telnet_port."\n";
print "access db ip: ".$access_db_ip."\n";
print "db update interval: ".$db_update_interval."\n";
#die;

sub GetLocalIp()
{
    open (IN, "/sbin/ifconfig |");
    while (<IN>) {
        if ( /^\s*inet addr:([\d\.]*)\s*Bcast:([\d\.]*)\s*Mask:([\d\.]*)/ ) {
            $host_ip = $1;
            last ;
        }
    }
    close(IN);
    $host_ip;
}

sub reload_conf
{
    my $telnet = new Net::Telnet(Timeout => 10, Prompt => '/[.\$#>]/', Errmode => "return", Port => $telnet_port);
    $telnet->max_buffer_length("10485760");

    my $telnet_ok = $telnet->open($telnet_server);

    unless($telnet_ok eq 1) {
        my $errmsg = $telnet->errmsg();
        $telnet->close;
        return;
    }

    #my @temp = $telnet->cmd(String => "PS1=\"@@@\"", Prompt => "/@@@/");
    #$telnet->buffer_empty;

    my @result = $telnet->cmd(String => "reload accessconf", Prompt => "/@@@/", Timeout => 1);
    $telnet->close;
}

sub diff_file()
{
    my $src_file = $_[0];
    my $dst_file = $_[1];

    `touch $dst_file`;
    my $diff = `diff $src_file $dst_file | grep -v -E "^#"`;
    return $diff ne "";
}

sub replace_file()
{
    my $src_file = $_[0];
    my $dst_file = $_[1];

    #$cp_result = `cp -f $src_file $dst_file`;
    my $cur_time = &GetFmtTime();
    `touch $dst_file`;
    `mv -f $dst_file $dst_file.$cur_time`;
    `mv -f $src_file $dst_file`;
}

sub GetFmtTime()
{
    my $date = `date +%Y%m%d-%T:%N`;
    return $date;
#    (my $start_sec,my $start_usec) = gettimeofday;
#    my $dt = DateTime->from_epoch( epoch => $start_sec );
#    $dt->ymd('')."-".$dt->hms.":".$start_usec;
}

my $config_file_tmp = $config_file.".tmp";

my $self_ip = $access_db_ip;

my $self_version = -1;

############################ check
while (1)
{
    if ($self_version == -1) {
    }

    #connect db
    my $db = new VisitCtrlDB('server'=>$db_server, 'port'=>$db_port, 'database'=>$db_database, 'user'=>$db_user, 'pwd'=>$db_pwd);
    print "connect db  \n";

    #db versiona
    my $db_version = $db->loadVersion("local");
    print "db version: ".$db_version."\n";

    #self version 
    print "self version: ".$self_version."\n";

    #compare version
    if ($db_version ne $self_version)
    {

        #load config 
        $db->writeConfigToFile($config_file_tmp);
        print "load config\n";

        #compare file
        if (&diff_file($config_file_tmp, $config_file)) {

            #replace config file
            &replace_file($config_file_tmp, $config_file);
            print "replace file\n";

            #reload conf
            &reload_conf();
            print "reload conf\n";

        }

        $self_version = $db_version;

        #update version
        $db->updateVersion($self_ip, $self_version);
        print "update version to db: ".$self_ip.":".$self_version."\n";
    }

    sleep($db_update_interval);
}

