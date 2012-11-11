package org.cpgf.metagen.codewriter;

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
	
	private String getIndentText() {
		String indentText = "";
		for(int i = 0; i < this.indent; ++i) {
			indentText = indentText + "    "; 
		}
		
		return indentText;
	}
	
	public void write(String s) {
		if(s == null || s.length() == 0) {
			return;
		}

		String indentText = this.getIndentText();

		if(this.text.length() > 0 && this.text.lastIndexOf('\n') == this.text.length() - 1) {
			this.text = this.text + indentText;
		}

		this.text = this.text + s;
	}
	
	public void writeLine(String s) {
		this.write(s + "\n");
	}
	
	public void writeMultipleLines(String s) {
		if(s.length() > 0) {
			String[] lines = s.split("\n");
			for(String line : lines) {
				this.write(line + "\n");
			}
		}
	}
	
	public void write(CodeWriter other) {
		if(other == null || other.getText().length() == 0) {
			return;
		}

		String indentText = this.getIndentText();
		String s = other.getText();
		s = s.replaceAll("\n", "\n" + indentText);
		s = indentText + s;
		s = s.replaceFirst(" +$", "");
		this.text = this.text + s;
	}

	public String getText() {
		return this.text;
	}

	public int getIndent() {
		return indent;
	}

	public void setIndent(int indent) {
		this.indent = indent;
	}

}
