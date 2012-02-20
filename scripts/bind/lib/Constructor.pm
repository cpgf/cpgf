package Constructor;

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
		template => 0,

		paramList => [],
		templateParamList => [],

		%args
	};

	Util::assignValues($self, $values);

	return $self;
}


1;
