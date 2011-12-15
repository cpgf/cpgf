use strict;
use warnings;

my @paths = ();

my $tempName = 't_A_0h_38_0';
my $tempSource = $tempName . '.cpp';
my $compiler = "gcc -I../include -Ie:/sourcecode/cpp/lua/src -Ie:/sourcecode/cpp/lua/etc -c -Wall $tempSource";

&doMain;

sub doMain
{
	&parseCommandLine;
	
	foreach(@paths) {
		&doPath($_);
	}
	
	&finish;
}

sub parseCommandLine
{
	foreach(@ARGV) {
		push @paths, $_;
	}
	
	&usage if($#paths < 0);
}

sub usage
{
	die "Usage: perl checkheader.pl path1 path2 ... \n";
}

sub doPath
{
	my $path = $_;
	my @files = glob($path);
	
	foreach(@files) {
		&doFile($_);
	}

	if($#files < 0) {
		print "Warning: can't find any files in ", $path, "\n";
	}
}

sub doFile
{
	my $file = $_;
	
	open FH, '>' . $tempSource or die "Can't open file $tempSource to write.\n";
	print FH '#include ', '"', $file, '"', "\n";
	print FH "int main() { return 0; } \n";
	close FH;
	
	if(system $compiler) {
		&finish;
		die "Error!\n";
	}
}

sub finish
{
	unlink $tempSource;
	unlink $tempName . '.o';
}
