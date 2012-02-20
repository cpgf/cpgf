package Destructor;

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
		_category => Item::catDestructor,
		
		%args
	};

	Util::assignValues($self, $values);

	return $self;
}


1;
