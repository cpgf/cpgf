package Define;

use base qw(Item);

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {};

	bless $self, ref $class || $class;

	$self = $self->SUPER::new(%args);

	my $values = {
		_value => undef,

		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub getValue { return shift->{_value}; }

sub getList
{
	my ($self, $class) = @_;

	return $class->{defineList};
}


1;
