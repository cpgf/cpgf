use strict;
use warnings;

my @cheatSheets = (
	{
		name => 'Field',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
		<pre>
template &lt;typename FT&gt;
void reflectField(const std::string &amp; name, FT field);
		</pre>
		Example:
		<pre>
reflectField(&quot;width&quot;, &amp;MyClass:width);
		</pre>
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
		<pre>
GMETA_FIELD(field);
		</pre>
		Example:
		<pre>
GMETA_FIELD(width);
		</pre>
EOM
			},

			{
				title => 'Get value',
				content => <<EOM,
		<pre>
int n = fromVariant<int>(metaField->get(obj)); // get as integer
std::string s = fromVariant<std::string>(metaField->get(obj)); // get as std::string
		</pre>
EOM
			},
		
			{
				title => 'Set value',
				content => <<EOM,
		<pre>
metaField->set(obj, 5); // set as integer, 5 will be converted to appropriate type
metaField->set(obj, "hello"); // set as std::string, or C string
		</pre>
EOM
			},
		
			{
				title => 'Get field address',
				content => <<EOM,
		<pre>
void * address = metaField->getAddress(obj);
		</pre>
EOM
			},
		
			{
				title => 'Get field size',
				content => <<EOM,
		<pre>
size_t size = metaField->getFieldSize();
		</pre>
EOM
			},

			{
				title => 'Get field type',
				content => <<EOM,
		<pre>
const GMetaType & type = metaField->getType();
		</pre>
EOM
			},
		
		],
	},

	{
		name => 'Property',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
		<pre>
template &lt;typename Getter, typename Setter&gt;
void reflectProperty(const std::string &amp; name,
	const Getter &amp; getter, const Setter &amp; setter);
		</pre>
		Example:
		<pre>
reflectProperty(&quot;width&quot;, &amp;MyClass::getWidth, &amp;MyClass::setWidth);
		</pre>
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
		<pre>
GMETA_PROPERTY(prop, getter, setter)
		</pre>
		Example:
		<pre>
GMETA_PROPERTY(width, getWidth, setWidth)
		</pre>
EOM
			},

			{
				title => 'Get value',
				content => <<EOM,
		<pre>
int n = fromVariant<int>(metaProperty->get(obj)); // get as integer
std::string s = fromVariant<std::string>(metaProperty->get(obj)); // get as std::string
		</pre>
EOM
			},
		
			{
				title => 'Set value',
				content => <<EOM,
		<pre>
metaProperty->set(obj, 5); // set as integer, 5 will be converted to appropriate type
metaProperty->set(obj, "hello"); // set as std::string, or C string
		</pre>
EOM
			},
		
			{
				title => 'Get property size',
				content => <<EOM,
		<pre>
size_t size = metaProperty->getPropertySize();
		</pre>
EOM
			},

			{
				title => 'Get property type',
				content => <<EOM,
		<pre>
const GMetaType & type = metaProperty->getType();
		</pre>
EOM
			},
		
		],
	},

	{
		name => 'Method',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
		<pre>
template &lt;typename FT&gt;
void reflectMethod(const std::string &amp; name, FT func);
		</pre>
		Example:
		<pre>
reflectMethod(&quot;checkSum&quot;, &amp;MyClass:checkSum);
		</pre>
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
		<pre>
GMETA_METHOD(method)
		</pre>
		Example:
		<pre>
GMETA_METHOD(checkSum)
		</pre>
EOM
			},

			{
				title => 'Invoking',
				content => <<EOM,
		<pre>
// the method accepts three parameters, integer, boolean, and string
// the method return an integer
int n = fromVariant&lt;int&gt;(metaMethod-&gt;invoke(obj, 3, false, &quot;abc&quot;));
		</pre>
EOM
			},

			{
				title => 'Examine parameters and result type',
				content => <<EOM,
		<pre>
// get parameter count
size_t paramCount = metaMethod-&gt;getParamCount();

// get first parameter type
GMetaType type = metaMethod-&gt;getParamType(0);

// get result type
GMetaType resultType = metaMethod-&gt;getResultType();
		</pre>
EOM
			},
		],
	},

	{
		name => 'Constructor',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
		<pre>
template &lt;typename Signature&gt;
void reflectConstructor();
		</pre>
		Example:
		<pre>
reflectConstructor&lt;void *(int, const std::string &amp;)&gt;();
		</pre>
		Note: the return type must always be "void *", can't be any other type.
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
Not available
EOM
			},
			
			{
				title => 'Construct an object',
				content => <<EOM,
		<pre>
// pass parameter 18 and "what" to contruct an object
// the object is returned as "obj"
void * obj = constructor->invoke(18, "what");
		</pre>
EOM
			},
		],
	},

	{
		name => 'Operator',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
		<pre>
template &lt;typename Signature, typename Creator&gt;
void reflectOperator(const Creator &amp; creator);
		</pre>
		Example:
		<pre>
reflectOperator&lt;MyClass (const GMetaSelf &amp;, int)&gt;(mopHolder + mopHolder);
		</pre>
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
Not available
EOM
			},
		],
	},

	{
		name => 'Class',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
		<pre>
void reflectClass(const GMetaClass * metaClass);
		</pre>
		Example:
		<pre>
reflectClass(findMetaClass<MyClass>());
		</pre>
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
		<pre>
GMETA_CLASS(cls)
		</pre>
		Example:
		<pre>
GMETA_CLASS(MyClass)
		</pre>
EOM
			},
		],
	},

	{
		name => 'Enumerator',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
			<pre>
reflectEnum<TestObject::BorderStyle>("BorderStyle")
	("bs0", TestObject::bs0)
	("bs1", TestObject::bs1)
	("bs2", TestObject::bs2)
;
			</pre>
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
			<pre>
GMETA_ENUM(BorderStyle, TestObject::bs0, TestObject::bs1, TestObject::bs2);
			</pre>
EOM
			},
		],
	},

	{
		name => 'Annotation',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
			<pre>
reflectAnnotation("attribute")
	("name", "What's the name")
	("count", 3)
	("data", TestData("blahblah", 5))
;
			</pre>
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
Not available
EOM
			},
		],
	},

	{
		name => 'Property',
		items => [
			{
				title => 'Reflect using function',
				content => <<EOM,
EOM
			},

			{
				title => 'Reflect using macro',
				content => <<EOM,
EOM
			},
		],
	},
);

&doMain;

sub doMain
{
	&genHeader;
	
	foreach(@cheatSheets) {
		&genSheet($_);
	}
	
	&genFooter;
}

sub genSheet
{
	my ($sheet) = @_;

	print '<a id="', &formatAnchor($sheet->{name}), '" >', "</a>\n";
	print <<EOM;
<table cols="1" border="1" style="width:100%">
<tr><td style="background-color:#dddddd">
<h3 style="text-align:center">$sheet->{name}</h3>
</td></tr>
EOM

	foreach(@{$sheet->{items}}) {
		my $item = $_;
		my $content = $item->{content};
		$content =~ s/\n[ \t]*\Z//msg;
		$content =~ s/\s+\<\/pre\>/<\/pre>/msg;
		print <<EOM;
<tr>
<td>
<p style="margin-top:20px">
	<b>$item->{title}</b>
</p>
<p>
	$content
</p>
</td>
</tr>
EOM
	}

	print <<EOM;
</table>
<p style="text-align:right">
<a href="#top">Back to top</a>
</p>
<br /><br />
EOM
}

sub genHeader
{
	print <<EOM;
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">
<link rel="stylesheet" type="text/css" href="maintpl.css" />
</head>
<body><div style="width:900px">

<h1>Quick reference -- the cheat sheet</h1>

<a id="top"></a>
<ul style="text-align:left">
EOM
	foreach(@cheatSheets) {
		my $sheet = $_;
		my $anchor = &formatAnchor($sheet->{name});
		print '<li><a href="#', $anchor, '">', $sheet->{name}, '</a>', "\n";
	}
	
	print '</ul>';
}

sub genFooter
{
	print <<EOM;
</div></body></html>
EOM
}

sub formatAnchor
{
	my ($anchor) = @_;
	
	$anchor = lc($anchor);
	
	$anchor =~ s/\s/_/sg;
	
	return $anchor;
}
