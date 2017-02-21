package org.cpgf.metagen.codewriter;

public class CppWriter extends CodeWriter {
	public void beginIncludeGuard(String name) {
		name = name.replaceAll("\\.", "_");
		name = name.toUpperCase();
		name = "CPGF_" + name;

		this.write("#ifndef " + name + "\n");
		this.write("#define " + name + "\n");
		this.write("\n\n");
	}

	public void endIncludeGuard() {
		this.write("\n\n");
		this.write("#endif");
		this.write("\n");
	}

	public void include(String fileName) {
		this.include(fileName, false);
	}
	
	public void include(String fileName, boolean useBrackets) {
		String begin = useBrackets ? "<" : "\"";
		String end = useBrackets ? ">" : "\"";

		this.write("#include " + begin + fileName + end + "\n");
	}

	public void beginNamespace(String namespace) {
		if(namespace != null) {
			if(! namespace.equals("")) {
				namespace = namespace + " ";
			}
			this.write("namespace " + namespace + "{ \n");
			this.write("\n\n");
		}
	}

	public void endNamespace(String namespace) {
		if(namespace != null) {
			if(namespace.equals("")) {
				this.write("} // unnamed namespace\n");
			}
			else {
				this.write("} // namespace " + namespace + "\n");
			}
			this.write("\n\n");
		}
	}

	public void useNamespace(String namespace) {
		this.write("using namespace " + namespace + ";\n");
	}

	public void beginBlock() {
		this.write("{\n");
		this.incIndent();
	}

	public void endBlock() {
		this.decIndent();
		this.write("}\n");
	}

	public void endBlock(String append) {
		this.decIndent();
		this.write("}" + append + "\n");
	}

}
