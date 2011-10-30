package TagHelper;

sub new
{
	my $class = shift || {};

	my $self = {
	};

	bless $self, $class;

	return $self;
}

sub getAllText
{
	my ($self, $tagProcessor) = @_;
	
	my $lines = $tagProcessor->getAllNextText(1);
	
	return undef unless defined $lines;
	
	my $text = join("\n", @$lines);
	$text =~ s/^\s+//s;
	$text =~ s/\s+$//s;

	return $text;
}

sub paramAsBoolean
{
	my ($self, $paramMap, $name, $default) = @_;
	
	my $value = $paramMap->{$name};
	
	return $default unless defined $value;
	
	return 1 if($value =~ /^\s*true\s*$/i);
	return 0 if($value =~ /^\s*false\s*$/i);
	
	die "Wrong boolean value for parameter." unless($value =~ /^\s*\d+\s*$/);
	
	return int($value);
}

sub paramAsInt
{
	my ($self, $paramMap, $name, $default) = @_;
	
	my $value = $paramMap->{$name};
	
	return $default unless defined $value;
	
	die "Wrong integer value for parameter." unless($value =~ /^\s*\d+\s*$/);
	
	return int($value);
}

sub paramAsString
{
	my ($self, $paramMap, $name, $default) = @_;
	my $value = $paramMap->{$name};

	return $default unless defined $value;
	
	return $value;
}



package TagProcessor;

sub new
{
	my $class = shift || {};
	my $tags = shift;
	my $context = shift;
	my $textHandler = shift;

	my $self = {
		tags => $tags,
		context => $context,
		textHandler => $textHandler,
		
		atomList => undef,
		atomIndex => undef,
		currentTag => undef,
		currentAtom => undef,
	};

	bless $self, $class;

	return $self;
}

sub _getAtomCount
{
	my ($self) = @_;
	
	return scalar(@{$self->{atomList}});
}

sub _getAtom
{
	my ($self, $index) = @_;
	
	die "_getAtom missing index" unless defined $index;
	
	return $self->{atomList}->[$index];
}

sub _findTag
{
	my ($self, $tag) = @_;
	
	foreach(@{$self->{tags}}) {
		my $t = $_;
		if($t->{tag} =~ /$tag/i) {
			return $t;
		}
	}
	
	return undef;
}

sub getCurrentAtom
{
	my ($self) = @_;
	
	return $self->{currentAtom};
}

sub getCurrentTag
{
	my ($self) = @_;
	
	return $self->{currentTag};
}

sub getNextText
{
	my ($self, $skipText) = @_;
	
	die "getNextText missing skipText" unless defined $skipText;
	
	my $nextIndex = $self->{atomIndex} + 1;

	return undef if($nextIndex >= $self->_getAtomCount);
	
	my $atom = $self->_getAtom($nextIndex);
	return undef if($atom->{type} != &TagParser::atomTypeText);
	
	my $text = $atom->{text};
	++$self->{atomIndex} if($skipText);

	return $text;
}

sub getAllNextText
{
	my ($self, $skipText) = @_;
	
	die "getAllNextText missing skipText" unless defined $skipText;
	
	my $oldIndex = $self->{atomIndex};
	my $text = $self->getNextText(1);
	
	return undef unless(defined $text);
	
	my $lines = [];
	
	do {
		push @$lines, $text;
		$text = $self->getNextText(1);
	} while(defined $text);
	
	if(not $skipText) {
		$self->{atomIndex} = $oldIndex;
	}
	
	return $lines;
}

sub verify
{
	my ($self, $atomList) = @_;
	
	my $error = sub {
		my ($atom, $msg) = @_;
		die "Line: " . $atom->{line} . ' ' . $msg . ".\n";
	};
	
	my @stack = ();
	
	foreach(@$atomList) {
		my $atom = $_;
		
		next if($atom->{type} != &TagParser::atomTypeTag);
		my $tag = $self->_findTag($atom->{tag});
		
		if(not defined $tag) {
			&$error($atom, "Undefined tag -- " . $atom->{tag});
		}
		
		if($atom->{closeTag} && $atom->{selfClose}) {
			&$error($atom, "Tag is both close and self close -- " . $atom->{tag});
		}

		if($tag->{needClose}) {
			if(not $atom->{selfClose}) {
				if($atom->{closeTag}) {
					if($#stack < 0) {
						&$error($atom, "Closing non existing tag -- " . $atom->{tag});
					}
					else {
						my $topAtom = $stack[$#stack];
						if($topAtom->{tag} !~ /$atom->{tag}/i) {
							&$error($atom, "Closing mismatch tag -- " . $atom->{tag} . " for " . $topAtom->{tag});
						}
					}
					
					pop @stack;
				}
				else {
					push @stack, $atom;
				}
			}
		}
	}
	
	if($#stack >= 0) {
		my $topAtom = $stack[$#stack];
		&$error($topAtom, "Tag is not closed -- " . $topAtom->{tag});
	}
}

sub process
{
	my ($self, $atomList) = @_;
	
	$self->{atomList} = $atomList;
	$self->{atomIndex} = 0;

	while(1) {
		last if($self->{atomIndex} >= $self->_getAtomCount);
		
		my $atom = $self->_getAtom($self->{atomIndex});
		
		$self->{currentAtom} = $atom;
		
		my $needProcess = 1;
		
		if($needProcess && $atom->{type} == &TagParser::atomTypeText) {
			&{$self->{textHandler}}($self, $self->{context}, $atom->{text} . "\n");
			$needProcess = 0;
		}

		if($needProcess && $atom->{type} == &TagParser::atomTypeTag) {
			my $tag = $self->_findTag($atom->{tag});
			
			$self->{currentTag} = $tag;

			if(defined $tag) {
				if(defined $tag->{handler}) {
					my $passTag = lc $atom->{tag};
					if($atom->{selfClose}) {
						&{$tag->{handler}}($self, $self->{context}, $passTag, 0);
						&{$tag->{handler}}($self, $self->{context}, $passTag, 1);
					}
					else {
						&{$tag->{handler}}($self, $self->{context}, $passTag, $atom->{closeTag});
					}
				}
				else {
					die "Undefined handler for tag " . $tag->{tag} . ".\n";
				}
			}
			else {
				die "Unknown tag " . $atom->{tag} . ".\n";
			}

			$needProcess = 0;
		}
		
		++$self->{atomIndex};
	}
	
	undef $self->{atomList};
}


package TagParser;

sub new
{
	my $class = shift || {};
	my $self = {
	};

	bless $self, $class;

	return $self;
}

sub dump
{
	my ($self, $atomList) = @_;

	foreach(@$atomList) {
		my $atom = $_;

		if($atom->{type} == $self->atomTypeText) {
			print "Text atom: ", $atom->{text}, "\n";
			next;
		}

		if($atom->{type} == $self->atomTypeTag) {
			print "Tag atom: ", $atom->{tag}, " closeTag=", $atom->{closeTag}, " selfClose=", $atom->{selfClose}, "\n";
			if(defined $atom->{params}) {
				print "\tParams:";
				foreach(@{$atom->{params}}) {
					my $param = $_;
					print " name=", $param->{name}, " value=(", $param->{value}, ")";
				}
				print "\n";
			}
			next;
		}

		die "Unknown atom. \n";
	}
}

sub atomTypeText { return 1; }
sub atomTypeTag { return 2; }

sub _addTextAtom
{
	my ($self, $atomList, $lineNumber, $text) = @_;

	my $atom = {
		type => $self->atomTypeText,
		line => $lineNumber,
		text => $text,
	};

	push @$atomList, $atom;
}

sub _addTagAtom
{
	my ($self, $atomList, $lineNumber, $tagName, $paramText, $closeTag, $selfClose) = @_;
	
	undef $paramText if($paramText =~ /^\s*$/);
	
	my $params = undef;
	
	$params = $self->_parseParams($paramText) if(defined $paramText);
	my $paramMap = $self->_buildParamMap($params);
	
	my $atom = {
		type => $self->atomTypeTag,
		line => $lineNumber,
		tag => $tagName,
		paramText => $paramText,
		params => $params,
		paramMap => $paramMap,
		closeTag => $closeTag,
		selfClose => $selfClose,
	};

	push @$atomList, $atom;
}

sub _buildParamMap
{
	my ($self, $params) = @_;
	
	return undef unless defined $params;
	
	my $paramMap = {};
	
	foreach(@$params) {
		my $p = $_;
		
		$paramMap->{$p->{name}} = $p->{value};
	}
	
	return $paramMap;
}

sub _parseParams
{
	my ($self, $paramText, $lineNumber) = @_;
	my $params = [];
	
	while(1) {
		$paramText =~ s/^\s+//;
		$paramText =~ s/\s+$//;
		
		last if($paramText eq '');
		
		my ($name, $value, $remainder) = (undef, undef, undef);
		
		if($paramText =~ /^(\w+)\s*=\s*"(.*?)"(.*)$/) {
			$name = $1;
			$value = $2;
			$remainder = $3;
		}
		elsif($paramText =~ /^(\w+)\s*=\s*(\S+)(.*)$/) {
			$name = $1;
			$value = $2;
			$remainder = $3;
		}

		die "Wrong parameter at line " . $lineNumber . ".\n" unless defined $name;
		
		my $p = {
			name => $name,
			value => $value,
		};
		
		push @$params, $p;
		
		$paramText = $remainder;
	}
	
	return scalar(@$params) > 0 ? $params : undef;
}

sub _parseAtomLine
{
	my ($self, $atomList, $line, $lineNumber) = @_;

	while(1) {
		if($line =~ m!^(.*?)\<\{\s*(/?)\s*(\w+)(\s+.*?)?(/?)\s*\}?\>(.*)$!) {
			my ($before, $closeTag, $tagName, $paramText, $selfClose, $after) = ($1, $2, $3, $4, $5, $6);

			$self->_addTextAtom($atomList, $lineNumber, $before) unless($before =~ /^\s*$/);
			$self->_addTagAtom($atomList, $lineNumber, $tagName, $paramText, $closeTag eq '/', $selfClose eq '/');

			$line = $after;
			
			last if($line =~ /^\s*$/);
		}
		else {
			$self->_addTextAtom($atomList, $lineNumber, $line);

			last;
		}
	}
}

sub parse
{
	my ($self, $lines) = @_;
	my $lineCount = scalar(@$lines);
	my $atomList = [];
	my $tag = undef;

	for(my $i = 0; $i < $lineCount; ++$i) {
		my $line = $lines->[$i];
		$line =~ s/^\n+//;
		$line =~ s/\n+$//;

		$self->_parseAtomLine($atomList, $line, $i + 1);
	}

	push @$atomList, $tag if(defined $tag);

	return $atomList;
}


1;
