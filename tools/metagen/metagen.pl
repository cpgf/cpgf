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
my $headerOutputPath = undef;
my $sourceOutputPath = undef;

GetOptions(
	'xml=s' => \@xmlFileNames,
	'config=s' => \$configFile,
	'headerOutput=s' => \$headerOutputPath,
	'sourceOutput=s' => \$sourceOutputPath,
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

$bindConfig->{headerOutput} = $headerOutputPath if defined $headerOutputPath;
$bindConfig->{sourceOutput} = $sourceOutputPath if defined $sourceOutputPath;
&doMain;

sub greeting
{
	print <<EOM;
cpgf library meta data generator.
Copyright 2012, Wang Qi

EOM
}
	
sub usage
{
	print <<EOM;
Usage: perl metagen.pl --xml <Doxygen XML name> --config <Config Perl Script> --headerOutput [Header output path] --sourceOutputPath [Source output path]
    --xml                Specify the Doxygen XML index file name.
    --config             Specify the config file. See config.default.pl for reference.
    --headerOutput       Optional. Specify where the .h files are created in. It can be specified in the config file too. If it's omitted, "./output" is used.
    --sourceOutput       Optional. Specify where the .cpp files are created in. It can be specified in the config file too. If it's omitted, "./output" is used.
EOM
	die "\n";
}

sub doMain
{
	my $processedCount = 0;

	print "Processing, waiting...\n";
	foreach(@xmlFileNames) {
		my $xmlName = $_;

		my $loader = new DoxyXmlLoader;
		if($loader->parseFile($xmlName)) {
			++$processedCount;

			$loader->fixup();

			my $metaWriter = new MetaWriter(
				_classList => $loader->getClassList,
				_fileMap => $loader->getFileMap,
				_config => $bindConfig,
			);

			$metaWriter->write();
		}
	}

	if($processedCount > 0) {
		print "Written " . &Util::getWrittenFileCount . " files. \n";
		print "Skipped " . &Util::getSkippedFileCount . " identical files.\n";
		print "Done.\n";
	}
	else {
		print "Nothing to process. \n";
	}
}
