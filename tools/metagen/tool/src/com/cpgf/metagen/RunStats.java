package com.cpgf.metagen;

public class RunStats {
	private static int writtenFileCount;
	private static int totalWrittenFileCount;

	public static void report() {
		Util.trace("Total files " + totalWrittenFileCount);
		Util.trace("Written " + writtenFileCount + "  Skipped " + (totalWrittenFileCount - writtenFileCount));
	}
	
	public static void countWrittenFile(boolean written) {
		++totalWrittenFileCount;
		if(written) {
			++writtenFileCount;
		}
	}
}
