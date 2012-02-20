package Operator;

use base qw(Item);

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {};

	bless $self, ref $class || $class;

	$self = $self->SUPER::new(%args);

	my $values = {
		operator => undef,

		static => 0,
		const => 0,

		template => 0,

		paramList => [],
		templateParamList => [],

		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub getList
{
	my ($self, $class) = @_;

	return $class->{operatorList};
}


1;
