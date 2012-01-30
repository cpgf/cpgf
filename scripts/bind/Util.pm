package Util;

use strict;
use warnings;

our @EXPORT = qw(&fatal &findItemByName &listPush &getBaseName &valueYesNo);

sub fatal
{
	my @msg = @_;

	print @msg;
	die "\n";
}

sub findItemByName
{
	my ($itemList, $name) = @_;

	foreach(@{$itemList}) {
		my $base = $_;
		if($base->{name} eq $name) {
			return $base;
		}
	}

	return undef;
}

sub listPush
{
	my ($list, $item) = @_;

	push @{$list}, $item;
}

sub getBaseName
{
	my ($name) = @_;

	$name =~ s/^.*\b(\w+)$/$1/;

	return $name;
}

sub valueYesNo
{
	my ($s) = @_;

	return lc($s) eq 'no' ? 0 : 1;
}


1;
