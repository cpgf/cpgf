package Constructor;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		paramList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
