package Enum;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,

		valueList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
