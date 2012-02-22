use strict;
use warnings;

use File::Basename;
use File::Copy;
use File::Path;

my %defaultConfig = (
	path => undef,
	files => [],
	excludePath => [],
	excludeFile => [],
	recursive => 1,
	licenseHead => 1,
);

my $patternList = [
	{
		%defaultConfig,

		path => "build",
		files => [ '*' ],
		recursive => 0,
	},

	{
		%defaultConfig,

		path => "include/cpgf",
		files => [ '*' ],
		excludePath => [ 'game' ],
		recursive => 1,
	},

];


my $home;
my $destPath;
my $version;

&doMain;

sub doMain
{
	&usage unless($#ARGV == 1);

	my $currentPath = &normalizePath(dirname($0));

	$home = $currentPath . '../';
	$destPath = &normalizePath($ARGV[0]);
	$version = $ARGV[1];

	print $home;

	foreach(@{$patternList}) {
		&processPattern($_);
	}
}

sub normalizePath
{
	my ($path) = @_;

	$path =~ s!\\!/!g;
	$path =~ s!/$!!;
	$path .= '/';

	return $path;
}

sub getFileName
{
	my ($file) = @_;

	$file =~ s!.*/!!;
	$file =~ s!.*\\!!;

	return $file;
}

sub processPattern
{
	my ($pattern) = @_;

	&doProcessPattern($pattern, $pattern->{path}, $destPath . $pattern->{path});
}

sub doProcessPattern
{
	my ($pattern, $path, $dest) = @_;

	$dest = &normalizePath($dest);

	mkpath($dest);

	$path = $home . $path;
	$path = &normalizePath($path);

	my @fileList = glob($path . '*');

	foreach(@fileList) {
		my $file = $_;
		
		my $destFileName = $dest . &getFileName($file);

		if(-d $file) {
			if($pattern->{recursive}) {
				if(!&matchFileList($file, $pattern->{excludePath}))	{
					&doProcessPattern($pattern, $file, $destFileName);
				}
			}

			next;
		}

		copy($file, $dest);
	}
}

sub matchFileList
{
	my ($file, $patternList) = @_;

	foreach(@{$patternList}) {
		if(&matchFile($file, $_)) {
			return 1;
		}
	}

	return 0;
}

sub matchFile
{
	my ($file, $pattern) = @_;

	$pattern =~ s/\*/.*/g;
	$pattern =~ s/\./\\\\./g;

	return &getFileName($file) =~ $pattern;
}

sub usage
{
	print "Usage: perl release.pl destpath version \n";
	die "\n";
}
