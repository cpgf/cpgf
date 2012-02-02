package Operator;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		operator => undef,

		template => 0,

		paramList => [],
		templateParamList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
