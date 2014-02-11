use strict;
use warnings;

my $fromValue = "{indent} case {vt}:\n"
			.	"{indent+1} return variant_internal::CastVariantHelper<{type}, ResultType>::cast(v.data.{value});\n"
			.	"\n"
			;

my $fromPtr = "{indent} case {vt} | byPointer:\n"
			.	"{indent+1} return variant_internal::CastVariantHelper<{type} *, ResultType>::cast(const_cast<{type} *>(v.data.{ptr}));\n"
			.	"\n"
			;

my $fromRef = "{indent} case {vt} | byReference:\n"
			.	"{indent+1} return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile {type} *, const volatile RefValueType *>::cast(v.data.{ptr}));\n"
			.	"\n"
			;

my $canFromValue = "{indent} case {vt}:\n"
			.	"{indent+1} return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<{type}, ResultType>::CanCast;\n"
			.	"\n"
			;

my $canFromPtr = "{indent} case {vt} | byPointer:\n"
			.	"{indent+1} return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<{type} *, ResultType>::CanCast;\n"
			.	"\n"
			;

my $canFromRef = "{indent} case {vt} | byReference:\n"
			.	"{indent+1} return variant_internal::CastVariantHelper<const volatile {type} *, const volatile RefValueType *>::CanCast;\n"
			.	"\n"
			;


my @types = (
	{ vt => "vtBool", type => "bool", value => "valueBool", ptr => "ptrBool", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtChar", type => "char", value => "valueChar", ptr => "ptrChar", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtWchar", type => "wchar_t", value => "valueWchar", ptr => "ptrWchar", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtSignedChar", type => "signed char", value => "valueSignedChar", ptr => "ptrSignedChar", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtUnsignedChar", type => "unsigned char", value => "valueUnsignedChar", ptr => "ptrUnsignedChar", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtSignedShort", type => "signed short", value => "valueSignedShort", ptr => "ptrSignedShort", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtUnsignedShort", type => "unsigned short", value => "valueUnsignedShort", ptr => "ptrUnsignedShort", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtSignedInt", type => "signed int", value => "valueSignedInt", ptr => "ptrSignedInt", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtUnsignedInt", type => "unsigned int", value => "valueUnsignedInt", ptr => "ptrUnsignedInt", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtSignedLong", type => "signed long", value => "valueSignedLong", ptr => "ptrSignedLong", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtUnsignedLong", type => "unsigned long", value => "valueUnsignedLong", ptr => "ptrUnsignedLong", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtSignedLongLong", type => "signed long long", value => "valueSignedLongLong", ptr => "ptrSignedLongLong", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtUnsignedLongLong", type => "unsigned long long", value => "valueUnsignedLongLong", ptr => "ptrUnsignedLongLong", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtFloat", type => "float", value => "valueFloat", ptr => "ptrFloat", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtDouble", type => "double", value => "valueDouble", ptr => "ptrDouble", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },
	{ vt => "vtLongDouble", type => "long double", value => "valueLongDouble", ptr => "ptrLongDouble", fromValue => $fromValue, fromPtr => $fromPtr, fromRef => $fromRef, canFromValue => $canFromValue, canFromPtr => $canFromPtr, canFromRef => $canFromRef },

	{ vt => "vtPointer", type => "void *", value => "valuePointer", ptr => "ptrPointer",
		fromValue => "{indent} case {vt}:\n"
				.	 "{indent+1} return variant_internal::CastVariantHelper<const volatile void *, ResultType>::cast(v.data.{value});\n"
				.	 "\n"
		,
		fromPtr => "{indent} case {vt} | byPointer:\n"
				.	 "{indent+1} return variant_internal::CastVariantHelper<void *, ResultType>::cast(const_cast<void *>(v.data.{ptr}));\n"
				.	 "\n"
		,
		fromRef => "{indent} case {vt} | byReference:\n"
				.	 "{indent+1} return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::cast(v.data.{ptr}));\n"
				.	 "\n"
		,
		canFromValue => "{indent} case {vt}:\n"
				.	 "{indent+1} return variant_internal::CastVariantHelper<const volatile void *, ResultType>::CanCast;\n"
				.	 "\n"
		,
		canFromPtr => "{indent} case {vt} | byPointer:\n"
				.	 "{indent+1} return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<void *, ResultType>::CanCast;\n"
				.	 "\n"
		,
		canFromRef => "{indent} case {vt} | byReference:\n"
				.	 "{indent+1} return variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;\n"
				.	 "\n"
		,
	},

	{ vt => "vtObject", type => "void *", value => "valueObject", ptr => "ptrObject",
		fromValue => "{indent} case {vt}:\n"
				.	 "{indent+1} return *variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::cast(v.data.{value});\n"
				.	 "\n"
		,
		fromPtr => "{indent} case {vt} | byPointer:\n"
				.	 "{indent+1} return variant_internal::CastVariantHelper<void *, ResultType>::cast(const_cast<void *>(v.data.{ptr}));\n"
				.	 "\n"
		,
		fromRef => "{indent} case {vt} | byReference:\n"
				.	 "{indent+1} return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::cast(v.data.{ptr}));\n"
				.	 "\n"
		,
		canFromValue => "{indent} case {vt}:\n"
				.	 "{indent+1} return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;\n"
				.	 "\n"
		,
		canFromPtr => "{indent} case {vt} | byPointer:\n"
				.	 "{indent+1} return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<void *, ResultType>::CanCast;\n"
				.	 "\n"
		,
		canFromRef => "{indent} case {vt} | byReference:\n"
				.	 "{indent+1} return variant_internal::isNonPointer<RefValueType>() && variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;\n"
				.	 "\n"
		,
	},

	{ vt => "vtShadow", type => "void *", value => "shadowObject", ptr => "#",
		fromValue => "{indent} case {vt}:\n"
				.	 "{indent+1} return *variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::cast(v.data.shadowObject->getObject());\n"
				.	 "\n"
		,
		fromPtr => undef,
		fromRef => undef,
		canFromValue => "{indent} case {vt}:\n"
				.	 "{indent+1} return variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;\n"
				.	 "\n"
		,
		canFromPtr => undef,
		canFromRef => undef,
	},
);

my $currentIndent = 0;

&doMain;

sub doMain
{
	print &generateCastFromVariant;
	
	print "\n\n";
	print "====================";
	print "\n\n";

	print &generateCanCastFromVariant;
}

sub indentText
{
	return "\t" x $currentIndent;
}

sub indent
{
	++$currentIndent;
}

sub unindent
{
	--$currentIndent;
}

sub replaceText
{
	my ($type, $text) = @_;

	$text =~ s/\{vt\}/$type->{vt}/sg;
	$text =~ s/\{type\}/$type->{type}/sg;
	$text =~ s/\{value\}/$type->{value}/sg;
	$text =~ s/\{ptr\}/$type->{ptr}/sg;

	my $s;

	$s = &indentText;
	$text =~ s/\{indent\}\s*/$s/sg;
	
	&indent;
	$s = &indentText;
	$text =~ s/\{indent\s*\+\s*1\}\s*/$s/sg;
	&unindent;
	
	return $text;
}


sub generateCanCastFromVariantXXX
{
	my $text = &generateCastFromVariant;

	my @lines = split("\n", $text);

	foreach(@lines) {
		s/^(\s*).*(variant_internal::CastVariantHelper\s*\<.+\>\s*::).*/$1return $2CanCast;/;
	}

	$text = join("\n", @lines);

	return $text;
}

sub generateCanCastFromVariant
{
	my $result = '';

	&indent;
	&indent;
	&indent;

	foreach(@types) {
		$result .= &replaceText($_, $_->{canFromValue}) if(defined($_->{canFromValue}));
	}
	
	foreach(@types) {
		$result .= &replaceText($_, $_->{canFromPtr}) if(defined($_->{canFromPtr}));
	}
	
	foreach(@types) {
		$result .= &replaceText($_, $_->{canFromRef}) if(defined($_->{canFromRef}));
	}
	
	&unindent;
	&unindent;
	&unindent;

	return $result;
}

sub generateCastFromVariant
{
	my $result = '';

	&indent;
	&indent;
	&indent;

	foreach(@types) {
		$result .= &replaceText($_, $_->{fromValue});
	}
	
	foreach(@types) {
		$result .= &replaceText($_, $_->{fromPtr}) if(defined($_->{fromPtr}));
	}
	
	foreach(@types) {
		$result .= &replaceText($_, $_->{fromRef}) if(defined($_->{fromRef}));
	}
	
	&unindent;
	&unindent;
	&unindent;

	return $result;
}

