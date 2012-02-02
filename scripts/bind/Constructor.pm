package Constructor;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		visibility => 'public',
		location => undef,

		template => 0,

		paramList => [],
		templateParamList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
