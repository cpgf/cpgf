use strict;
use warnings;

use DoxyXmlLoader;
use CodeWriter;

use Data::Dumper;

#my $xmlName = "test/xml/class_n_n_n_1_1_my_class.xml";
my $xmlName = "test/xml/index.xml";

my $loader = new DoxyXmlLoader;
$loader->parseFile($xmlName);

print Dumper($loader->{classList});

foreach(@{$loader->{classList}}) {
	my $writer = new CodeWriter;
#	print Util::dumpClass($writer, $_);
}


__END__

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