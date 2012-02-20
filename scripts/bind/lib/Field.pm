package Field;

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
		_category => Item::catField,
		
		_type => undef,
		
		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub getType { return shift->{_type}; }

sub getList
{
	my ($self, $class) = @_;

	return $class->getFieldList;
}


1;
