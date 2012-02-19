package MetaWriter;

use strict;
use warnings;

use CodeWriter;
use Util;
use MetaFileWriter;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		classList => undef,
		fileMap => undef,
		config => undef,
		fileWriterList => [],
		
		%args
	};

	bless $self, $class;

	return $self;
}

sub write
{
	my ($self) = @_;

	$self->buildFileWriterList();

	foreach(@{$self->{fileWriterList}}) {
		my $fileWriter = $_;
		$fileWriter->writeHeader();
		$fileWriter->writeSource();
	}
	
	$self->createNamespaceCpp();
}

sub createNamespaceCpp
{
	my ($self) = @_;

	return unless($self->{config}->{autoRegisterToGlobal});
	
	my $outFileName = File::Spec->catfile($self->{config}->{cppOutputDir}, 'meta_namespace_' . $self->{config}->{id}) . '.cpp';
	my $content = 'const char * ' . Util::makeNamespaceSymbol($self->{config}) . ' = ';
	if(defined $self->{config}->{namespace}) {
		$content .= '"' . $self->{config}->{namespace} . '"';
	}
	else {
		$content .= '0';
	}
	$content .= ";\n";

	Util::writeToFile($outFileName, $content);
}

sub buildFileWriterList
{
	my ($self) = @_;
	my %fm = ();
	

	$self->{fileWriterList} = [];

	foreach(@{$self->{classList}}) {
		my $item = $_;
		my $location = $item->{location};

		if(not defined $fm{$location}) {
			$fm{$location} = new MetaFileWriter(
				sourceFileName => $location,
				fileMap => $self->{fileMap}->{$location},
				config => $self->{config}
			);
			Util::listPush($self->{fileWriterList}, $fm{$location});
		}
		Util::listPush($fm{$location}->{classList}, $item);
	}
}



1;
