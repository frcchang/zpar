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

$command = "$compiler segmentor";
print "$command\n";
system($command);

$command = "$compiler conparser";
print "$command\n";
system($command);

$command = "$compiler deppar";
print "$command\n";
system($command);

$command = "$compiler joint_seg_tag";
print "$command\n";
system($command);

$command = "$compiler eng_tagger";
print "$command\n";
system($command);

$command = "$compiler independent";
print "$command\n";
system($command);

$command = "$compiler ccg";
print "$command\n";
system($command);

$command = "$compiler tweb_tagger";
print "$command\n";
system($command);

$command = "$compiler build_system";
print "$command\n";
system($command);
