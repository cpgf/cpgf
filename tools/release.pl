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

		path => "",
		files => [ 'readme.txt', 'license', 'changelog' ],
		recursive => 0,
	},

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

	{
		%defaultConfig,

		path => "src",
		files => [ '*' ],
		excludePath => [ 'game' ],
		recursive => 1,
	},

	{
		%defaultConfig,

		path => "samples",
		files => [ '*' ],
		recursive => 1,
	},

	{
		%defaultConfig,

		path => "test",
		files => [ '*' ],
		recursive => 1,
	},

	{
		%defaultConfig,

		path => "tools",
		files => [ 'gen_pp.pl' ],
		recursive => 0,
	},

	{
		%defaultConfig,

		path => "tools/metagen",
		files => [ '*' ],
		excludePath => [ 'xml' ],
		recursive => 1,
	},

];

my $scriptPath;
my $home;
my $releasePath;
my $destPath;
my $version;

&doMain;

sub doMain
{
	&usage unless($#ARGV == 1);

	$scriptPath = &normalizePath(dirname($0));

	$home = $scriptPath . '../';
	$releasePath = &normalizePath($ARGV[0]);
	$destPath = $releasePath . 'cpgf/';
	$version = $ARGV[1];
	
	if(-e $destPath) {
		die "$destPath exists. \n";
	}

	foreach(@{$patternList}) {
		&processPattern($_);
	}
	
	mkdir($destPath . 'lib');

	&makeZip;
}

sub makeZip
{
	chdir($releasePath);
	
	my $v = "$version";
	$v =~ s/\./_/;
	my $zipName = "cpgf_$v.zip";
	system "zip -rq -9 $zipName cpgf";
}

sub processPattern
{
	my ($pattern) = @_;

	&doProcessPattern($pattern, $home . $pattern->{path}, $destPath . $pattern->{path});
}

sub doProcessPattern
{
	my ($pattern, $path, $dest) = @_;

	$dest = &normalizePath($dest);

	mkpath($dest);

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

		next unless(matchFileList($file, $pattern->{files}));

		copy($file, $destFileName);
		&processDestFile($pattern, $destFileName);
	}
}

sub processDestFile
{
	my ($pattern, $destFile) = @_;

	if($pattern->{licenseHead}) {
		my $license = undef;

		if($destFile =~ /\.(h|cpp)$/i) {
			$license = 'licensehead_apache.txt';
		}
		elsif($destFile =~ /\.p[ml]$/i) {
			$license = 'licensehead_apache_perl.txt';
		}
		if(defined($license)) {
			system "perl $scriptPath/file_prefix.pl $scriptPath/$license $destFile";
		}
	}
	
	if($destFile =~ /\.doxyfile$/i) {
		&compactFile($destFile);
	}
}

sub compactFile
{
	my ($fileName) = @_;
	
	open FH, '<' . $fileName or die "Can't open file $fileName to read for compact.\n";
	my @lines = <FH>;
	close FH;
	
	open FH, '>' . $fileName or die "Can't open file $fileName to write for compact.\n";
	foreach(@lines) {
		my $line = $_;
		chomp($line);
		$line =~ s/#.*$//;
		if($line !~ /^\s*$/) {
			print FH $line . "\n";
		}
	}
	close FH;
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

	$pattern =~ s/\./\\./g;
	$pattern =~ s/\*/\.\*/g;

	return &getFileName($file) =~ m!^$pattern$!;
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

sub usage
{
	print "Usage: perl release.pl destpath version \n";
	die "\n";
}
