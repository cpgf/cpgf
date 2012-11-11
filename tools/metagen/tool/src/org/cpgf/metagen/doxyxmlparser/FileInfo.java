package org.cpgf.metagen.doxyxmlparser;

import java.util.ArrayList;
import java.util.List;

import org.cpgf.metagen.Util;


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
	
	public void appendNamespaces(List<String> otherNamespaces) {
		for(String namespace : otherNamespaces) {
			Util.addToList(this.namespaceList, namespace);
		}
	}
	
}
