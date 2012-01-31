package Class;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,

		baseNameList => [], # names of base classes

		constructorList => [],
		fieldList => [],
		methodList => [],
		enumList => [],
		operatorList => [],
		classNameList => [], # names of inner classes

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
