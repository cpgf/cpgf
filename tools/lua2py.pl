@replacements = (
	[ '\btrue\b', 'True' ],
	[ '\bfalse\b', 'False' ],
	[ '\belseif\b', 'elif' ],
	[ '\bimport\b', '_import' ],
	[ ' \.\. ', ' + ' ],
	[ '~=', '!=' ],
	[ '\bnil\b', 'None' ],
	[ '\bthen\b', ':' ],
	[ '\bfunction(\s+.*)$', 'def$1 :' ],
	[ '\blocal\s+', '' ],
	[ '^\s*end\s*$', '' ],
);
