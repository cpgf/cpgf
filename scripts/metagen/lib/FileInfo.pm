package FileInfo;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_location => undef,

		_namespaceList => [],

		%args
	};

	bless $self, $class;

	return $self;
}

sub getLocation { return shift->{_location}; }
sub setLocation { my ($self, $value) = @_; $self->{_location} = $value; }

sub getNamespaceList { return shift->{_namespaceList}; }
sub addNamespace { my ($self, $v) = @_; Util::listPush($self->{_namespaceList}, $v); }


1;
