package FilePrefix;

use strict;
use warnings;

our @EXPORT = qw(
	&prefixLoadHeadContent
	&prefixProcessPattern
	&prefixProcessFile
);


sub prefixLoadHeadContent
{
	my ($fileName) = @_;

	open FH, '<' . $fileName or die "Can't open head file $fileName to read.\n";
	my @lines = <FH>;
	close FH;
	
	return join('', @lines);
}

sub prefixProcessPattern
{
	my ($pattern, $headContent) = @_;
	my @files = glob($pattern);

	die "Can't find file for pattern $pattern.\n" unless($#files >= 0);
	
	foreach(@files) {
		&prefixProcessFile($_, $headContent);
	}
}

sub prefixProcessFile
{
	my ($file, $headContent) = @_;
	
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


1;
