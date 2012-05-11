package com.cpgf.metagen.metawriter.callback;

import com.cpgf.metagen.Util;

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
	
	// used by Javascript callback
	public void trace(String message) {
		Util.trace(message);
	}
}
