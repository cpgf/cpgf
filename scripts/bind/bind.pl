$basePath = '';

BEGIN {
	$basePath = $0;
	$basePath =~ s![^/\\]+$!!;
	unshift @INC, $basePath;
	unshift @INC, $basePath . "lib";
}

require 'config.default.pl';

my $bindConfig = {
	%{$config},
};

use Getopt::Long;

my $configFile = undef;
my @xmlFileNames = ();
my $outputPath = './output';
my $cppOutputPath = './output';

GetOptions(
	'xml=s' => \@xmlFileNames,
	'config=s' => \$configFile,
	'output=s' => \$outputPath,
	'cppoutput=s' => \$cppOutputPath,
);

if(defined $configFile) {
	require $configFile;
	$bindConfig = {
		%{$bindConfig},
		%{$config},
	};
}

use strict;
use warnings;

use DoxyXmlLoader;
use CodeWriter;
use MetaClassWriter;
use MetaWriter;

use Data::Dumper;

&greeting;

&usage if($#xmlFileNames < 0);

$bindConfig->{outputDir} = $outputPath;
$bindConfig->{cppOutputDir} = $cppOutputPath;
&doMain;

sub greeting
{
	print <<EOM;
cpgf library meta data generating tool.
Copyright 2012, Wang Qi

EOM
}
	
sub usage
{
	print <<EOM;
Usage:
EOM
	die "\n";
}

sub doMain
{
	print "Processing, waiting...\n";
	foreach(@xmlFileNames) {
		my $xmlName = $_;

		my $loader = new DoxyXmlLoader;
		$loader->parseFile($xmlName);
		$loader->fixup();

		#print Dumper($loader->{classList});

		my $metaWriter = new MetaWriter(
			_classList => $loader->getClassList,
			_fileMap => $loader->getFileMap,
			_config => $bindConfig,
		);

		$metaWriter->write();
	}
	
	print "Written " . &Util::getWrittenFileCount . " files. \n";
	print "Skipped " . &Util::getSkippedFileCount . " identical files.\n";
	print "Done.\n";
}
