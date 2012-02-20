package Enum;

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
		_valueList => [],

		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub getValueList { return shift->{_valueList}; }

sub addValue
{
	my ($self, $value) = @_;

	Util::listPush($self->{_valueList}, $value);
}

sub getList
{
	my ($self, $class) = @_;

	return $class->{enumList};
}


1;
