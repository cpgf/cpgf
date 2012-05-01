package com.cpgf.metagen.doxyxmlparser;

import java.util.ArrayList;
import java.util.List;

public class FileInfo {
	private String location;
	private List<String> namespaceList;
	
	public FileInfo(String location) {
		this.location = location;
		this.namespaceList = new ArrayList<String>();
	}

	public String getLocation() {
		return location;
	}

	public List<String> getNamespaceList() {
		return namespaceList;
	}
	
	
}
