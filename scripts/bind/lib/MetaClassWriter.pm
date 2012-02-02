package MetaClassWriter;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		class => undef,
		codeWriter => undef,
		config => undef,

		%args
	};

	bless $self, $class;

	return $self;
}

sub getScopePrefix
{
	my ($self) = @_;

	return ($self->{class}->isGlobal() ? '' : $self->{class}->{name} . '::');
}

sub write
{
	my ($self) = @_;

	$self->writeConstructor();
	$self->writeField();
	$self->writeMethod();
}

sub writeConstructor
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $action = $self->getAction("_constructor");

	return if($self->{class}->isGlobal());

	foreach(@{$self->{class}->{constructorList}}) {
		my $item = $_;
		
		next unless($self->canWrite($item));
		
		$cw->out($action . "<void * (");
		Util::writeParamList($cw, $item->{paramList}, 0);
		$cw->out(")>(_p);\n");
	}
}

sub writeField
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_field");

	foreach(@{$self->{class}->{fieldList}}) {
		my $item = $_;
		my $name = $item->{name};
		
		next unless($self->canWrite($item));
		
		$cw->out($action);
		$cw->out('(' . $self->getReplace($name) . ", ");
		$cw->out("&" . $prefix . $name . ", _p);\n");
	}
}

sub writeMethod
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_method");

	my %methodOverload = ();

	foreach(@{$self->{class}->{methodList}}) {
		my $item = $_;
		my $name = $item->{name};
		++$methodOverload{$name};
	}

	foreach(@{$self->{class}->{methodList}}) {
		my $item = $_;
		my $name = $item->{name};
		my $overload = $methodOverload{$name} > 1;
		
		next if($item->{template});
		next unless($self->canWrite($item));

		$cw->out($action);
		$cw->out('(' . $self->getReplace($name) . ", ");
		if($overload) {
			$cw->out("(" . $item->{returnType} . " (" . $prefix . "*) (");
			Util::writeParamList($cw, $item->{paramList}, 0);
			$cw->out("))");
		}
		$cw->out("&" . $prefix . $name . ", _p);\n");
	}
}

sub canWrite
{
	my ($self, $item) = @_;

	return ($self->{config}->{allowPublic} and Util::itemIsPublic($item))
		|| ($self->{config}->{allowProtected} and Util::itemIsProtected($item))
		|| ($self->{config}->{allowPrivate} and Util::itemIsPrivate($item))
	;
}

sub getAction
{
	my ($self, $name) = @_;
	
	return '_d.CPGF_MD_TEMPLATE ' . $name;
}

sub getReplace
{
	my ($self, $name) = @_;
	
	return 'replaceName("' . $name . '", _r)';
}



1;
