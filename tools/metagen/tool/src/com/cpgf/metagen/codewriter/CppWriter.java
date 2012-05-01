package com.cpgf.metagen.codewriter;

public class CppWriter extends CodeWriter {
	public void beginIncludeGuard(String name) {
		name = name.replaceAll("\\.", "_");
		name = name.toUpperCase();
		name = "__" + name;

		this.out("#ifndef " + name + "\n");
		this.out("#define " + name + "\n");
		this.out("\n\n");
	}

	public void endIncludeGuard() {
		this.out("\n\n");
		this.out("#endif");
		this.out("\n");
	}

	public void include(String fileName) {
		this.include(fileName, false);
	}
	
	public void include(String fileName, boolean useBrackets) {
		String begin = useBrackets ? "<" : "\"";
		String end = useBrackets ? ">" : "\"";

		this.out("#include " + begin + fileName + end + "\n");
	}

	public void beginNamespace(String namespace) {
		if(namespace != null) {
			if(! namespace.equals("")) {
				namespace = namespace + " ";
			}
			this.out("namespace " + namespace + "{ \n");
			this.out("\n\n");
		}
	}

	public void endNamespace(String namespace) {
		if(namespace != null) {
			if(namespace.equals("")) {
				this.out("} // unnamed namespace\n");
			}
			else {
				this.out("} // namespace " + namespace + "\n");
			}
			this.out("\n\n");
		}
	}

	public void useNamespace(String namespace) {
		this.out("using namespace " + namespace + ";\n");
	}

	public void beginBlock() {
		this.out("{\n");
		this.incIndent();
	}

	public void endBlock() {
		this.decIndent();
		this.out("}\n");
	}

}
