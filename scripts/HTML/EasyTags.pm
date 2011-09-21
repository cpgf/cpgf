=head1 NAME

HTML::EasyTags - Make well-formed XHTML or HTML 4 tags, lists

=cut

######################################################################

package HTML::EasyTags;
require 5.004;

# Copyright (c) 1999-2003, Darren R. Duncan.  All rights reserved.  This module
# is free software; you can redistribute it and/or modify it under the same terms
# as Perl itself.  However, I do request that this copyright information and
# credits remain attached to the file.  If you modify this module and
# redistribute a changed version then please attach a note listing the
# modifications.  This module is available "as-is" and the author can not be held
# accountable for any problems resulting from its use.

use strict;
use warnings;
use vars qw($VERSION $AUTOLOAD);
$VERSION = '1.071';

######################################################################

=head1 DEPENDENCIES

=head2 Perl Version

	5.004

=head2 Standard Modules

	I<none>

=head2 Nonstandard Modules

	I<none>

=head1 SYNOPSIS

	use HTML::EasyTags;

	my $html = HTML::EasyTags->new();
	$html->groups_by_default( 1 );

	print
		$html->start_html( 
			'This Is My Page',
			$html->style( { type => 'text/css' }, 
				$html->comment_tag( <<__endquote ) ),
	\nbody {
		background-color: #ffffff; 
		background-image: none;
	}
	__endquote
		),
		$html->h1( 'A Simple Example' ),
		$html->p( 
			"Click " . 
			$html->a( href => 'http://search.cpan.org', text => 'here' ) . 
			" for more."
		),
		$html->hr,
		$html->table(
			$html->tr( [
				$html->th( [ 'Name', 'Count', 'URL', 'First Access' ] ), 
				$html->td( [ 'Old Page', 33, 'http://www.domain.com', 
					'1999/04/23 13:55:02' ] )
			] )
		),
		$html->hr,
		$html->form_start( method => 'post', action => 'http://localhost' ),
		$html->p( 
			"What's your name? " . 
			$html->input( type => 'text', name => 'name' ) 
		),
		$html->p( 
			"What's the combination?" .
			$html->input_group( 
				-type => 'checkbox', 
				-name => 'words',
				-value => ['eenie', 'meenie', 'minie', 'moe'],
				-checked => [1, 0, 1, 0],
				-text => ['Eenie', 'Meenie', 'Minie', 'Moe'] ),
		),
		$html->p( 
			"What's your favorite colour? " .
			$html->select_start( -size => 1, -name => 'color' ) .
			$html->option_group(
				-value => ['red', 'green', 'blue', 'chartreuse'], 
				-text => ['Red', 'Green', 'Blue', 'Chartreuse'] ) .
			$html->select_end
		),
		$html->input( type => 'submit' ),
		$html->form_end,
		$html->end_html;

=head1 DESCRIPTION

This Perl 5 object class can be used to generate any well-formed XHTML or HTML
tags in a format that is consistent with the W3C XHTML 1.0 or HTML 4.01
standards.  See B<http://www.w3.org/TR/xhtml1> and B<http://www.w3c.org/MarkUp>
for references.  There are no restrictions on what tags are named, however; you
can ask for any new or unsupported tag that comes along from Netscape or
Microsoft, and it will be made.  Additionally, you can generate lists of said
tags with one method call, or just parts of said tags (but not both at once).

This module's purpose is to be lightweight, easy to use, and whose results are
well-formed and pretty-printed (should humans wish to read or debug
it).  At the same time, it is supportive of your existing knowledge of HTML and 
as such its interface closely mirrors the actual appearance of the resulting 
tags.  This means that methods have the same name as the actual tags, and named
parameters that you pass correspond directly to  the tag attributes produced.
This module saves you having to remember the little details on formatting.  For
your convenience, a majority of the methods and their arguments are
backwards-compatible with those in CGI.pm, but you are saved 200K of code size.

As a reference, I strongly recommend that you check out B<Kevin Werbach's>
excellent "The Bare Bones Guide to HTML", which is available at
B<http://werbach.com/barebones/>.  I found this document invaluable when making
this module, as it provides a comprehensive list of all the HTML tags along with
their formatting and extensions.

In this implementation, "well formed" means that tags are made as pairs by
default, which look like "<tag></tag>", unless they are known to be "no pair"
tags, in which case they look like "<tag />".  Tags that I know to be "no pair"
are [basefont, img, area, param, br, hr, input, option, tbody, frame, comment,
isindex, base, link, meta].  However, you can force any tag to be "pair" or
"start only" or "end only" by appropriately modifying your call to the tag making
method.

Also, "well formed" means that tag attributes are formatted as "key=value". While
the HTML standard allowed there to be "no value" attributes, XHTML does not. 
These were formatted simply as "key" because their very presence indicates
positive assertion, while their absense means otherwise.  Before release 1-06,
attributes that were known to be "no value" attributes were formatted as "key" by
default.  Modifiers that I know to be "no value" are [ismap, noshade, compact,
checked, multiple, selected, nowrap, noresize, param].  As of release 1-06, "no
value" attributes are formatted as 'key="1"' when they are true and they are
absent when false, to keep backwards compatability.  "Well formed" means that 
attribute values will always become bounded by quotes, which ensures they work 
with both string and numerical quantities (eg: key="value").

Convenience methods start_html() and end_html() are provided to generate the 
required HTML that appears above and below your content; however, you can still 
make said HTML one tag at a time if you wish.

=head1 HTML CODE FROM SYNOPSIS PROGRAM

	<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN">
	<html>
	<head>
	<title>This Is My Page</title>
	<style type="text/css">
	<!-- 
	body {
		background-color: #ffffff; 
		background-image: none;
	}
	 --></style>
	</head>
	<body>
	<h1>A Simple Example</h1>
	<p>Click 
	<a href="http://search.cpan.org">here</a> for more.</p>
	<hr />
	<table>
	<tr>
	<th>Name</th>
	<th>Count</th>
	<th>URL</th>
	<th>First Access</th></tr>
	<tr>
	<td>Old Page</td>
	<td>33</td>
	<td>http://www.domain.com</td>
	<td>1999/04/23 13:55:02</td></tr></table>
	<hr />
	<form method="post" action="http://localhost">
	<p>What's your name? 
	<input type="text" name="name" /></p>
	<p>What's the combination?
	<input type="checkbox" name="words" checked="1" value="eenie" />Eenie
	<input type="checkbox" name="words" value="meenie" />Meenie
	<input type="checkbox" name="words" checked="1" value="minie" />Minie
	<input type="checkbox" name="words" value="moe" />Moe</p>
	<p>What's your favorite colour? 
	<select name="color" size="1">
	<option value="red" />Red
	<option value="green" />Green
	<option value="blue" />Blue
	<option value="chartreuse" />Chartreuse
	</select></p>
	<input type="submit" />
	</form>
	</body>
	</html>

=cut

######################################################################

# Names of properties for objects of this class are declared here:
my $KEY_AUTO_GROUP = 'auto_group';  # do we make tag groups by default?
my $KEY_PROLOGUE = 'prologue';  # special prologue starting every html page

# These extra tag properties work only with AUTOLOAD:
my $PARAM_TEXT = 'text';  #tag pair is wrapped around this
my $PARAM_LIST = 'list';  #force tag groups to be returned in ARRAY ref

# Constant values used in this class go here:

my $TAG_GROUP = 'group';  # values that "what_to_make" can have
my $TAG_PAIR  = 'pair';  # this one looks like "<tag></tag>"
my $TAG_START = 'start';  # this one looks like "<tag>"
my $TAG_END   = 'end';  # this one looks like "</tag>"
my $TAG_MINI  = 'mini';  # this one stands for "minimized" or "<tag />"

my %NO_PAIR_TAGS = (  # comments correspond to Bare Bones sections
	basefont => 1,   # PRESENTATION FORMATTING
	img => 1,   # LINKS, GRAPHICS, AND SOUNDS
	area => 1,   # LINKS, GRAPHICS, AND SOUNDS
	param => 1,   # LINKS, GRAPHICS, AND SOUNDS
	br => 1,   # DIVIDERS
	hr => 1,   # DIVIDERS
	input => 1,   # FORMS
	option => 1,   # FORMS
	tbody => 1,   # TABLES
	frame => 1,   # FRAMES
	comment => 1,   # MISCELLANEOUS
	isindex => 1,   # MISCELLANEOUS
	base => 1,   # MISCELLANEOUS
	'link' => 1,   # MISCELLANEOUS
	meta => 1,   # MISCELLANEOUS
);

my %NO_VALUE_PARAMS = (  # comments correspond to Bare Bones sections
	ismap => 1,   # LINKS, GRAPHICS, AND SOUNDS
	noshade => 1,   # DIVIDERS
	compact => 1,   # LISTS
	checked => 1,   # FORMS
	multiple => 1,   # FORMS
	selected => 1,   # FORMS
	nowrap => 1,   # TABLES
	noresize => 1,   # FRAMES
	param => 1,   # SCRIPTS AND JAVA
);

my %PARAMS_PRECEDENCE = (   # larger number means goes first; undef last
	method => 190,
	action => 185,
	type => 180,
	name => 175,
	width => 170,
	height => 165,
	rows => 160,
	cols => 155,
	border => 150,
	cellspacing => 145,
	cellpadding => 140,
	multiple => 135,
	checked => 130,
	selected => 125,
	value => 120,
	target => 115,
	rev => 113,
	rel => 112,
	href => 110,
	src => 105,
	alt => 100,
);

######################################################################

=head1 SYNTAX

This class does not export any functions or methods, so you need to call them
using object notation.  This means using B<Class-E<gt>function()> for functions
and B<$object-E<gt>method()> for methods.  If you are inheriting this class for
your own modules, then that often means something like B<$self-E<gt>method()>. 

Methods of this class always "return" their results, rather than printing them
out to a file or the screen.  Not only is this simpler, but it gives the calling
code the maximum amount of control over what happens in the program.  They may
wish to do post-processing with the generated HTML, or want to output it in a
different order than it is generated.  By default, all results are returned as a
scalar, but methods which generate a list of tags can optionally return an ARRAY
ref, with each element containing a single tag.  This can aid in post-processing
and possibly speed up the program because there is less copying done.

Through the magic of autoloading, this class can make any html tag by calling a
class method with the same name as the tag you want.  For examples, use "hr()" to
make a "<hr />" tag, or "p('text')" to make "<p>text</p>".  This also means that if
you mis-spell any method name, it will still make a new tag with the mis-spelled
name.  For autoloaded methods only, the method names are case-insensitive.

If you call a class method whose name ends in either of ['_start', '_end',
'_pair', '_mini'], this will be interpreted as an instruction to make just part
of one tag whose name are the part of the method name preceeding that suffix. 
For example, calling "p_start( 'text' )" results in "<p>text" rather than
"<p>text</p>". Similarly, calling "p_end()" will generate a "</p>" only.  Using
the '_pair' suffix will force tags to be made as a pair, whether or not they
would do so naturally.  For example, calling "br_pair" would produce a
"<br></br>" rather than the normal "<br />".  Calling "p_mini( 'text' )" results
in "<p />text".  When using either of ['_start', '_pair', '_mini'], the arguments
you pass the method are exactly the same as the unmodified method would use, and
there are no other symantec differences.  However, when using the '_end' suffix,
any arguments are ignored, as the latter member of a tag pair never carries any
attributes anyway.

If you call a class method whose name ends in "_group", this will be interpreted
as an instruction to make a list of tags whose name are the part of the method
name preceeding the "_group".  For example, calling "td_group(
['here','we','are'] )" results in "<td>here</td><td>we</td><td>are</td>" being
generated.  The arguments that you call this method are exactly the same as for
calling a method to make a single tag of the same name, except that the extra
optional parameter "list" can be used to force an ARRAY ref of the new tags to be
returned instead of a scalar.  The symantec difference is that any arguments
whose values are ARRAY refs are interpreted as a list of values where each one is
used in a separate tag; for a single tag, the literal ARRAY ref itself would be
used.  The number of tags produced is equal to the length of the longest ARRAY
ref passed as an argument.  For any other arguments who have fewer than this
count, their last value is replicated and appended enough times as necessary to
make them the same length.  The value of a scalar argument is used for all the
tags.  For example, calling "input_group( type => checkbox, name => 'letters',
value => ['a','b','c'] )" produces '<INPUT TYPE="checkbox" NAME="letters"
VALUE="a"><INPUT TYPE="checkbox" NAME="letters" VALUE="b"><INPUT TYPE="checkbox"
NAME="letters" VALUE="c">'.

All autoloaded methods require their parameters to be in named format.  These
names and values correspond to attribute names and values for the new tags. 
Since "no value" attributes are essentially booleans, they can have any true or
false value associated with them in the parameter list, which won't be printed. 
If an autoloaded method is passed exactly one parameter, it will be interpreted
as the "text" that goes between the tag pair (<tag>text</tag>) or after "start
tags" (<tag>text).  The same result can be had explicitely by passing the named
parameter "text".  The names of any named parameters can upper or lower or 
mixed case, as is your preference, and optionally start with a "-".

All static (non-autoloaded) methods require positional parameters.  

=cut

######################################################################
# All HTML tags have a method of this class associated with them.

sub AUTOLOAD {
	my $self = shift( @_ );
	$AUTOLOAD =~ m/([^:]*)$/;   # we don't need fully qualified name
	my $called_sub_name = $1;

	# Autoloaded subroutines are in the form "tagname_part" or "tagname_group" 
	# where "tagname" is the literal html tag to make and "part" says to force 
	# a certain form of the tag; "group" says make several tags in standard form.

	my ($tag_name, $what_to_make) = split( '_', lc( $called_sub_name ), 2 );
	$what_to_make ||= '';

	# Determine what part of the html tag to make.

	unless( $what_to_make =~ 
			/^($TAG_GROUP|$TAG_PAIR|$TAG_START|$TAG_END|$TAG_MINI)$/ ) {
		if( $self->{$KEY_AUTO_GROUP} ) {
			$what_to_make = $TAG_GROUP;
		} else {
			$what_to_make = '';
		}
	}

	# Fetch our arguments, which may be in a variety of formats, and extract 
	# special ones so that they are treated different than others.

	my $rh_params = $self->_params_to_hash( \@_ );
	my $ra_text = delete( $rh_params->{$PARAM_TEXT} );  # visible text
	my $force_list = delete( $rh_params->{$PARAM_LIST} );  # keep tags separate

	# Here we make a group of related html tags; the group may have one member

	if( $what_to_make eq $TAG_GROUP ) {
		return( $self->make_html_tag_group( 
			$tag_name, $rh_params, $ra_text, $force_list ) );
	}

	# Here we make a single html tag, or parts of one

	return( $self->make_html_tag( 
		$tag_name, $rh_params, $ra_text, $what_to_make ) );
}

# This is provided so AUTOLOAD isn't called instead.
sub DESTROY {}

######################################################################

=head1 FUNCTIONS AND METHODS

Note that all the methods defined below are static, so information specific to
autoloaded methods won't likely apply to them.  All of these methods take
positional arguments.

=head2 new()

This function creates a new HTML::EasyTags object (or subclass thereof) and 
returns it.

=cut

######################################################################

sub new {
	my $class = shift( @_ );
	my $self = bless( {}, ref($class) || $class );
	$self->initialize( @_ );
	return( $self );
}

######################################################################

=head2 initialize()

This method is used by B<new()> to set the initial properties of an object,
that it creates.  All page attributes are wiped clean, resulting in an empty
page.

=cut

######################################################################

sub initialize {
	my $self = shift( @_ );
	$self->{$KEY_AUTO_GROUP} = 0;
	$self->{$KEY_PROLOGUE} = 
		"\n".'<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN">';
}

######################################################################

=head2 clone([ CLONE ])

This method initializes a new object to have all of the same properties of the
current object and returns it.  This new object can be provided in the optional
argument CLONE (if CLONE is an object of the same class as the current object);
otherwise, a brand new object of the current class is used.  Only object 
properties recognized by HTML::EasyTags are set in the clone; other properties 
are not changed.

=cut

######################################################################

sub clone {
	my ($self, $clone, @args) = @_;
	ref($clone) eq ref($self) or $clone = bless( {}, ref($self) );

	$clone->{$KEY_AUTO_GROUP} = $self->{$KEY_AUTO_GROUP};
	$clone->{$KEY_PROLOGUE} = $self->{$KEY_PROLOGUE};

	return( $clone );
}

######################################################################

=head2 groups_by_default([ VALUE ])

This method is an accessor for the boolean "automatic grouping" property of this
object, which it returns.  If VALUE is defined, this property is set to it.  In
cases where we aren't told explicitely that autoloaded methods are making a
single or multiple tags (using ['_start', '_end', '_pair', '_mini'] and '_group'
respectively), we look to this property to determine what operation we guess. 
The default is "single".  When this property is true, we can make both single and
groups of tags by using a suffix-less method name; however, making single tags
this way is slower than when this property is false.  Also, be aware that when we
are making a "group", arguments that are ARRAY refs are always flattened, and
when we are making a "single", ARRAY ref arguments are always used literally.

=cut

######################################################################

sub groups_by_default {
	my $self = shift( @_ );
	if( defined( my $new_value = shift( @_ ) ) ) {
		$self->{$KEY_AUTO_GROUP} = $new_value;
	}
	return( $self->{$KEY_AUTO_GROUP} );
}

######################################################################

=head2 prologue_tag([ VALUE ])

This method is an accessor for the scalar "prologue tag" property of this object,
which it returns.  If VALUE is defined, this property is set to it. This property
is meant to be used literally and be the very first thing in an XHTML or HTML
document.  It tells the web browser such things as what version of the HTML
standard we are adhering to.  Citing backwards compatability with earlier
versions of this class, the default prologue tag we make is for HTML version 4.0
and looks like '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN">'.

=cut

######################################################################

sub prologue_tag {
	my $self = shift( @_ );
	if( defined( my $new_value = shift( @_ ) ) ) {
		$self->{$KEY_PROLOGUE} = $new_value;
	}
	return( $self->{$KEY_PROLOGUE} );
}

######################################################################

=head2 comment_tag( TEXT )

This method returns a comment tag, which is only visible to people viewing the
HTML source of a document, and not otherwise.  It can take either a scalar or a
list or an Array ref as its TEXT argument.  If a single item of text is passed,
then a comment tag that looks like "<!-- text -->" is made.  If more than one
item of text is passed, then a multi-line comment is made, which has each item of
text on its own line and indented with a single tab.  The latter is suitable for
displaying CSS or JavaScript code in an elegant manner.

=cut

######################################################################

sub comment_tag {
	my $self = shift( @_ );
	my @text = (ref($_[0]) eq 'ARRAY') ? @{$_[0]} : @_;
	if( scalar( @text ) <= 1 ) {
		return( "\n<!-- @text -->" );
	} else {
		return( "\n<!-- \n\t".join( "\n\t", @text )."\n -->" );
	}
}

######################################################################

=head2 make_html_tag( NAME[, PARAMS[, TEXT[, PART]]] )

This method is used internally to do the actual construction of single html tags.
 You can call it directly when you want faster code and/or more control over how
tags are made.  The first argument, NAME, is a scalar that defines the actual
name of the tag we are making (eg: 'br'); it is case-insensitive.  The optional
second argument, PARAMS, is a HASH ref containing attribute names and values for
the new tag; the names (keys) are case-insensitive.  The attribute values are all
printed literally, so they should be scalars.  The optional third argument, TEXT,
is a scalar containing the text that goes between the tag pairs; it is not a tag
attribute.  The optional fourth argument, PART, is a scalar which indicates we
should make just a certain part of the tag; acceptable values are ['pair',
'start', 'end'], and it is case-insensitive.  This method knows which HTML tags
are normally paired or not, which tag attributes take specified values or not,
and acts accordingly.

=cut

######################################################################

sub make_html_tag {
	my $self = shift( @_ );

	# Fetch our arguments

	my $tag_name = lc(shift( @_ ));  # lowercase to match our lookup tables
	my $rh_params = shift( @_ );
	my $text = shift( @_ ); 
	defined( $text ) or $text = '';  # avoid warnings
	my $what_to_make = lc(shift( @_ ));  # lowercase to match our lookup tables

	# Make sure our tag params argument is lowercased, to match our lookup tables

	my %tag_params = map { ( lc($_) => $rh_params->{$_} ) } 
		(ref($rh_params) eq 'HASH') ? (keys %{$rh_params}) : ();

	# Determine what part of the html tag to make

	unless( $what_to_make =~ /^($TAG_PAIR|$TAG_START|$TAG_END|$TAG_MINI)$/ ) {
		$what_to_make = $NO_PAIR_TAGS{$tag_name} ? $TAG_MINI : $TAG_PAIR;
	}

	# Shortcut - if we're making just an end tag, there are no args or text

	if( $what_to_make eq $TAG_END ) {
		return( "\n</$tag_name>" );
	}

	# Assemble the html tag attributes, ordered with more important on the left

	my $param_str = '';
	foreach my $param ( sort {
			($PARAMS_PRECEDENCE{$b} || 0) <=> ($PARAMS_PRECEDENCE{$a} || 0)
			} keys %tag_params ) {

		# Some tag attributes assert true simply by their names being present.
		# Therefore, omit these if their values are false.

		next if( $NO_VALUE_PARAMS{$param} and !$tag_params{$param} );

		# Show names of attributes that display with values or are true
		# Show values of attributes that display with values, in quotes

		$param_str .= " $param=\"$tag_params{$param}\"";
	}

	# Here we make just a minimized tag with attributes and text

	if( $what_to_make eq $TAG_MINI ) {
		return( "\n<$tag_name$param_str />$text" );
	}

	# Here we make just a start tag with attributes and text

	if( $what_to_make eq $TAG_START ) {
		return( "\n<$tag_name$param_str>$text" );
	}

	# Here we make both start and end tags, with attributes and text

	return( "\n<$tag_name$param_str>$text</$tag_name>" );
}

######################################################################

=head2 make_html_tag_group( NAME[, PARAMS[, TEXT[, LIST]]] )

This method is used internally to do the actual construction of html tag groups. 
You can call it directly when you want faster code and/or more control over how
tags are made.  The first argument, NAME, is a scalar that defines the actual
name of the tag we are making (eg: 'br'); it is case-insensitive.  The optional
second argument, PARAMS, is a HASH ref containing attribute names and values for
the new tag; the names (keys) are case-insensitive.  Any attribute values which
are ARRAY refs are flattened, and the number of tags made is determined by the
length of the longest one.  The optional third argument, TEXT, is a HASH ref (or
scalar) containing the text that goes between the tag pairs; it is not a tag
attribute, but if its an ARRAY ref then its length will influence the number of
tags that are made as the length of tag attribute arrays do.  The optional fourth
argument, LIST, is a boolean/scalar which indicates whether this method returns
the new tags in an ARRAY ref (one tag per element) or as a scalar (tags are
concatenated together); a true value forces an ARRAY ref, scalar is the default. 
This method knows which HTML tags are normally paired or not, which tag
attributes take specified values or not, and acts accordingly.

=cut

######################################################################

sub make_html_tag_group {
	my $self = shift( @_ );

	# Fetch our arguments

	my $tag_name = lc(shift( @_ ));  # lowercase to match our lookup tables
	my $rh_params = shift( @_ );
	my $text_in = shift( @_ );
	my $force_list = shift( @_ );

	# Make sure our tag params argument is lowercased, to match our lookup tables

	my %tag_params = map { ( lc($_) => $rh_params->{$_} ) } 
		(ref($rh_params) eq 'HASH') ? (keys %{$rh_params}) : ();

	# Prepare to normalize the count of text values with other attributes

	$tag_params{$PARAM_TEXT} = $text_in;

	# Ensure that all tag attribute values are in arrays for consistency.
	# Also, determine the maximum value count of any attribute.
	# This count determines the count of html tags that we will make.

	my $max_tag_ind = 0;
	foreach my $key (keys %tag_params) {
		my $ra_values = $tag_params{$key};
		unless( ref($ra_values) eq 'ARRAY' ) {
			$tag_params{$key} = [$ra_values];
			next;
		}
		if( $#{$ra_values} > $max_tag_ind ) {
			$max_tag_ind = $#{$ra_values};
		}
	}

	# Ensure that all tag attribute arrays are the same length by taking any 
	# which are shorter than the longest and extending them; the inserted values 
	# are copies of the value currently in the highest element of that array.

	foreach my $ra_values (values %tag_params) {
		my $last_value = $ra_values->[-1];
		push( @{$ra_values}, 
			map { $last_value } (($#{$ra_values} + 1)..$max_tag_ind) );
	}

	# Now put the text back where it belongs; its value count is now normalized

	my $ra_text = delete( $tag_params{$PARAM_TEXT} );

	# Get list of html tag attribute names, ordered with more important on left

	my @param_seq = sort { ($PARAMS_PRECEDENCE{$b} || 0)
		<=> ($PARAMS_PRECEDENCE{$a} || 0) } keys %tag_params;

	# Declare the destination variable we will output

	my @new_tags = ();

	# This loop iterates for the count of html tags we will make.
	# The loop counter, $index, is used to lookup elements in value arrays 
	# that go with each tag attribute and visible text for that tag.

	foreach my $index (0..$max_tag_ind) {
		my $param_str = '';

		# This loop iterates over the attributes that each tag would have.

		foreach my $param ( @param_seq ) {

			# Some tag attributes assert true simply by their names being present
			# Therefore, omit these if their values are false.

			next if( $NO_VALUE_PARAMS{$param} and 
				!$tag_params{$param}->[$index] );

			# Show names of attributes that display with values or are true
			# Show values of attributes that display with values, in quotes

			$param_str .= " $param=\"$tag_params{$param}->[$index]\"";
		}

		# Get the visible text for the tag.

		my $text = defined( $ra_text->[$index] ) ? $ra_text->[$index] : '';

		# Here we make just a minimized tag with attributes and text

		if( $NO_PAIR_TAGS{$tag_name} ) {
			push( @new_tags, "\n<$tag_name$param_str />$text" );

		# Here we make both start and end tags, with attributes and text

		} else {
			push( @new_tags, "\n<$tag_name$param_str>$text</$tag_name>" );
		}
	}

	# Return all of the new tags as either a list (array ref) or a single scalar

	return( $force_list ? \@new_tags : join( '', @new_tags ) );
}

######################################################################

=head2 start_html([ TITLE[, HEAD[, BODY[, FRAMESET]]] ])

This method returns a canned HTML template that is suitable for use as the top of
an HTML page.  It consists of the prologue tag (<!DOCTYPE...), the opening 'html'
tag, the entire 'head' section, and the opening 'body' tag.  The prologue tag
looks the same as that generated by the class method prologue_tag().  The first
optional argument, TITLE, is a scalar which defines the title for the document,
and its default value is 'Untitled Document'.  The second argument, HEAD, is an
Array ref (or scalar) containing anything else you would like to appear in the
'head' section; it is flattened and the elements used as-is.  The third argument,
BODY, is a Hash ref containing attributes and values for the opening 'body' tag.
As of release 1.05 this class supports frameset documents as well as normal HTML.
If the fourth argument, FRAMESET, is true, then it is assumed we will be making a
frameset document, whereby the body section is wrapped in a <noframes> pair and
there is a <frameset> pair between the head and noframes/body sections.  FRAMESET
is a Hash ref that contains the attributes and values that go in the opening
frameset tag (analogous to BODY argument), except that a 'text' attribute will be
resolved into the text that goes inside the frameset pair, as with any autoloaded
tag methods.  So the 'text' attribute of FRAMESET is where the "frame" tag html
that you previously rendered goes.  If FRAMESET is true but not a hash, it will
be taken literally as 'text' likewise as with autoloaded methods.

=cut

######################################################################

sub start_html {
	my ($self, $title, $ra_head, $rh_body, $rh_frameset) = @_;
	return( join( '',
		$self->prologue_tag(),
		$self->html_start(),
		$self->head_start(),
		$self->title( $title || 'Untitled Document' ),
		ref( $ra_head ) eq 'ARRAY' ? @{$ra_head} : ($ra_head || ''),
		$self->head_end(),
		$rh_frameset ? (
			$self->frameset( $rh_frameset ),
			$self->noframes_start(),
		) : (),
		$self->body_start( $rh_body ),
	) );
}

######################################################################

=head2 end_html([ FRAMESET ])

This method returns a canned HTML template that is suitable for use as the bottom
of an HTML page.  It consists of the closing 'body' and 'html' tags.  
As of release 1.05 this class supports frameset documents as well as normal HTML.  
If the optional argument FRAMESET is true, then there is a closing 'noframes' tag 
between the closing 'body' and 'html' tags.

=cut

######################################################################

sub end_html {
	my ($self, $is_frameset) = @_;
	return( join( '',
		$self->body_end(),
		$is_frameset ? $self->noframes_end() : (),
		$self->html_end(),
	) );
}

######################################################################
# _params_to_hash( ARGS )
# This private method cleans up the argument lists passed to autoloaded html 
# tag making methods and returns the named arguments in a hash.  Input arguments, 
# provided in ARGS, are usually in named format, and the names can be any case or 
# optionally begin with a "-".  We know that ARGS is named format if the first 
# element is a hash ref or there is more than one element; ARGS is not in named 
# format if there is only one element and it is not a hash ref.  In the second 
# case this single element is implicitely named 'text' and returned that way.
# If the first ARGS element is a hash and there are more elements, then the 
# second one is implicitely named 'text' and inserted into the returned hash.

sub _params_to_hash {
	my ($self, $ra_params_in) = @_;
	@{$ra_params_in} or return( {} );

	my %params_in = ();
	if( ref( $ra_params_in->[0] ) eq 'HASH' ) {
		%params_in = %{$ra_params_in->[0]};
		@{$ra_params_in} > 1 and 
			$params_in{$PARAM_TEXT} = $ra_params_in->[1];
	} elsif( @{$ra_params_in} == 1 ) {
		return( { $PARAM_TEXT => $ra_params_in->[0] } );
	} else {
		%params_in = @{$ra_params_in};
	}

	my %params_out = ();
	foreach my $key (sort keys %params_in) {
		my $value = $params_in{$key};
		substr( $key, 0, 1 ) eq '-' and $key = substr( $key, 1 );
		$params_out{lc( $key )} = $value;
	}

	delete( $params_out{''} );
	return( \%params_out );
}

######################################################################

1;
__END__

=head1 COMPARISONS WITH CGI.PM

The methods of this class and their parameters are designed to be compatible with
any same-named methods in the popular CGI.pm class. This class will produce
browser-compatible (and often identical) HTML from such methods, and this class
can accept all the same argument formats.  Exceptions to this include:

=over 4

=item 0

None of our methods are exported and must be called using object
notation, whereas CGI.pm can export any of it's methods.

=item 0

Autoloaded methods do not use the presence or absence of arguments to
decide whether to make the new tag as a pair or as "start only".  Also, 
CGI.pm does not do end-only tags.

=item 0

Autoloaded methods that make html tags won't concatenate their arguments
into a single argument under any circumstances, but in some cases the "shortcuts"
of CGI.pm will do so.

=item 0

We go further to make the generated HTML human-readable by: 1. having each
new tag start on a new line; 2. making all tag and attribute names lowercase; 3.
ensuring that about 20 often-used tag attributes always appear in the same order
(eg: 'type' is before 'name' is before 'value'), and before any others.

=item 0

Our textarea() method is autoloaded, and doesn't have the special symantecs
that CGI.pm's textarea() does.

=item 0

Our convenience method start_html() is very simple and only accepts the three
positional arguments ['title', 'head', 'body'].  Title is the most commonly used
argument by far, and you can easily replicate the effects of missing arguments by
making appropriate tags explicitely and passing them with the "head" argument.  
As of release 1.05, this method and end_html() also provided explicit frameset 
document support, and CGI.pm has no explicit frameset support.

=back

=head1 AUTHOR

Copyright (c) 1999-2003, Darren R. Duncan.  All rights reserved.  This module
is free software; you can redistribute it and/or modify it under the same terms
as Perl itself.  However, I do request that this copyright information and
credits remain attached to the file.  If you modify this module and
redistribute a changed version then please attach a note listing the
modifications.  This module is available "as-is" and the author can not be held
accountable for any problems resulting from its use.

I am always interested in knowing how my work helps others, so if you put this
module to use in any of your own products or services then I would appreciate
(but not require) it if you send me the website url for said product or
service, so I know who you are.  Also, if you make non-proprietary changes to
the module because it doesn't work the way you need, and you are willing to
make these freely available, then please send me a copy so that I can roll
desirable changes into the main release.

Address comments, suggestions, and bug reports to B<perl@DarrenDuncan.net>.

=head1 CREDITS

Thanks very much to B<Kevin Werbach> for publishing "The Bare Bones Guide to
HTML", which I found to be an invaluable resource when writing this module (and
at other times as well).  The latest version of the document is available at
B<http://werbach.com/barebones/>.

This quick reference lists all the HTML tags that current browsers are likely to
recognize, including all the elements of the official HTML 4.0 recommendation,
and some Netscape and Microsoft extensions as well.  Common attributes for these
tags are also included in context, giving a good idea on how they are used.

When writing this module, I used the Bare Bones reference to verify the
consistant formatting used by all HTML tags, including how tag attributes are
formatted.  I could see the proper formatting for prologue and comment tags as
well; their formats are unique compared to all the other tags.  The other main
uses I had for the document was in determining all the HTML tags which were not
used as a pair (most use pairs, few don't), and for determining which tag
attributes made a positive assertion just by their presence, without need for any
associated values (most have values, few don't).

Thanks to W3C for publishing their standards documents in an easy-to-understand 
manner.  I made good use of their XHTML primer document when making release 1-06 
of this module.  The most recent version is at "http://www.w3.org/TR/xhtml1".  
The full title is "XHTML(TM) 1.0: The Extensible HyperText Markup Language -- 
A Reformulation of HTML 4 in XML 1.0 -- W3C Recommendation 26 January 2000".  
I used this document to determine what changes I needed to make for this 
module's output to easily support the new XHTML standard as it supported HTML 4 
and previous versions before, while keeping backwards compatability.

Thanks to B<Lincoln D. Stein> for setting a good interface standard in the
HTML-related methods of his CGI.pm module.  I was heavily influenced by his
interfaces when designing my own.  Thanks also because I borrowed ideas for my
Synopsis program from his aforementioned module.

=head1 SEE ALSO

perl(1), HTML::FormTemplate, CGI::Portable, CGI::FormMagick, CGI.

=cut
