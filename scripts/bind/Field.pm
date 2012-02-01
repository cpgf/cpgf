package Field;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,
		type => undef,
		
		static => 0,

		%args
	};

	bless $self, $class;

	return $self;
}


1;
