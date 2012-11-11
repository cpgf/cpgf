BEGIN {
	my $p = $0;
	$p =~ s![^/\\]+$!!;
	unshift @INC, $p;
}

use strict;
use warnings;

my $tokenFile = undef;
my @inputFiles = ();

our @replacements = ();

&doMain();


sub doMain
{
	&parseCommandLine;
	
	require $tokenFile;

	foreach(@inputFiles) {	
		&doReplaceFile($_);
	}
	
	print "Done.\n";
}

sub parseCommandLine
{
	foreach(@ARGV) {
		my $arg = $_;

		if($arg =~ /\.pl$/ && not defined($tokenFile)) {
			$tokenFile = $arg;
			next;
		}

		push @inputFiles, $arg;
	}

	if(not defined($tokenFile)) {
		die "No token file. \n";
	}

	die "No input files.\n" if($#inputFiles < 0);
}

sub doReplaceFile
{
	my ($fileName) = @_;

	print "Replacing $fileName...\n";
	open FH, "<" . $fileName or die "Can't open file $fileName to read.\n";
	my @lines = <FH>;
	close FH;
	
	open FH, ">" . $fileName or die "Can't open file $fileName to write.\n";
	foreach(@lines) {
		my $ln = $_;
		chomp($ln);
		my $blank = &isBlankLine($ln);
		foreach(@replacements) {
			my $r = $_;
			my $pattern = $r->[0];
			my $value = $r->[1];
			$value = "qq($value)";
			$ln =~ s/$pattern/$value/gee;
		}
		if(!$blank && &isBlankLine($ln)) {
			next;
		}
		print FH $ln . "\n";
	}
	close FH;
}

sub isBlankLine
{
	my ($line) = (@_);
	
	return $line =~ /^\s*$/;
}
