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
		_category => Item::catOperator,
		
		_operator => undef,
		
		_returnType => undef,

		_const => 0,

		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub isConst { return shift->{_const}; }

sub getOperator { return shift->{_operator}; }

sub getReturnType { return shift->{_returnType}; }
sub setReturnType { my ($self, $value) = @_; $self->{_returnType} = $value; }

sub getList
{
	my ($self, $class) = @_;

	return $class->getOperatorList;
}


1;
