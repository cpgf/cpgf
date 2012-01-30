package Operator;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		operator => undef,

		paramList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
