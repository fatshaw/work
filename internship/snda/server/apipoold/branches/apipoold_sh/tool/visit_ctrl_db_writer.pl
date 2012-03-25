#!/usr/bin/perl

push(@INC, `pwd`);

use VisitCtrlDB;

require("./config_loader.pl");

my $install_conf_file = "../bin/install.conf";

my $db_server = &getConfigure("ACCESS_DB_SERVER", $install_conf_file);
my $db_database = &getConfigure("ACCESS_DB_DATABASE", $install_conf_file);
my $db_port = &getConfigure("ACCESS_DB_PORT", $install_conf_file);
my $db_user = &getConfigure("ACCESS_DB_USER", $install_conf_file);
my $db_pwd = &getConfigure("ACCESS_DB_PWD", $install_conf_file);

my $overwrite = $ARGV[0];
chomp($overwrite);
my $visit_conf_file = $ARGV[1];
chomp($visit_conf_file);
if ($overwrite ne "-o") {
    $visit_conf_file = $ARGV[0];
    chomp($visit_conf_file);
    $overwrite = "";
}

print "db server: ".$db_server."\n";
print "db database: ".$db_database."\n";
print "db port: ".$db_port."\n";
print "db user: ".$db_user."\n";
print "db pwd: "."\n";#$db_pwd;
print "overwrite: ".$overwrite."\n";
print "visit conf: ".$visit_conf_file."\n";
#die;

if ($visit_conf_file eq "") {
    print "conf file empty\n";
    die;
}

if ($db_database eq "") {
    print "no database\n";
    die;
}

my $db = new VisitCtrlDB('server'=>$db_server, 'port'=>$db_port, 'database'=>$db_database, 'user'=>$db_user, 'pwd'=>$db_pwd);

$db->log("write $visit_conf_file to db");

$db->loadConfigFromFile($visit_conf_file, $overwrite eq "-o");

my $version = $db->loadVersion("local");
my $version = $db->updateVersion("local", $version + 1);

