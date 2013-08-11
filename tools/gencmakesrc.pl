use strict;
use warnings;
use File::Basename;
use Getopt::Long;

my %sourceFileExtensions = (
	'cpp' => 1
);

my @folderData = ();

my $sourcePath = '';
my $outputFile = '';
my $srcRoot = '${SRC_PATH}';
my $srcMaster = '${SRC_MASTER}';
my $silent;

&doMain();

sub doMain
{
	&parseCommandLine;

	&loadFilesData($sourcePath, "");

	my $context = "#Auto generated, don't modify.\n\n";
	$context = &processFilesData($context);
	$context = &generateMaster($context);

	&outputResult($context);
}

sub myLog
{
	my ($text) = @_;

	if($silent) {
		return;
	}

	print $text;
}

sub outputResult
{
	my ($content) = @_;

	if(! $outputFile) {
		print $content;
	}
	else {
		if(&shouldRewriteFile($content, $outputFile)) {
			open FH, '>' . $outputFile or die "Can't open file $outputFile to write.\n";
			print FH $content;
			close FH;

			myLog("File has been written.\n");
		}
		else {
			myLog("No need to update.\n");
		}
	}
}

sub shouldRewriteFile
{
	my ($content, $fileName) = @_;

	open FH, '<' . $fileName or return 1;
	local $/;
	my $s = <FH>;
	close FH;
	return $s ne $content;
}

sub parseCommandLine
{
	my $help;
	GetOptions(
		"help" => \$help,
		"silent" => \$silent,
		"path=s" => \$sourcePath,
		"output=s" => \$outputFile,
		"root=s" => \$srcRoot,
		"master=s" => \$srcMaster,
	);
	
	my $usage = "Usage: gencmakesrc --path PATH_TO_SOURCE [--output OUTPUT_FILE_NAME --root SRC_ROOT --master MASTER_LIST_NAME --silent]";

	if($help) {
		print "$usage\n";
		exit(0);
	}

	if(! $sourcePath) {
		print "$usage\n";
		die "--help for detailed help information.\n";
	}

	$sourcePath =~ s![\\/]$!!;
}

sub processFilesData
{
	my ($context) = @_;

	foreach(@folderData) {
		next if(&shouldSkip($_));
		$context = &processOneFolder($context, $_);
		$context .= "\n";
	}

	return $context;
}

sub processOneFolder
{
	my ($context, $data) = @_;

	my $srcName = &makeSrcName($data);
	my $relative = $data->{relative};
	$relative =~ s!\\!/!;
	if($relative ne "") {
		$relative = '/' . $relative;
	}
	$relative .= '/';
	$context .= 'SET(' . $srcName . "\n";
	foreach my $file(@{$data->{files}}) {
		$context .= "\t" . $srcRoot . $relative . $file . "\n";
	}
	$context .= ")\n";

	my $filter = $data->{relative};
	$filter =~ s!/!\\!g;
	$filter =~ s!\\!\\\\!g;
	$filter = 'src\\\\' . $filter;
	$context .= "SOURCE_GROUP($filter FILES \${$srcName})\n";

	return $context;
}

sub generateMaster()
{
	my ($context) = @_;

	if($srcMaster eq '' or $srcMaster eq '#') {
		myLog("Master list is not generated.\n");
		return $context;
	}

	$context .= "set(" . &stripName($srcMaster) . "\n";
	foreach my $data(@folderData) {
		next if(&shouldSkip($data));
		my $srcName = &makeSrcName($data);
		$context .= "\t\${$srcName}\n";
	}
	$context .= ")\n";

	return $context;
}

sub stripName
{
	my ($name) = @_;
	$name =~ s/\W//g;
	return $name;
}

sub shouldSkip
{
	my ($data) = @_;

	return scalar(@{$data->{files}}) == 0;
}

sub makeSrcName
{
	my ($data) = @_;

	if($data->{relative} eq '') {
		return 'SRC_ROOT';
	}

	my $name = 'src_' . $data->{relative};
	$name =~ s![\\/]!_!g;
	return uc($name);
}

sub loadFilesData
{
	my ($path, $relative) = @_;

	$path .= '/';

	my $data = {
		relative => $relative,
		files => []
	};

	push @folderData, $data;

	my @files = glob($path . '*');
	foreach my $file(@files) {
		my $fileName = fileparse($file);
		if(-d $file) {
			if($fileName =~ /^\./) {
				next;
			}
			&loadFilesData($file, $relative . ($relative eq "" ? "" : "/") . $fileName);
		}
		else {
			my $suffix = "";

			if($fileName =~ /\.([^\.]+)$/) {
				$suffix = $1;
			}

			if(! $sourceFileExtensions{$suffix}) {
				next;
			}
			push @{$data->{files}}, $fileName;
		}
	}
}
