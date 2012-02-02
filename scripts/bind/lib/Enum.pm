package Enum;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,

		location => undef,
		visibility => 'public',

		valueList => [],

		%args
	};

	bless $self, $class;

	return $self;
}

sub getList
{
	my ($self, $class) = @_;

	return $class->{enumList};
}


1;
