$basePath = '';

BEGIN {
	$basePath = $0;
	$basePath =~ s![^/\\]+$!!;
	unshift @INC, $basePath . "lib";
}

require 'config.default.pl';

my $bindConfig = {
	%{$config},
};

use strict;
use warnings;

use DoxyXmlLoader;
use CodeWriter;
use MetaClassWriter;
use MetaWriter;

use Data::Dumper;

#my $xmlName = "test/xml/class_n_n_n_1_1_my_class.xml";
my $xmlName = "test/xml/index.xml";

my $loader = new DoxyXmlLoader;
$loader->parseFile($xmlName);
$loader->fixup();

#print Dumper($loader->{classList});

my $metaWriter = new MetaWriter(
	classList => $loader->{classList},
	config => $bindConfig,
);

$metaWriter->write();
