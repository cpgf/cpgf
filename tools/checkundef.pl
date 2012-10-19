use strict;
use warnings;

my ($sourceFileNames) = @ARGV;

&usage unless defined $sourceFileNames;

my @files = glob($sourceFileNames);

foreach(@files) {
	next if(-d $_);
	&processFile($_);
	print "\n\n";
}

sub processFile
{
	my ($fn) = @_;
	my %defines = ();
	my %fullDefines = ();
	
	print "Process $fn \n";
	open FH, "<$fn" or die "Can't open file $fn to read.\n";
	while(<FH>) {
		my $ln = $_;
		
		if($ln =~ /^\s*#\s*define\s+(\w+)/) {
			if($ln =~ /define\s+__\S+_H/) {
				next;
			}
			$defines{$1} = 1;
			$fullDefines{$1} = -1;
		}
		if($ln =~ /^\s*#\s*undef\s+(\w+)/) {
			my $def = $1;
			if(! $defines{$def}) {
				print "Undefining an unexist macro $def\n";
			}
			$defines{$def} = 0;
		}
	}
	close FH;
	
	foreach my $def(sort keys %defines) {
		if($defines{$def}) {
			print "#undef $def\n";
		}
	}

	open FH, "<$fn" or die "Can't open file $fn to read.\n";
	while(<FH>) {
		my $ln = $_;

		foreach my $def(keys %fullDefines) {
			if($ln =~ /\b$def\b/ && $ln !~ /^\s*#\s*undef\s+/) {
				$fullDefines{$def}++;
			}
		}		
	}
	close FH;
	
	foreach my $def(sort keys %fullDefines) {
		if($fullDefines{$def} <= 0) {
			print "Unused $def\n";
		}
	}
}

sub usage
{
	die "Usage: \n";
}
