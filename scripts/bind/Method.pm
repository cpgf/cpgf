package Method;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,
		returnType => undef,
		
		member => 0,
		static => 0,

		paramList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
