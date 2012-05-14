package com.cpgf.metagen.metawriter.callback;

import com.cpgf.metagen.Util;

public class OutputCallbackData {
	private boolean skipBind;
	private String headerCode;
	private String sourceCode;
	
	public OutputCallbackData() {
		this.skipBind = false;
	}

	public boolean isSkipBind() {
		return skipBind;
	}

	public void setSkipBind(boolean skipBind) {
		this.skipBind = skipBind;
	}
	
	// used by Javascript callback
	public void trace(String message) {
		Util.trace(message);
	}
	
	public void addHeaderCode(String code) {
		if(this.headerCode == null) {
			this.headerCode = "";
		}
		
		this.headerCode = this.headerCode + code + "\n";
	}

	public void addSourceCode(String code) {
		if(this.sourceCode == null) {
			this.sourceCode = "";
		}
		
		this.sourceCode = this.sourceCode + code + "\n";
	}

	public String getHeaderCode() {
		return headerCode;
	}

	public String getSourceCode() {
		return sourceCode;
	}
}
