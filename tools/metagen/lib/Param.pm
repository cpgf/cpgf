package Param;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_name => undef,
		_type => undef,
		_defaultValue => undef,

		%args
	};

	bless $self, $class;

	return $self;
}

sub getName { return shift->{_name}; }
sub getType { return shift->{_type}; }
sub getDefaultValue { return shift->{_defaultValue}; }
sub hasDefaultValue { return defined shift->{_defaultValue}; }

1;
