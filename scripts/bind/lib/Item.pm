package Item;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_name => undef,
		
		_location => undef,
		_visibility => 'public',
		
		%args
	};

	bless $self, ref $class || $class;

	return $self;
}

sub getName { return shift->{_name}; }
sub setName { my ($self, $value) = @_; $self->{_name} = $value; }

sub getLocation { return shift->{_location}; }
sub setLocation { my ($self, $value) = @_; $self->{_location} = $value; }

sub getVisibility { return shift->{_visibility}; }
sub setVisibility { my ($self, $value) = @_; $self->{_visibility} = $value; }


1;
