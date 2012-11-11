package org.cpgf.metagen.metawriter;

public class MetaClassCode {
	public String headerCode = "";
	public String sourceCode = "";
	public String createFunctionName = "";
	
	public MetaClassCode() {
		this.headerCode = "";
		this.sourceCode = "";
		this.createFunctionName = "";
	}
	
	public MetaClassCode(MetaClassCode other) {
		this.headerCode = other.headerCode;
		this.sourceCode = other.sourceCode;
		this.createFunctionName = other.createFunctionName;
	}
	
}
