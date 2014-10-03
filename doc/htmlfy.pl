#!/usr/bin/perl -CADSL

use strict;
use warnings;
use utf8;

use v5.14;

$_=<>;
if ( not /^#!(.+)$/ ) {
  die "Ill formed input!";
}

say "<!DOCTYPE html>";
say "<html>";
say "<head>";
say '<meta content="text/html; charset=UTF-8" http-equiv="Content-Type">';
say '<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/bootstrap/3.0.3/css/bootstrap.min.css">';
say '<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/bootstrap/3.0.3/css/bootstrap-responsive.min.css">';
say '
<style type="text/css">
.nav { }
.nav li { float: left; width: 110px; }
.container { background-color: #ffffff; padding: 30px; }
.content { padding: 30px; }
body, h1, h2, h3, h4 { font-family: "Trebuchet MS","Helvetica Neue",Arial,Helvetica,sans-serif,"Georgia";}
body { background-color: #eeeeee; }
header { width: 100%; }
blockquote p { font-size: 14px; }
</style>
';
say "<title>ZPar | $1</title>";
say "</head>";

say "<body>";
say '<header>';
say '<div class="page-header text-center">';
say "<h1>$1</h1>";
say '</div>';
say '</header>';

say '<div class="table-bordered container">';
say '<div class="content">';

while (<>) {
  print;
}

say '</div>';
say '</div>';
say "<footer class=\"text-center\">";
say "<p>";
say "ZPar Release 0.7";
say "</p>";
say "</footer>";
say "</body>";
say "</html>";
