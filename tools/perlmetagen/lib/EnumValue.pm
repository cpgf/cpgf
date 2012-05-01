package EnumValue;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_name => undef,
		_value => undef,

		%args
	};

	bless $self, $class;

	return $self;
}

sub getName { return shift->{_name}; }
sub getValue { return shift->{_value}; }


1;
