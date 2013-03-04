#!perl

use warnings;
use strict;

my $command = "";
my $compiler = "htlatex";
my $change_dir  = "chdir";

$command = "$compiler index";
print "$command\n";
system($command);

chdir("doc");

$command = "$compiler segmenter";
print "$command\n";
system($command);

$command = "$compiler conparser";
print "$command\n";
system($command);
