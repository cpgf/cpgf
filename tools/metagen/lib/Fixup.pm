package Fixup;

use strict;
use warnings;

use Util;

our @EXPORT = qw(
	&fixupClassList
);

sub fixupClassList
{
	my ($classList) = @_;

	$classList = &doFixupGlobals($classList);
	$classList = &doFixupBases($classList);
	$classList = &doFixupInners($classList);
	$classList = &doFixupOwners($classList);

	return $classList;
}

sub doFixupGlobals
{
	my ($classList) = @_;

	my %fileMap = ();
	my $finished = 0;
	while(not $finished) {
		$finished = 1;

		for(my $i = 0; $i <= $#{@{$classList}}; ++$i) {
			my $c = $classList->[$i];
			next unless($c->isGlobal());

			&doFixupGlobalItems(\%fileMap, $c->getFieldList);
			&doFixupGlobalItems(\%fileMap, $c->getMethodList);
			&doFixupGlobalItems(\%fileMap, $c->getOperatorList);
			&doFixupGlobalItems(\%fileMap, $c->getEnumList);
			&doFixupGlobalItems(\%fileMap, $c->getDefineList);

			splice(@{$classList}, $i, 1);
			$finished = 0;
			last;
		}
	}

	foreach(values(%fileMap)) {
		unshift @{$classList}, $_;
	}

	return $classList;
}

sub doFixupGlobalItems
{
	my ($fileMap, $itemList) = @_;

	foreach(@{$itemList}) {
		my $item = $_;
		my $location = $item->getLocation;
		if(not defined $fileMap->{$location}) {
			$fileMap->{$location} = new Class;
			$fileMap->{$location}->setLocation($location);
		}
		&Util::listPush($item->getList($fileMap->{$location}), $item);
	}
}

sub doFixupBases
{
	my ($classList) = @_;

	foreach(@{$classList}) {
		my $target = $_;
		$target->setBaseList([]);

		foreach(@{$target->getBaseNameList}) {
			my @names = split('~', $_);
			my $baseClass = &Util::findItemByName($classList, $names[0]);
			if(defined $baseClass) {
				$target->addBase($baseClass);
			}
		}
	}

	return $classList;
}

sub doFixupInners
{
	my ($classList) = @_;

	foreach(@{$classList}) {
		my $target = $_;
		$target->setClassList([]);

		foreach(@{$target->getClassNameList}) {
			my @names = split('~', $_);
			my $innerClass = &Util::findItemByName($classList, $names[0]);
			if(defined $innerClass) {
				$innerClass->setInner(1);
				$target->addClass($innerClass);
				$innerClass->setVisibility($names[1]);
			}
		}
	}
	
	for(my $i = $#{@{$classList}}; $i >= 0; --$i) {
		my $c = $classList->[$i];
		if($c->isInner) {
			splice(@{$classList}, $i, 1);
		}
	}

	return $classList;
}

sub doFixupOwners
{
	my ($classList) = @_;

	foreach(@{$classList}) {
		my $target = $_;
		$target->fixupOwners();
	}

	return $classList;
}


1;
