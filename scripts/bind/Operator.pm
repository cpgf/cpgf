package Operator;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		operator => undef,

		location => undef,
		visibility => 'public',

		template => 0,

		paramList => [],
		templateParamList => [],

		%args
	};

	bless $self, $class;

	return $self;
}

sub getList
{
	my ($self, $class) = @_;

	return $class->{operatorList};
}


1;
