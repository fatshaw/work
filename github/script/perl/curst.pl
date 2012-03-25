#!/usr/bin/perl

if ( !&IsRun() )
{
    my $scriptpath = &ExtractPath($ARGV[0]);
    system "cd $scriptpath;".$ARGV[0]." > /dev/null &";
}

sub IsRun()
{
    my $script_name = &ExtractFileName ($ARGV[0]);
    my @rows = `ps -ef|grep $script_name|grep -v grep`;
    $counts = @rows;
    if(1==$counts)
    {
        0;
    }
    else
    {
        1;
    }
}

sub ExtractFileName()
{
    my @seg = split('\/',$_[0]);
    my $file_name=pop(@seg);
}

sub ExtractPath()
{
    my @seg = split('\/',$_[0]);
    pop(@seg);
    my $path = join("/",@seg);
}

