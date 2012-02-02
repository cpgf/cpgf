package Class;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,

		visibility => 'public',
		location => undef,

		baseNameList => [], # names of base classes
		baseList => [], # base classes

		constructorList => [],
		fieldList => [],
		methodList => [],
		enumList => [],
		operatorList => [],

		classNameList => [], # names of inner classes
		classList => [], # inner classes

		defineList => [],

		template => 0,
		templateParamList => [],

		inner => 0,

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
