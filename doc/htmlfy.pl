#!/usr/bin/perl -CADSL

use strict;
use warnings;
use utf8;

use v5.14;

my $RELEASEVER = '0.7';

$_=<>;
s/<RELEASEVER>/$RELEASEVER/g;

if ( not /^#!(.+)$/ ) {
  die "Ill formed input!";
}

say '<!DOCTYPE html>
<html>
<head>

<meta content="text/html; charset=UTF-8" http-equiv="Content-Type">

<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/bootstrap/3.0.3/css/bootstrap.min.css">
<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/bootstrap/3.0.3/css/bootstrap-responsive.min.css">

<style type="text/css">
.nav { }
.nav li { float: left; width: 110px; }
.container { background-color: #ffffff; padding: 30px; }
.content { padding: 30px; }
body, h1, h2, h3, h4 { font-family: "Trebuchet MS","Helvetica Neue",Arial,Helvetica,sans-serif,"Georgia";}
body { background-color: #eeeeee; }
header { width: 100%; }
blockquote p { font-size: 14px; }
</style>';

say "<title>ZPar | $1</title>";
say '</head>

<body>
<div class="table-bordered container">
<div class="content">

<header>
<div class="page-header text-center">';

say "<h1>$1</h1>
</div>
</header>";

while (<>) {
  s/<RELEASEVER>/$RELEASEVER/g;
  print;
}

say "</div>
</div>
<footer class=\"text-center\">
<p>
ZPar Release $RELEASEVER
</p>
</footer>
</body>
</html>";
