use strict;
use warnings;

my $pattern;
my $replace;

our $text;

&doMain;

sub doMain
{
	if($#ARGV != 1) {
		&usage;
	}
	
	$pattern = $ARGV[0];
	$replace = $ARGV[1];
	
	while(<STDIN>) {
		$text = $_;
		eval("\$text =~ s/$pattern/$replace/");
		print $text;
	}
}

sub usage
{
	print "Usage: \n";
	die "\n";
}


