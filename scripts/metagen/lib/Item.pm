package Item;

use strict;
use warnings;

our @EXPORT = qw(
	catUnknown
	catConstructor
	catDestructor
	catClass
	catDefine
	catEnum
	catField
	catMethod
	catOperator
);

sub catUnknown { return 0; }
sub catConstructor { return 1; }
sub catDestructor { return 2; }
sub catClass { return 3; }
sub catDefine { return 4; }
sub catEnum { return 5; }
sub catField { return 6; }
sub catMethod { return 7; }
sub catOperator { return 8; }

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_category => catUnknown,

		_owner => undef,
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

sub getCategory { return shift->{_category}; }

sub isConstructor { return shift->getCategory == catConstructor; }
sub isDestructor { return shift->getCategory == catDestructor; }
sub isClass { return shift->getCategory == catClass; }
sub isDefine { return shift->getCategory == catDefine; }
sub isEnum { return shift->getCategory == catEnum; }
sub isField { return shift->getCategory == catField; }
sub isMethod { return shift->getCategory == catMethod; }
sub isOperator { return shift->getCategory == catOperator; }

sub getOwner { return shift->{_owner}; }
sub setOwner { my ($self, $value) = @_; $self->{_owner} = $value; }

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


sub getList
{
	Util::fatalAbstract('getList');
}


1;
