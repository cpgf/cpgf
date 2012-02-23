BEGIN {
	$basePath = $0;
	$basePath =~ s![^/\\]+$!!;
	unshift @INC, $basePath;
	unshift @INC, $basePath . "lib";
}

use strict;
use warnings;

use FilePrefix;

my $headFileName = undef;
my @filePatterns = ();

my $headContent = '';

&doMain;

sub doMain
{
	&parseCommandLine(\@ARGV);
	$headContent = &FilePrefix::prefixLoadHeadContent($headFileName);
	
	foreach(@filePatterns) {
		&FilePrefix::prefixProcessPattern($_, $headContent);
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

