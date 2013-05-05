BEGIN {
	$basePath = $0;
	$basePath =~ s![^/\\]+$!!;
	unshift @INC, $basePath;
	unshift @INC, $basePath . "lib";
}

use strict;
use warnings;

use File::Basename;
use File::Copy;
use File::Path;

use FilePrefix;

my %defaultConfig = (
	path => undef,
	files => [],
	excludePath => [],
	excludeFiles => [],
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
		excludeFiles => [ '*.bat', 'qtest.sh' ],
		recursive => 0,
	},

	{
		%defaultConfig,

		path => "include/cpgf",
		files => [ '*.h', '*.hpp', '*.cpp', '*.inl' ],
		excludePath => [ 'game' ],
		recursive => 1,
	},

	{
		%defaultConfig,

		path => "src",
		files => [ '*.h', '*.hpp', '*.cpp', '*.inl' ],
		excludePath => [ 'game' ],
		recursive => 1,
	},

	{
		%defaultConfig,

		path => "samples",
		files => [ '*.h', '*.cpp', 'readme*', '*.js', '*.lua', '*.py', '*.vcproj', '*.cbp', '*.fbp' ],
		excludePath => [ 'bin', 'obj', 'release', 'debug' ],
		recursive => 1,
	},

	{
		%defaultConfig,

		path => "test",
		files => [ '*.h', '*.cpp' ],
		excludePath => [ 'xml' ],
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
		excludeFiles => [ '*.jar', '.classpath', '.project' ],
		excludePath => [ 'xml', 'bin' ],
		recursive => 1,
	},

];

my $scriptPath;
my $home;
my $releasePath;
my $destPath;
my $version;

my $cppHeadContent = undef;
my $perlHeadContent = undef;

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
	
	&processBuildConfig($destPath . 'build/build.config.txt');
	
	mkdir($destPath . 'lib');

	&makeZip;
}

sub processBuildConfig
{
	my ($fileName) = @_;
	
	open FH, '<' . $fileName or die "Can't open build config file $fileName to read.\n";
	my @lines = <FH>;
	close FH;

	open FH, '>' . $fileName or die "Can't open build config file $fileName to write.\n";
	foreach(@lines) {
		my $s = $_;
		if($s =~ /#release=false/) {
			$s =~ s/\s*#release=false//;
			$s =~ s/\b1\b/0/;
		}
		print FH $s;
	}
	close FH;
}

sub makeZip
{
	chdir($releasePath);
	
	my $v = "$version";
	$v =~ s/\./_/g;
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
		next if(matchFileList($file, $pattern->{excludeFiles}));

		copy($file, $destFileName);
		&processDestFile($pattern, $destFileName);
	}
}

sub processDestFile
{
	my ($pattern, $destFile) = @_;

	if($pattern->{licenseHead}) {
		&licenseFile($destFile);
	}
	
	if($destFile =~ /\.doxyfile$/i) {
		&compactFile($destFile);
	}
}

sub licenseFile
{
	my ($destFile) = @_;

	if($destFile =~ /\.(h|cpp|java)$/i) {
		$cppHeadContent = &FilePrefix::prefixLoadHeadContent('licensehead_apache.txt') unless defined $cppHeadContent;
		&FilePrefix::prefixProcessFile($destFile, $cppHeadContent);
	}
	elsif($destFile =~ /\.p[ml]$/i) {
		$perlHeadContent = &FilePrefix::prefixLoadHeadContent('licensehead_apache_perl.txt') unless defined $perlHeadContent;
		&FilePrefix::prefixProcessFile($destFile, $perlHeadContent);
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

	return &getFileName($file) =~ m!^$pattern$!i;
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
