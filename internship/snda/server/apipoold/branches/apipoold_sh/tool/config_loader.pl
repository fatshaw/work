#! /usr/bin/perl

sub getConfigure {
    my $conf_name = $_[0];
    my $file_name = $_[1];

    my $command = "awk -F ' *= *| ' '\$1==\"$conf_name\"{print \$2}' $file_name";
    my $value = `$command`;
    chomp($value);
    return $value;
}

1;

