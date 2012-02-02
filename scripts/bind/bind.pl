$basePath = '';

BEGIN {
	$basePath = $0;
	$basePath =~ s![^/\\]+$!!;
	unshift @INC, $basePath;
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

use Data::Dumper;

#my $xmlName = "test/xml/class_n_n_n_1_1_my_class.xml";
my $xmlName = "test/xml/index.xml";

my $loader = new DoxyXmlLoader;
$loader->parseFile($xmlName);
$loader->fixup();

print Dumper($loader->{classList});

my $codeWriter = new CodeWriter;
foreach(@{$loader->{classList}}) {
	my $class = $_;
	my $className = 'global';
	$className = $class->{name} if(not $class->isGlobal());
	$className = Util::getBaseName($className);
	my $writer = new MetaClassWriter(class => $class, codeWriter => $codeWriter, config => $bindConfig);
	$writer->beginMetaFunction("do" . $className);
	$writer->write();
	$writer->endMetaFunction();
}
print $codeWriter->{text};


