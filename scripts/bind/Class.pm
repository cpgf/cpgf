package Class;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,

		baseList => [],

		constructorList => [],
		fieldList => [],
		methodList => [],
		classList => [],
		enumList => [],
		operatorList => [],

		%args
	};

	bless $self, $class;

	return $self;
}

sub isGlobal
{
	my ($self) = @_;

	return not defined $self->{name};
}


1;
