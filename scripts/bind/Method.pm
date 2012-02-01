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
		
		static => 0,
		virtual => 0,

		paramList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
