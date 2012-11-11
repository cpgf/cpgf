package org.cpgf.metagen.cppparser;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.cpgf.metagen.Util;
import org.cpgf.metagen.metadata.CppInvokable;
import org.cpgf.metagen.metadata.CppType;
import org.cpgf.metagen.metadata.EnumCategory;
import org.cpgf.metagen.metadata.Parameter;


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
	
	// move "const" or "volatile" to left most until it's next to * or &
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
					Util.swapListItems(tokenList, constIndex, nextIndex);
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
			
			if(nextToken == null || Util.findInArray(signedUnsignedTypes, nextToken.getToken()) < 0) {
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
			
			if(Util.findInArray(keywordToDiscard, token.getToken()) < 0) {
				continue;
			}
			
			tokenList.remove(i);

			did = true;
		}
		
		return did;
	}
	
	public static String composePolicyRuleForParameter(String rule, int parameterIndex) {
		return rule + "<" + parameterIndex + ">";
	}

	// split the string by delimiter, the delimiter in brackets or quote marks is not counted
	public static int splitDelimitedString(List<String> tokenList, String s, char delimiter, int startIndex) {
		String leftBrackets = "{[<(";
		String rightBrackets = "}]>)";
		String brackets = leftBrackets + rightBrackets + "\"'";
		Deque<Character> bracketStack = new ArrayDeque<Character>();
		String token = "";
		boolean hasToken = false;
		for(; startIndex < s.length(); ++startIndex) {
			char c = s.charAt(startIndex);
			
			if(c == delimiter) {
				if(bracketStack.size() == 0) {
					tokenList.add(token.trim());
					token = "";
					hasToken = false;
					continue;
				}
			}
			
			if(brackets.indexOf(c) >= 0) {
				if(bracketStack.size() > 0) {
					char openingBracket = bracketStack.peekLast().charValue();
					if(openingBracket == '"' || openingBracket == '\'') {
						if(c == openingBracket) {
							bracketStack.pop();
						}
					}
					else {
						if(rightBrackets.indexOf(c) >= 0) {
							if(leftBrackets.indexOf(openingBracket) == rightBrackets.indexOf(c)) {
								bracketStack.pop();
							}
							else {
								break; // orphan bracket
							}
						}
						else {
							bracketStack.push(c);
						}
					}
				}
				else {
					if(rightBrackets.indexOf(c) >= 0) {
						break; // orphan bracket
					}
					bracketStack.push(c);
				}
			}
			
			token = token + c;
			hasToken = true;
		}
		
		if(hasToken) {
			tokenList.add(token.trim());
		}
		
		return startIndex;
	}

	public static String getLastWord(String text) {
		Matcher matcher = Pattern.compile(".*\\b([\\w\\d_]+)[^\\w\\d_]*$").matcher(text);
		if(matcher.matches()) {
			return matcher.group(1);
		}
		else {
			return text.trim();
		}
	}
	
	public static CppInvokable parseFunctionType(String functionType) {
		CppInvokable method = null;
		
		int leftBracketIndex = functionType.indexOf('(');
		int rightBracketIndex = functionType.lastIndexOf(')');
		
		if(leftBracketIndex >= 0 && rightBracketIndex > 0) {
			String resultType = functionType.substring(0, leftBracketIndex).trim();
			method = new CppInvokable(EnumCategory.Method, null, new CppType(null, resultType));
			String parameterText = functionType.substring(leftBracketIndex + 1, rightBracketIndex);
			List<String> parameters = new ArrayList<String>();
			splitDelimitedString(parameters, parameterText, ',', 0);
			
			for(String param : parameters) {
				String name = getLastWord(param);
				int n = param.lastIndexOf(name);
				String type = param.substring(0, n).trim();
				method.getParameterList().add(new Parameter(name, new CppType(null, type), null, null));
			}
		}

		return method;
	}

	public static List<String> parseArrayDimensions(List<TypeToken> tokenList) {
		List<String> dimensions = null;
		int count = tokenList.size();
		int nestedDepth = 0;

		for(int i = 0; i < count - 1; ++i) {
			TypeToken token = tokenList.get(i);
			
			if(token.getKind() == EnumTypeTokenKind.LeftAngle || token.getKind() == EnumTypeTokenKind.LeftParenthesis) {
				++nestedDepth;
			}
			
			if(token.getKind() == EnumTypeTokenKind.RightAngle || token.getKind() == EnumTypeTokenKind.RightParenthesis) {
				--nestedDepth;
			}
			
			if(nestedDepth == 0 && token.getKind() == EnumTypeTokenKind.LeftSquare) {
				if(dimensions == null) {
					dimensions = new ArrayList<String>();
				}

				String dim = "";
				int k = i + 1;
				for(; k < count; ++k) {
					TypeToken nextToken = tokenList.get(k);
					if(nextToken.getKind() == EnumTypeTokenKind.RightSquare) {
						break;
					}

					if(dim.length() > 0) {
						dim = dim + " ";
					}
					
					dim = dim + nextToken.getToken();
				}
				
				dimensions.add(dim);

				i = k;
			}
		}

		return dimensions;
	}

}
