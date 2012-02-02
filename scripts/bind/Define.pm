package Define;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		location => undef,
		visibility => 'public',
		
		name => undef,
		value => undef,

		%args
	};

	bless $self, $class;

	return $self;
}

sub getList
{
	my ($self, $class) = @_;

	return $class->{defineList};
}


1;
