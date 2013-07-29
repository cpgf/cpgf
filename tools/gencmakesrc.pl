use strict;
use warnings;
use File::Basename;
use Data::Dump;

my $srcRoot = '${SRC_PATH}';
my $srcMaster = '${SRC_LIB}';
my %sourceFileExtensions = (
	'cpp' => 1
);

my @folderData = ();

&doMain();

sub doMain
{
	if(scalar(@ARGV) != 1) {
		die "Usage: gencmakesrc PATH_TO_SOURCE \n";
	}

	my $path = $ARGV[0];
	$path =~ s![\\/]$!!;
	&loadFilesData($path, "");
	&processFilesData();
	&generateMaster();

#	Data::Dump::dump(@folderData);
}

sub processFilesData
{
	foreach(@folderData) {
		next if(&shouldSkip($_));
		&processOneFolder($_);
		print "\n";
	}
}

sub processOneFolder
{
	my ($data) = @_;

	my $srcName = &makeSrcName($data);
	my $relative = $data->{relative};
	$relative =~ s!\\!/!;
	if($relative ne "") {
		$relative = '/' . $relative;
	}
	$relative .= '/';
	print 'SET(', $srcName, "\n";
	foreach my $file(@{$data->{files}}) {
		print "\t", $srcRoot, $relative, $file, "\n";
	}
	print ")\n";

	my $filter = $data->{relative};
	$filter =~ s!/!\\!g;
	$filter =~ s!\\!\\\\!g;
	$filter = 'src\\\\' . $filter;
	print "SOURCE_GROUP($filter FILES \${$srcName})\n";
}

sub generateMaster()
{
	print "set($srcMaster\n";
	foreach my $data(@folderData) {
		next if(&shouldSkip($data));
		my $srcName = &makeSrcName($data);
		print "\t\${$srcName}\n";
	}
	print ")\n";
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
