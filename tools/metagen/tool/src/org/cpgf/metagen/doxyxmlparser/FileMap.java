package org.cpgf.metagen.doxyxmlparser;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.cpgf.metagen.Util;
import org.w3c.dom.Node;


public class FileMap {
	private HashMap<String, FileInfo> fileMap;
	private List<String> locationList;
	
	public FileMap() {
		this.fileMap = new HashMap<String, FileInfo>();
		this.locationList = new ArrayList<String>();
	}
	
	public void addLocation(String location, Node node) {
		if(! this.fileMap.containsKey(location)) {
			this.fileMap.put(location, new FileInfo(location));
			this.locationList.add(location);
		}
		
		FileInfo fileInfo = this.fileMap.get(location);
		for(Node child : Util.getChildNodesByName(node, "innernamespace")) {
			fileInfo.getNamespaceList().add(Util.getNodeText(child));
		}
	}

	public HashMap<String, FileInfo> getFileMap() {
		return fileMap;
	}
}
