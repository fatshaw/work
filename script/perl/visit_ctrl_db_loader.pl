#!/usr/bin/perl

use DBI;
use Net::Telnet();
#use Time::HiRes qw(gettimeofday);
#use DateTime;

my $db_server = "10.241.12.117";
my $db_user = "root";
my $db_pwd = "123qwe";
my $db_apipool = "apipool_access_ctrl";
my $db_connector = "dbi:mysql:database=$db_apipool;host=$db_server";

my $tb_request_src_ip = "request_src_ip_ctrl";
my $tb_request_src_service_rate = "request_src_service_rate_ctrl";
my $tb_request_src_service_api = "request_src_service_api_ctrl";
my $tb_params = "ctrl_params";
my $tb_version = "version_ctrl";
my $config_file = "./visit_ctl.conf";

my $telnet_server = "127.0.0.1";
my $telnet_port = "8014";

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

sub load_version_from_db
{
    my $conf_db = $_[0];
    my $conf_host = $_[1];

    #get db version
    my $query_db_version = "select version from $tb_version where host='$conf_host'";
    my $ver_cursor = $conf_db->prepare($query_db_version);
    $ver_cursor->execute();
    my $conf_db_version = $ver_cursor->fetchrow_array;
    $ver_cursor->finish;
    return $conf_db_version;
}

sub update_version_to_db
{
    my $conf_db = $_[0];
    my $conf_host = $_[1];
    my $conf_version = $_[2];

    #get db version
    my $update_version = "update $tb_version set version='$conf_version' where host='$conf_host'";
    my $update_version_cursor = $conf_db->prepare($update_version);
    $update_version_cursor->execute();
    $update_version_cursor->finish;
}

sub load_config_from_db
{
    my $conf_db = $_[0];
    my $conf_file = $_[1];

    #open tmp config file
    open(CONFIGFILE, ">$conf_file");

    #load version
    syswrite(CONFIGFILE, "#version = ");
    my $db_version = &load_version_from_db($conf_db, "local");
    syswrite(CONFIGFILE, $db_version);
    syswrite(CONFIGFILE, "\n");

    #title 
    syswrite(CONFIGFILE, "[visit_ctl_config]\n");

    #load request_src_ip
    syswrite(CONFIGFILE, "\n");
    syswrite(CONFIGFILE, "\n");
    syswrite(CONFIGFILE, "visitor_ip_map = (\n");
    my $query_req_ip = "select * from $tb_request_src_ip";
    my $req_ip_cursor = $conf_db->prepare($query_req_ip);
    $req_ip_cursor->execute();
    while (@req_ip_row = $req_ip_cursor->fetchrow_array) {
        syswrite(CONFIGFILE, "($req_ip_row[0] $req_ip_row[1])\n");
    }
    $req_ip_cursor->finish;
    syswrite(CONFIGFILE, ")\n");

    #load request_src_service_rate and request_src_service_api
    syswrite(CONFIGFILE, "\n");
    syswrite(CONFIGFILE, "\n");
    syswrite(CONFIGFILE, "visitor_dstservice_allow_info_map = (\n");
    my $query_req_rate = "select * from $tb_request_src_service_rate";
    my $req_rate_cursor = $conf_db->prepare($query_req_rate);
    $req_rate_cursor->execute();
    while (@req_rate_row = $req_rate_cursor->fetchrow_array) {
        my $req_src = $req_rate_row[0];
        my $service = $req_rate_row[1];
        my $amount = $req_rate_row[2];
        my $freq = $req_rate_row[3];
        syswrite(CONFIGFILE, "($req_src $service $amount $freq ");
        my $query_req_api = "select * from $tb_request_src_service_api where request_src='$req_src' and service='$service'";
        my $req_api_cursor = $conf_db->prepare($query_req_api);
        $req_api_cursor->execute();
        my @api_names = ();
        while (@req_api_row = $req_api_cursor->fetchrow_array) {
            my $api_name = $req_api_row[2];
            push(@api_names, $api_name);
        }
        $req_api_cursor->finish;
        my $join_api_names = join(",", @api_names);
        syswrite(CONFIGFILE, $join_api_names);
        syswrite(CONFIGFILE, ")\n");
    }
    $req_rate_cursor->finish;
    syswrite(CONFIGFILE, ")\n");

    #load period_for_amount
    syswrite(CONFIGFILE, "\n");
    syswrite(CONFIGFILE, "\n");
    my $query_period_amount = "select * from $tb_params where name='period_for_amount'";
    my $period_amount_cursor = $conf_db->prepare($query_period_amount);
    $period_amount_cursor->execute();
    @period_amount_row = $period_amount_cursor->fetchrow_array;
    syswrite(CONFIGFILE, "period_for_amount = $period_amount_row[1]\n");
    $period_amount_cursor->finish;

    #load period_for freq
    syswrite(CONFIGFILE, "\n");
    my $query_period_freq = "select * from $tb_params where name='period_for_freq'";
    my $period_freq_cursor = $conf_db->prepare($query_period_freq);
    $period_freq_cursor->execute();
    @period_freq_row = $period_freq_cursor->fetchrow_array;
    syswrite(CONFIGFILE, "period_for_freq = $period_freq_row[1]\n");
    $period_freq_cursor->finish;

    close(CONFIGFILE);
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

my $self_ip = &GetLocalIp();

my $self_version = -1;

############################ check
while (1)
{
    if ($self_version == -1) {
    }

    #connect db
    my $db = DBI->connect("$db_connector", "$db_user", "$db_pwd");

    #db version
    my $db_version = &load_version_from_db($db, "local");

    #self version 

    #compare version
    if ($db_version ne $self_version)
    {

        #load config 
        &load_config_from_db($db, $config_file_tmp);

        #compare file
        if (&diff_file($config_file_tmp, $config_file)) {

            #replace config file
            &replace_file($config_file_tmp, $config_file);

            #reload conf
            reload_conf();

        }

        $self_version = $db_version;

        #update version
        &update_version_to_db($db, $self_ip, $self_version);
    }

    $db->disconnect();
    sleep(1);
}
