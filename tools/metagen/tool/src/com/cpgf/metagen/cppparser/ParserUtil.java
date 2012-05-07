package com.cpgf.metagen.cppparser;

import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class ParserUtil {
	public static List<String> splitTypeTokenLiterals(String statement) {
		List<String> tokenList = new ArrayList<String>();

		StringTokenizer tokenizer = new StringTokenizer(statement, " \t\n*&<>", true);

		// check rvalue reference
		for(int i = tokenList.size() - 1; i > 0; --i) {
			if(tokenList.get(i).equals("&") && tokenList.get(i - 1).equals("&")) {
				tokenList.set(i - 1, "&&");
				tokenList.remove(i);
			}
		}

		while(tokenizer.hasMoreTokens()) {
			String token = tokenizer.nextToken();
			if(token.matches("\\s")) {
				continue;
			}
			
			tokenList.add(token);
		}
		
		return tokenList;
	}
	
	public static List<TypeToken> splitTypeTokens(List<String> tokenLiterals) {
		List<TypeToken> tokenList = new ArrayList<TypeToken>();
		
		for(String token : tokenLiterals) {
			tokenList.add(new TypeToken(token));
		}
		
		return tokenList;
	}
	
	private static EnumTypeTokenKind[] typeTokenKinds = {
		EnumTypeTokenKind.Pointer,
		EnumTypeTokenKind.RValueRerference,
		EnumTypeTokenKind.Reference,
		EnumTypeTokenKind.LeftAngle,
		EnumTypeTokenKind.RightAngle,
		EnumTypeTokenKind.Const,
		EnumTypeTokenKind.Volatile,
	};
	
	private static String[] typeTokens = {
		"*",
		"&&",
		"&",
		"<",
		">",
		"const",
		"volatile",
	};

	public static EnumTypeTokenKind getTypeTokenKind(String token) {
		for(int i = 0; i < typeTokens.length; ++i) {
			if(typeTokens[i].equals(token)) {
				return typeTokenKinds[i];
			}
		}
		return EnumTypeTokenKind.Symbol;
	}

}
