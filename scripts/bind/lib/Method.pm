package Method;

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
		_returnType => undef,

		_const => 0,

		_virtual => 0,
		_pureVirtual => 0,
		
		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub isConst { return shift->{_const}; }
sub isVirtual { return shift->{_virtual}; }
sub isPureVirtual { return shift->{_pureVirtual}; }

sub getReturnType { return shift->{_returnType}; }
sub setReturnType { my ($self, $value) = @_; $self->{_returnType} = $value; }

sub getList
{
	my ($self, $class) = @_;

	return $class->{methodList};
}


1;
