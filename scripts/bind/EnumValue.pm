package EnumValue;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,
		value => undef,

		%args
	};

	bless $self, $class;

	return $self;
}


1;
