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

		_static => 0,
		
		_paramList => [],

		_templateParamList => [],
		
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

sub isStatic { return shift->{_static}; }
sub setStatic { my ($self, $value) = @_; $self->{_static} = $value; }

sub getParamList { return shift->{_paramList}; }
sub hasParam
{
	return $#{@{shift->{_paramList}}} >= 0;
}
sub getParamCount
{
	return $#{@{shift->{_paramList}}} + 1;
}
sub addParam
{
	my ($self, $param) = @_;

	Util::listPush($self->{_paramList}, $param);
}

sub getTemplateParamList { return shift->{_templateParamList}; }
sub isTemplate
{
	return $#{@{shift->{_templateParamList}}} >= 0;
}
sub addTemplateParam
{
	my ($self, $param) = @_;

	Util::listPush($self->{_templateParamList}, $param);
}


1;
