use strict;
use warnings;
use Data::Dump;

my $STATE_SECTION = 1;
my $STATE_SIZE = 2;

my $state = $STATE_SECTION;

my $sectionMap = {};
my $symbolList = [];

my $firstFound = 1;
my $previousSection = -1;
my $previousAddr = 0;

if(scalar(@ARGV) != 1) {
	die "Usage:\n";
}

&doMain($ARGV[0]);

#Data::Dump::dump($symbolList);

sub doMain
{
	my ($fileName) = @_;

	&collectData($fileName);
	&statsData($symbolList);
}

sub collectData
{
	my ($fileName) = @_;
	open FH, '<' . $fileName or die "Can't open file $fileName to read. \n";
	while(my $line = <FH>) {
		if($state == $STATE_SECTION) {
			&searchSection($line);
		}
		elsif($state == $STATE_SIZE) {
			&searchSize($line);
		}
	}
	close FH;
}

sub statsData
{
	my ($symbolList) = @_;
	my $symbolMap = {};

	foreach my $symbol(@{$symbolList}) {
		if(not defined($symbolMap->{$symbol->{name}})) {
			$symbolMap->{$symbol->{name}} = {
				size => 0,
				count => 0
			};
		}
		my $data = $symbolMap->{$symbol->{name}};
		$data->{size} += $symbol->{size};
		$data->{count}++;
	}

	my @sortedList;

	print "[1].Sort by size\n";
	&doStatsOutput($symbolMap, 'size');

	print "\n[2].Sort by count\n";
	&doStatsOutput($symbolMap, 'count');
	
	print "\n[3].Sort by individual size\n";
	@sortedList = sort {
		return ($b->{size} <=> $a->{size});
	} @{$symbolList};
	foreach my $k(@sortedList) {
		next unless $k->{size};
		printf "%8d\t%s\t\t%s\n", $k->{size}, $k->{name}, $k->{line};
	}
return;	
	print "\n[4].Sort by individual names\n";
	@sortedList = sort {
		return (lc($a->{name}) cmp lc($b->{name})) || ($b->{size} <=> $a->{size});
	} @{$symbolList};
	foreach my $k(@sortedList) {
		next unless $k->{size};
		printf "%8d\t%s\t\t%s\n", $k->{size}, $k->{name}, $k->{line};
	}
}

sub doStatsOutput
{
	my ($symbolMap, $keyName) = @_;

	my @keys = sort {
		return ($symbolMap->{$b}->{$keyName} <=> $symbolMap->{$a}->{$keyName});
	} keys(%{$symbolMap});

	foreach my $k(@keys) {
		printf "%8d\t%s\n", $symbolMap->{$k}->{$keyName}, $k;
	}
}

sub searchSection
{
	my ($line) = @_;

	if($line =~ /^\s*Address/i) {
		$state = $STATE_SIZE;
		return;
	}
	if($line =~ /^\s*(\d{4}):(\w{8})\s+(\w{8})/) {
		my $section = {
			addr => hex($2),
			len => hex($3)
		};
		$sectionMap->{$1} = [] unless defined $sectionMap->{$1};
		push @{$sectionMap->{$1}}, $section;
	}
}

sub searchSize
{
	my ($line) = @_;

	chomp $line;

	if($line =~ /^\s*(\d{4}):(\w{8})\s+.*?\b([\w\:]+)\</) {
		my ($section, $addr, $symbol) = (hex($1), hex($2), $3);

		if($firstFound) {
			$firstFound = 0;
		}
		else {
			my $size = 0;
			if($section != $previousSection) {
				$previousSection = $section;
			}
			else {
				$size = $addr - $previousAddr;
			}
			$previousAddr = $addr;

			$line =~ s/^\s*(\d{4}):(\w{8})\s+//;
			my $symbolData = {
				name => $symbol,
				size => $size,
				line => $line
			};

			push @{$symbolList}, $symbolData;
		}
	}
}

sub getSizeFromSection
{
	my ($section, $addr) = @_;
}
