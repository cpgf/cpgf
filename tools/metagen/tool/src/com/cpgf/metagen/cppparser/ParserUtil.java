package com.cpgf.metagen.cppparser;

import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class ParserUtil {
	public static List<String> splitTypeTokenLiterals(String statement) {
		List<String> tokenList = new ArrayList<String>();

		StringTokenizer tokenizer = new StringTokenizer(statement, " \t\n*&<>[](),", true);

		while(tokenizer.hasMoreTokens()) {
			String token = tokenizer.nextToken();
			
			tokenList.add(token);
		}
		
		// check rvalue reference
		for(int i = tokenList.size() - 1; i > 0; --i) {
			if(tokenList.get(i).equals("&") && tokenList.get(i - 1).equals("&")) {
				tokenList.set(i - 1, "&&");
				tokenList.remove(i);
			}
		}

		// remove all spaces
		for(int i = tokenList.size() - 1; i >= 0; --i) {
			if(tokenList.get(i).matches("\\s")) {
				tokenList.remove(i);
			}
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
		EnumTypeTokenKind.RValueReference,
		EnumTypeTokenKind.Reference,
		EnumTypeTokenKind.LeftAngle,
		EnumTypeTokenKind.RightAngle,
		EnumTypeTokenKind.LeftSquare,
		EnumTypeTokenKind.RightSquare,
		EnumTypeTokenKind.LeftParenthesis,
		EnumTypeTokenKind.RightParenthesis,
		EnumTypeTokenKind.Comma,
		EnumTypeTokenKind.Const,
		EnumTypeTokenKind.Volatile,
	};
	
	private static String[] typeTokens = {
		"*",
		"&&",
		"&",
		"<",
		">",
		"[",
		"]",
		"(",
		")",
		",",
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

	public static <T> void swapListItems(List<T> list, int a, int b) {
		T temp = list.get(a);
		list.set(a, list.get(b));
		list.set(b, temp);
	}
	
	public static <T> int findInArray(T[] array, T item) {
		for(int i = 0; i < array.length; ++i) {
			if(array[i].equals(item)) {
				return i;
			}
		}
		
		return -1;
	}
	
	// move "const" to right most until it's next to * or &
	private static boolean normalizeConst(List<TypeToken> tokenList) {
		return doNormalizeConstVolatile(tokenList, EnumTypeTokenKind.Const);
	}

	// move "volatile" to right most until it's next to * or &
	private static boolean normalizeVolatile(List<TypeToken> tokenList) {
		return doNormalizeConstVolatile(tokenList, EnumTypeTokenKind.Volatile);
	}
	
	public static boolean normalizeConstVolatile(List<TypeToken> tokenList) {
		boolean did = normalizeVolatile(tokenList);
		did = normalizeConst(tokenList) || did;
		return did;
	}
	
	// move "const" or "volatile" to right most until it's next to * or &
	private static boolean doNormalizeConstVolatile(List<TypeToken> tokenList, EnumTypeTokenKind kindToNormalize) {
		boolean did = false;
		
		for(int i = tokenList.size() - 1; i > 0; --i) {
			TypeToken token = tokenList.get(i);
			
			if(token.getKind() != kindToNormalize) {
				continue;
			}
			
			int constIndex = i;
			int nextIndex = i - 1;

			int nestedDepth = 0;
			
			while(nextIndex >= 0) {
				TypeToken nextToken = tokenList.get(nextIndex);
				if(nextToken.isRightBracket()) {
					++nestedDepth;
				}
				else if(nextToken.isLeftBracket()) {
					if(nestedDepth == 0) { // int <const...
						break;
					}
					
					--nestedDepth;
				}

				if(nestedDepth != 0 || ! nextToken.isPointerOrReference()) {
					swapListItems(tokenList, constIndex, nextIndex);
					constIndex = nextIndex;

					if(! did) {
						did = true;
					}
				}
				else {
					break;
				}
				
				--nextIndex;
			}
		}

		return did;
	}

	private static String[] signedUnsignedTypes = {
		"char", "short", "int", "long", // no need long long because it's covered by long
	};
	
	// replace all isolate signed/unsigned to signed/unsigned int
	public static boolean normalizeSignedAndUnsigned(List<TypeToken> tokenList) {
		boolean did = false;
		
		for(int i = 0; i < tokenList.size(); ++i) {
			TypeToken token = tokenList.get(i);
			
			if(token.getKind() != EnumTypeTokenKind.Symbol) {
				continue;
			}
			
			if(! token.getToken().equals("signed") && ! token.getToken().equals("unsigned")) {
				continue;
			}
			
			boolean last = (i == tokenList.size() - 1);
			
			TypeToken nextToken = null;
			
			if(! last) {
				nextToken = tokenList.get(i + 1); 
			}
			
			if(nextToken == null || findInArray(signedUnsignedTypes, nextToken.getToken()) < 0) {
				tokenList.add(i + 1, new TypeToken("int"));
				did = true;
			}
		}
		
		return did;
	}
	
	// replace all isolate short to short int
	public static boolean normalizeShortInt(List<TypeToken> tokenList) {
		boolean did = false;
		
		for(int i = 0; i < tokenList.size(); ++i) {
			TypeToken token = tokenList.get(i);
			
			if(token.getKind() != EnumTypeTokenKind.Symbol) {
				continue;
			}
			
			if(! token.getToken().equals("short")) {
				continue;
			}
			
			boolean last = (i == tokenList.size() - 1);
			
			TypeToken nextToken = null;
			
			if(! last) {
				nextToken = tokenList.get(i + 1); 
			}
			
			if(nextToken == null || ! nextToken.getToken().equals("int")) {
				tokenList.add(i + 1, new TypeToken("int"));
				did = true;
			}
		}
		
		return did;
	}
	
	private static String[] keywordToDiscard = {
			"class", "struct", "union", "enum"
	};

	// replace all class, struct, union, and enum
	public static boolean normalizeUnusedKeyword(List<TypeToken> tokenList) {
		boolean did = false;
		
		for(int i = tokenList.size() - 1; i >= 0; --i) {
			TypeToken token = tokenList.get(i);
			
			if(token.getKind() != EnumTypeTokenKind.Symbol) {
				continue;
			}
			
			if(findInArray(keywordToDiscard, token.getToken()) < 0) {
				continue;
			}
			
			tokenList.remove(i);

			did = true;
		}
		
		return did;
	}
	
	
	
}
