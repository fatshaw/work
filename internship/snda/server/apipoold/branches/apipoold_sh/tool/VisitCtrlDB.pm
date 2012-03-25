#!/usr/bain/perl

package VisitCtrlDB;

require Exporter;
@ISA = qw(Exporter);
@EXPORT = qw(
new 
isConnected 
loadVersion 
writeConfigToFile 
updateRequestSrcIpTable 
updateRequestSrcServiceRateTable
);

use DBI;

my $tb_request_src_ip = "request_src_ip_ctrl";
my $tb_request_src_service_rate = "request_src_service_rate_ctrl";
my $tb_request_src_service_api = "request_src_service_api_ctrl";
my $tb_params = "ctrl_params";
my $tb_version = "version_ctrl";
my $tb_loginfo = "loginfo";

my @tb_list = (
$tb_request_src_ip, 
$tb_request_src_service_rate, 
$tb_request_src_service_api, 
$tb_params, 
$tb_version, 
$tb_loginfo,
);

my %tb_creations = (
$tb_request_src_ip => "create table `".$tb_request_src_ip."` (
  `request_src` varchar(128) not null,
  `ip` varchar(15) not null,
  primary key  (`request_src`,`ip`)
)",
$tb_request_src_service_rate => "create table `".$tb_request_src_service_rate."` (
  `request_src` varchar(128) not null,
  `service` varchar(128) not null,
  `amount` int(11) default '0',
  `freq` int(11) default '0',
  primary key  (`request_src`,`service`)
)",
$tb_request_src_service_api => "create table `".$tb_request_src_service_api."` (
  `request_src` varchar(128) character set latin1 collate latin1_bin not null default '',
  `service` varchar(128) character set latin1 collate latin1_bin not null default '',
  `api` varchar(128) character set latin1 collate latin1_bin not null default '',
  primary key  (`request_src`,`service`,`api`)
)",
$tb_params => "create table `".$tb_params."` (
  `name` varchar(128) not null,
  `value` varchar(128) default null,
  primary key  (`name`)
)",
$tb_version => "create table `".$tb_version."` (
  `host` varchar(15) not null,
  `version` int(11) default '0',
  primary key  (`host`)
)",
$tb_loginfo => "create table `".$tb_loginfo."` (
  `info` varchar(1024) not null,
  `rt` timestamp not null default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP
)",
);

sub new 
{
    my $type = shift;
    my $this = {};
    my %params = @_;
    my $server = $params{'server'};
    my $port = $params{'port'};
    my $database = $params{'database'};
    if (! defined $database) {
        $database = "apipool_access_ctrl";
    }
    my $connector = "dbi:mysql:database=$database:host=$server";
    my $user = $params{'user'};
    my $pwd = $params{'pwd'};
    $this->{'type'} = $type;
    $this->{'server'} = $server;
    $this->{'port'} = $port;
    $this->{'database'} = $database;
    $this->{'connector'} = $connector;
    $this->{'user'} = $user;
    $this->{'pwd'} = $pwd;
    $this->{'db'} = DBI->connect($connector, $user, $pwd) or die $DBI::errstr;
    bless $this, $type;
    return $this;
}

sub isConnected 
{
    my ($this, $params) = @_;
    my $db = $this->{'db'};
    return (defined $db);
}

sub dropDatabase
{
    my ($db, $name) = @_;

    my $sql = "drop Database if exists $name";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub createDatabase
{
    my ($db, $name) = @_;

    my $sql = "create Database if not exists $name";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub useDatabase
{
    my ($db, $name) = @_;

    my $sql = "use $name";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub dropTable
{
    my ($db, $name) = @_;

    my $sql = "drop table if exists $name";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub createTable
{
    my ($db, $name) = @_;

    my $sql = $tb_creations{$name};
    print $sql."\n";
    if (! $sql) {
        return;
    }

    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub initDatabaseData 
{
    my ($db) = @_;

    my $sql = "replace into $tb_params(name, value) values('period_for_amount', '86400')";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
    
    my $sql = "replace into $tb_params(name, value) values('period_for_freq', '60')";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;

    my $sql = "replace into $tb_version(host, version) values('local', 0)";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub initDatabase
{
    my %params = @_;
    my $server = $params{'server'};
    my $port = $params{'port'};
    my $user = $params{'user'};
    my $pwd = $params{'pwd'};
    my $database = $params{'database'};
    my $connector = "dbi:mysql:host=$server";
    
    if (! defined $database) {
        print "no database !\n";
        return;
    }

    my $db = DBI->connect($connector, $user, $pwd) or die $DBI::errstr;

    &dropDatabase($db, $database);
    &createDatabase($db, $database);
    &useDatabase($db, $database);

    for $tb (@tb_list) {
        &dropTable($db, $tb);
        &createTable($db, $tb);
    }

    &initDatabaseData($db);

    $db->disconnect();
}

sub log
{
    my ($this, $info) = @_;
    my $db = $this->{'db'};

    my $sql = "insert into $tb_loginfo values('$info', null)";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub loadVersion 
{
    my ($this, $host) = @_;
    my $db = $this->{'db'};

    #get db version
    my $sql = "select version from $tb_version where host='$host'";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    my $version = $cursor->fetchrow_array;
    $cursor->finish;
    return $version;
}

sub updateVersion
{
    VisitCtrlDB::updateVersionTable(@_);
}

sub updateRequestSrcIpTable
{
    my ($this, $request_src, $ip) = @_;
    my $db = $this->{'db'};

    my $sql = "replace into $tb_request_src_ip(request_src, ip) values('$request_src', '$ip')";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub updateRequestSrcServiceRateTable
{
    my ($this, $request_src, $service, $amount, $freq) = @_;
    my $db = $this->{'db'};

    my $sql = "replace into $tb_request_src_service_rate(request_src, service, amount, freq) values('$request_src', '$service', $amount, $freq)";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub updateRequestSrcServiceApiTable
{
    my ($this, $request_src, $service, $api) = @_;
    my $db = $this->{'db'};

    my $sql = "replace into $tb_request_src_service_api(request_src, service, api) values('$request_src', '$service', '$api')";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub updateVersionTable
{
    my ($this, $host, $version) = @_;
    my $db = $this->{'db'};

    #get db version
    my $sql = "replace into $tb_version(host, version) values('$host', $version)";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub updateParamsTable
{
    my ($this, $name, $value) = @_;
    my $db = $this->{'db'};

    my $sql = "replace into $tb_params(name, value) values('$name', '$value')";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub clearTable
{
    my ($this, $name) = @_;
    my $db = $this->{'db'};

    my $sql = "delete from $name";
    print $sql."\n";
    my $cursor = $db->prepare($sql);
    $cursor->execute();
    $cursor->finish;
}

sub writeConfigToFile
{
    my ($this, $file) = @_;
    my $db = $this->{'db'};

    #open tmp config file
    open(CONFIGFILE, ">$file");

    #load version
    syswrite(CONFIGFILE, "#version = ");
    my $db_version = $this->loadVersion("local");
    syswrite(CONFIGFILE, $db_version);
    syswrite(CONFIGFILE, "\n");

    #title 
    syswrite(CONFIGFILE, "[visit_ctl_config]\n");

    #load request_src_ip
    syswrite(CONFIGFILE, "\n");
    syswrite(CONFIGFILE, "\n");
    syswrite(CONFIGFILE, "visitor_ip_map = (\n");
    my $query_req_ip = "select * from $tb_request_src_ip";
    my $req_ip_cursor = $db->prepare($query_req_ip);
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
    my $req_rate_cursor = $db->prepare($query_req_rate);
    $req_rate_cursor->execute();
    while (@req_rate_row = $req_rate_cursor->fetchrow_array) {
        my $req_src = $req_rate_row[0];
        my $service = $req_rate_row[1];
        my $amount = $req_rate_row[2];
        my $freq = $req_rate_row[3];
        syswrite(CONFIGFILE, "($req_src $service $amount $freq ");
        my $query_req_api = "select * from $tb_request_src_service_api where request_src='$req_src' and service='$service'";
        my $req_api_cursor = $db->prepare($query_req_api);
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
    my $period_amount_cursor = $db->prepare($query_period_amount);
    $period_amount_cursor->execute();
    @period_amount_row = $period_amount_cursor->fetchrow_array;
    syswrite(CONFIGFILE, "period_for_amount = $period_amount_row[1]\n");
    $period_amount_cursor->finish;

    #load period_for freq
    syswrite(CONFIGFILE, "\n");
    my $query_period_freq = "select * from $tb_params where name='period_for_freq'";
    my $period_freq_cursor = $db->prepare($query_period_freq);
    $period_freq_cursor->execute();
    @period_freq_row = $period_freq_cursor->fetchrow_array;
    syswrite(CONFIGFILE, "period_for_freq = $period_freq_row[1]\n");
    $period_freq_cursor->finish;

    close(CONFIGFILE);
}

sub loadConfigFromFile
{
    my ($this, $file, $overwrite) = @_;
    my $db = $this->{'db'};
    
    if ($overwrite) {
        $this->clearTable($tb_request_src_ip);
        $this->clearTable($tb_request_src_service_rate);
        $this->clearTable($tb_request_src_service_api);
    }

    open(CONFIGFILE, "$file");

    $block = 0;
    while ($line = <CONFIGFILE>) {
        my @words = split(/\s+/, $line);
        my $word_num = scalar(@words);
        if ($word_num == 0) {
            $block = $block + 1;
        }
        if ($block == 0 and $word_num == 2) {
            $this->updateRequestSrcIpTable($words[0], $words[1]);
        }  
        if ($block == 1 and $word_num == 4) {
            $this->updateRequestSrcServiceRateTable($words[0], $words[1], $words[2], $words[3]);
        }   
        if ($block == 2 and $word_num == 3) {
            $this->updateRequestSrcServiceApiTable($words[0], $words[1], $words[2]);
        }   
    }
    
    close(CONFIGFILE);
}

1;

