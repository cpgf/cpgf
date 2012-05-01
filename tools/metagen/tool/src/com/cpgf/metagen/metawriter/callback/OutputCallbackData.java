package com.cpgf.metagen.metawriter.callback;

public class OutputCallbackData {
	private boolean skipBind;
	
	public OutputCallbackData() {
		this.skipBind = false;
	}

	public boolean isSkipBind() {
		return skipBind;
	}

	public void setSkipBind(boolean skipBind) {
		this.skipBind = skipBind;
	}
}
