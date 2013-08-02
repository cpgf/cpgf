use strict;
use warnings;

my $STATE_SECTION = 1;
my $STATE_SIZE = 2;

my $state = $STATE_SECTION;

my $sectionMap = {};
my $symbolList = [];

my $previousData = undef;

my $maxSize = 0;
&doMain($ARGV[0]);

sub doMain
{
	my ($fileName) = @_;

	&collectData($fileName);
	&statsData($symbolList);
}

sub collectData
{
	my ($fileName) = @_;
	my $fh;
	if(defined $fileName) {
		open $fh, '<' . $fileName or die "Can't open file $fileName to read. \n";
		print STDERR "Processing $fileName.\n";
	}
	else {
		$fh = *STDIN;
	}
	while(my $line = <$fh>) {
		if($state == $STATE_SECTION) {
			&searchSection($line);
		}
		elsif($state == $STATE_SIZE) {
			&searchSize($line);
		}
	}
	if(defined $fileName) {
		close $fh;
	}
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

	print "[1].Sort by merged size\n";
	&doStatsOutput($symbolMap, 'size');

	print "\n[2].Sort by merged count\n";
	&doStatsOutput($symbolMap, 'count');
	
	print "\n[3].Sort by individual size\n";
	@sortedList = sort {
		return ($b->{size} <=> $a->{size});
	} @{$symbolList};
	foreach my $k(@sortedList) {
		next unless $k->{size};
		printf "%8d\t%s\t\t%s\n", $k->{size}, $k->{name}, $k->{line};
		if($k->{size} < 500) {
			print "Items size smaller than 500 are omitted. \n";
			last;
		}
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

	my $total = 0;
	my @keys = sort {
		return ($symbolMap->{$b}->{$keyName} <=> $symbolMap->{$a}->{$keyName});
	} keys(%{$symbolMap});

	foreach my $k(@keys) {
		printf "%8d\t%s\n", $symbolMap->{$k}->{$keyName}, $k;
		$total += $symbolMap->{$k}->{$keyName};
	}
	print "Total: $total\n";
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

	if($line =~ /^\s*(\d{4}):(\w{8})\s+/) {
		my ($section, $addr) = (hex($1), hex($2));
		my $symbol = undef;
		if($line =~ /^\s*(\d{4}):(\w{8})\s+.*?\b([\w\:]+)\</) {
			$symbol = $3;
		}

		$line =~ s/^\s*(\d{4}):(\w{8})\s+//;
		my $symbolData = {
			name => $symbol,
			size => 0,
			line => $line,
			section => $section,
			addr => $addr
		};

		push @{$symbolList}, $symbolData if defined($symbol);
		
		if(defined $previousData) {
			my $size = 0;
			if($section != $previousData->{section}) {
				$size = &getSizeFromSection($section, $previousData->{addr});
			}
			else {
				$size = $addr - $previousData->{addr};
#if($size > $maxSize) {
#$maxSize = $size;
#printf "!!! %d -- %x %x   %s\n", $size, $addr, $previousData->{addr}, $previousData->{line};
#}
			}
			$previousData->{size} = $size;
		}

		$previousData = $symbolData;
	}
}

sub getSizeFromSection
{
	my ($section, $addr) = @_;

	foreach my $k(keys %{$sectionMap}) {
		foreach my $i(@{$sectionMap->{$k}}) {
			if($i->{addr} <= $addr && $i->{addr} + $i->{len} > $addr) {
				my $size = $i->{addr} + $i->{len} - $addr;
				return $size;
			}
		}
	}

	return 0;
}
