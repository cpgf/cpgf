package Enum;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,

		values => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
