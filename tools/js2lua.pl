@replacements = (
	[ '\bvar\b', 'local' ],
	[ '!=', '~=' ],
	[ '\bnull\b', 'nil' ],
	[ '\bnew\s+', '' ],
	[ '\s*\/\/.*$', '' ],
	[ '\s*\{\s*$', '' ],
	[ '^(\s*)\}\s*$', '$1end' ],
	[ '^(\s*)if\s*\((.*)\)\s*$', '$1if $2 then' ],
	[ '^(\s*)while\s*\((.*)\)\s*$', '$1while $2 do' ],
);
