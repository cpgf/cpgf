package Destructor;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		visibility => 'public',
		location => undef,

		%args
	};

	bless $self, $class;

	return $self;
}


1;
