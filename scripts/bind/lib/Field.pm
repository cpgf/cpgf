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
		
		location => undef,
		visibility => 'public',

		static => 0,

		%args
	};

	bless $self, $class;

	return $self;
}

sub getList
{
	my ($self, $class) = @_;

	return $class->{fieldList};
}


1;
