use strict;
use warnings;

my @itemList = (
	{
		vt => 'vtBool',
		type => 'bool',
		value => 'valueInt',
	},

	{
		vt => 'vtChar',
		type => 'char',
		value => 'valueInt',
	},

	{
		vt => 'vtWchar',
		type => 'wchar_t',
		value => 'valueInt',
	},

	{
		vt => 'vtSignedChar',
		type => 'signed char',
		value => 'valueInt',
	},

	{
		vt => 'vtUnsignedChar',
		type => 'unsigned char',
		value => 'valueInt',
	},

	{
		vt => 'vtSignedShort',
		type => 'signed short',
		value => 'valueInt',
	},

	{
		vt => 'vtUnsignedShort',
		type => 'unsigned short',
		value => 'valueInt',
	},

	{
		vt => 'vtSignedInt',
		type => 'signed int',
		value => 'valueInt',
	},

	{
		vt => 'vtUnsignedInt',
		type => 'unsigned int',
		value => 'valueInt',
	},

	{
		vt => 'vtSignedLong',
		type => 'signed long',
		value => 'valueInt',
	},

	{
		vt => 'vtUnsignedLong',
		type => 'unsigned long',
		value => 'valueInt',
	},

	{
		vt => 'vtSignedLongLong',
		type => 'signed long long',
		value => 'valueInt',
	},

	{
		vt => 'vtUnsignedLongLong',
		type => 'unsigned long long',
		value => 'valueInt',
	},

	{
		vt => 'vtFloat',
		type => 'float',
		value => 'valueFloat',
	},

	{
		vt => 'vtDouble',
		type => 'double',
		value => 'valueDouble',
	},

	{
		vt => 'vtLongDouble',
		type => 'long double',
		value => 'valueLongDouble',
	},

);

sub doFormat
{
	my ($s) = @_;
	
	$s =~ s/\n/ /g;
	$s =~ s/\n\s*\n/\n/g;
	$s =~ s/([^\s])[ \t]+/$1 /g;
	$s .= "\n";
	
	return $s;
}

sub makeTypeList
{
	my ($type, $symbol) = @_;
	
	return "cpgf::GTypeList<const $type $symbol, $type $symbol, const volatile $type $symbol, volatile $type $symbol>";
}

sub makeReferenceToPointerTypeList
{
	my ($type, $symbol) = @_;
	
	return <<EOM;
				cpgf::GTypeList<
					const $type * $symbol, $type * $symbol, const volatile $type * $symbol, volatile $type * $symbol,
					const $type * const $symbol, $type * const $symbol, const volatile $type * const $symbol, volatile $type * const $symbol,
					const $type * volatile $symbol, $type * volatile $symbol, const volatile $type * volatile $symbol, volatile $type * volatile $symbol,
					const $type * const volatile $symbol, $type * const volatile $symbol, const volatile $type * const volatile $symbol, volatile $type * const volatile $symbol
				>
EOM
}

sub doCommon
{
	my ($item) = @_;

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<$item->{type}> >(data.$item->{value});

EOM
);
}

sub doCanCommon
{
	my ($item) = @_;

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return TypeListConvertible<cpgf::GTypeList<$item->{type}>, typename HelperValueType<ResultType>::type>::convertible;

EOM
);
}

sub doPointer
{
	my ($item) = @_;

	my $typeList = &makeTypeList($item->{type}, '*');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return (ResultType)helperFromVariant<
				ResultType,
				$typeList
			>(data.pointer);

EOM
);
}

sub doCanPointer
{
	my ($item) = @_;

	my $typeList = &makeTypeList($item->{type}, '*');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return TypeListConvertible<$typeList, ResultType>::convertible;

EOM
);
}

sub doLvalueReference
{
	my ($item) = @_;

	my $typeListPointer = &makeTypeList($item->{type}, '*');
	my $typeListReference = &makeTypeList($item->{type}, '&');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return (ResultType)helperFromPointerOrReference<
				ResultType,
				$typeListPointer,
				$typeListReference
			>(($item->{type} &) * ($item->{type} *)data.pointer);

EOM
);
}

sub doCanLvalueReference
{
	my ($item) = @_;

	my $typeListPointer = &makeTypeList($item->{type}, '*');
	my $typeListReference = &makeTypeList($item->{type}, '&');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return TypeListConvertible<$typeListPointer, ResultType>::convertible
				|| TypeListConvertible<$typeListReference, ResultType>::convertible
			;

EOM
);
}

sub doPointerLvalueReference
{
	my ($item) = @_;
	
	my $typeList = &makeReferenceToPointerTypeList($item->{type}, '&');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return (ResultType)helperFromVariant<
				ResultType,
$typeList
			>(($item->{type} * &) * ($item->{type} **)data.pointer);

EOM
);
}

sub doCanPointerLvalueReference
{
	my ($item) = @_;
	
	my $typeList = &makeReferenceToPointerTypeList($item->{type}, '&');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return TypeListConvertible<$typeList, ResultType>::convertible;

EOM
);
}

sub doRvalueReference
{
	my ($item) = @_;

	my $typeListPointer = &makeTypeList($item->{type}, '*');
	my $typeListReference = &makeTypeList($item->{type}, '&&');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return (ResultType)helperFromPointerOrReference<
				ResultType,
				$typeListPointer,
				$typeListReference
			>(($item->{type} &&) * ($item->{type} *)data.pointer);

EOM
);
}

sub doCanRvalueReference
{
	my ($item) = @_;

	my $typeListPointer = &makeTypeList($item->{type}, '*');
	my $typeListReference = &makeTypeList($item->{type}, '&&');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return TypeListConvertible<$typeListPointer, ResultType>::convertible
				|| TypeListConvertible<$typeListReference, ResultType>::convertible
			;

EOM
);
}

sub doPointerRvalueReference
{
	my ($item) = @_;

	my $typeList = &makeReferenceToPointerTypeList($item->{type}, '&&');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return (ResultType)helperFromVariant<
				ResultType,
$typeList
			>(($item->{type} * &&) * ($item->{type} **)data.pointer);

EOM
);
}

sub doCanPointerRvalueReference
{
	my ($item) = @_;

	my $typeList = &makeReferenceToPointerTypeList($item->{type}, '&&');

	print &doFormat(<<EOM
		case GVariantType::$item->{vt}:
			return TypeListConvertible<$typeList, ResultType>::convertible;

EOM
);
}


# cast

print "\n CastVariant_Value =======\n\n";
foreach my $item (@itemList) { &doCommon($item); }

print "\n CastVariant_Pointer =======\n\n";
foreach my $item (@itemList) { &doPointer($item); }

print "\n CastVariant_LvalueReference =======\n\n";
foreach my $item (@itemList) { &doLvalueReference($item); }

print "\n CastVariant_Pointer_LvalueReference =======\n\n";
foreach my $item (@itemList) { &doPointerLvalueReference($item); }

print "\n CastVariant_RvalueReference =======\n\n";
foreach my $item (@itemList) { &doRvalueReference($item); }

print "\n CastVariant_Pointer_RvalueReference =======\n\n";
foreach my $item (@itemList) { &doPointerRvalueReference($item); }

# can cast

print "\n CastVariant_Value canCast =======\n\n";
foreach my $item (@itemList) { &doCanCommon($item); }

print "\n CastVariant_Pointer canCast =======\n\n";
foreach my $item (@itemList) { &doCanPointer($item); }

print "\n CastVariant_LvalueReference canCast =======\n\n";
foreach my $item (@itemList) { &doCanLvalueReference($item); }

print "\n CastVariant_Pointer_LvalueReference canCast =======\n\n";
foreach my $item (@itemList) { &doCanPointerLvalueReference($item); }

print "\n CastVariant_RvalueReference canCast =======\n\n";
foreach my $item (@itemList) { &doCanRvalueReference($item); }

print "\n CastVariant_Pointer_RvalueReference canCast =======\n\n";
foreach my $item (@itemList) { &doCanPointerRvalueReference($item); }

print "\n\n";
