use strict;
use warnings;

my $headFileName = undef;
my @filePatterns = ();

my $headContent = '';

&doMain;

sub doMain
{
	&parseCommandLine(\@ARGV);
	&loadHeadContent;
	
	foreach(@filePatterns) {
		&prefixProcessPattern($_);
	}
}

sub parseCommandLine
{
	my ($argv) = @_;
	
	$headFileName = $argv->[0];
	
	die "Missing head file name.\n" unless(defined $headFileName);
	
	for(my $i = 1; $i <= $#{@$argv}; ++$i) {
		push @filePatterns, $argv->[$i];;
	}
	
	die "Files required.\n" unless($#filePatterns >= 0);
}

sub loadHeadContent
{
	open FH, '<' . $headFileName or die "Can't open head file $headFileName to read.\n";
	my @lines = <FH>;
	close FH;
	
	$headContent = join('', @lines);
}

sub prefixProcessPattern
{
	my ($pattern) = @_;
	my @files = glob($pattern);

	die "Can't find file for pattern $pattern.\n" unless($#files >= 0);
	
	foreach(@files) {
		&prefixProcessFile($_);
	}
}

sub prefixProcessFile
{
	my ($file) = @_;
	
	my ($dev, $ino, $mode, $nlink, $uid, $gid, $rdev, $size, $atime, $mtime, $ctime, $blksize, $blocks) = stat($file);

	open FH, '<' . $file or die "Can't open head file $file to read.\n";
	my @lines = <FH>;
	close FH;
	
	my $content = join('', @lines);

	open FH, '>' . $file or die "Can't open head file $file to write.\n";
	print FH $headContent;
	print FH $content;
	close FH;

#	utime $atime, $mtime, $file;	
}
