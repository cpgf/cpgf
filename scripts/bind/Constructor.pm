package Constructor;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		template => 0,

		paramList => [],
		templateParamList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
