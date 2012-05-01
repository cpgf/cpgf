package com.cpgf.metagen.codewriter;

public class CodeWriter {
	private int indent;
	private String text;
	
	public CodeWriter() {
		this.indent = 0;
		this.text = "";
	}

	public void incIndent() {
		++this.indent;
	}

	public void decIndent() {
		--this.indent;
		if(this.indent < 0) {
			this.indent = 0;
		}
	}
	
	public void out(String s) {
		String indentText = "";
		for(int i = 0; i < this.indent; ++i) {
			indentText = indentText + "    "; 
		}

		if(this.text.length() > 0 && this.text.lastIndexOf('\n') == this.text.length() - 1) {
			this.text = this.text + indentText;
		}

		this.text = this.text + s;
	}
	
	public String getText() {
		return this.text;
	}

}
