BEGIN {
	my $p = $0;
	$p =~ s![^/\\]+$!!;
	$p = './test/perlmod/';
	unshift @INC, $p;
}

use DoxyDocs;
use DoxyLoader;

use Data::Dumper;

#use strict;
use warnings;

my $loader = new DoxyLoader;
$loader->parse($doxydocs);

print Dumper($loader->{classList});