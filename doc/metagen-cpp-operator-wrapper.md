# Use C++ operator wrapper functions from script

The tool Metagen can generate named functions as wrapper for C++ operators. That allows most overloaded C++ operators can be used from script language, such as Javascript, which doesn't support operator overloading. The operator wrapper is portable. It behaves exactly same in all supported script languages -- Lua, JavaScript, and Python.

Though cpgf Lua binding already supports C++ operators, and in the future Python binding may support it too, usually the script languages support much less kinds of operators than C++, and operator overloading can't be ported between different script languages (imagine if we need to port Lua code to JavaScript).

## Use the operator wrapper

In C++ the operator overloading makes "a + b" looks very sweet, but in fact it's a kind of syntax sugar to the raw form "a.operator + (b)", in which the operator is used in function syntax.

Metagen generates wrapper similar as the function syntax, and map each C++ operator to a function. So in script, when we want to add two objects, we call a._opAdd(b). It's awkward than "a + b", but not worse than "a.operator + (b)".

Note: metagen only generates wrapper for class member operators, global operators are not wrapped.

The table below shows how C++ operators are mapped to function.

<HTML>
<table columns="4">
<tr>
<th>C++ operator</th>
<th>Function name</th>
<th>C++ code</th>
<th>Script code</th>
</tr>
<tr><td>  Addition +  </td><td>  _opAdd <sup>1</sup>  </td><td>  a + b  </td><td>  a._opAdd(b)  </td></tr>
<tr><td>  Subtraction -  </td><td>  _opSub  </td><td>  a - b  </td><td>  a._opSub(b)  </td></tr>
<tr><td>  Multiplication *  </td><td>  _opMul  </td><td>  a * b  </td><td>  a._opMul(b)  </td></tr>
<tr><td>  Division /  </td><td>  _opDiv  </td><td>  a / b  </td><td>  a._opDiv(b)  </td></tr>
<tr><td>  Modulo %  </td><td>  _opMod  </td><td>  a % b  </td><td>  a._opMod(b)  </td></tr>
<tr><td>  Unary plus +  </td><td>  _opPlus  </td><td>  +a  </td><td>  a._opPlus()  </td></tr>
<tr><td>  Unary minus -  </td><td>  _opMinus  </td><td>  -a  </td><td>  a._opMinus()  </td></tr>
<tr><td>  Addition assignment +=  </td><td>  _opAddAssign  </td><td>  a += b  </td><td>  a._opAddAssign(b)  </td></tr>
<tr><td>  Subtraction assignment -=  </td><td>  _opSubAssign  </td><td>  a -= b  </td><td>  a._opSubAssign(b)  </td></tr>
<tr><td>  Multiplication assignment *=  </td><td>  _opMulAssign  </td><td>  a *= b  </td><td>  a._opMulAssign(b)  </td></tr>
<tr><td>  Division assignment /=  </td><td>  _opDivAssign  </td><td>  a /= b  </td><td>  a._opDivAssign(b)  </td></tr>
<tr><td>  Modulo assignment %=  </td><td>  _opModAssign  </td><td>  a %= b  </td><td>  a._opModAssign(b)  </td></tr>
<tr><td>  Basic assignment =  </td><td>  _opAssign  </td><td>  a = b  </td><td>  a._opAssign(b)  </td></tr>
<tr><td>  Equal to ##### </td><td>  _opEqual  </td><td>  a b  </td><td>  a._opEqual(b)  </td></tr>
<tr><td>  Not equal to !=  </td><td>  _opNotEqual  </td><td>  a != b  </td><td>  a._opNotEqual(b)  </td></tr>
<tr><td>  Greater than >  </td><td>  _opGreater  </td><td>  a > b  </td><td>  a._opGreater(b)  </td></tr>
<tr><td>  Less than <  </td><td>  _opLess  </td><td>  a < b  </td><td>  a._opLess(b)  </td></tr>
<tr><td>  Greater than or equal to >=  </td><td>  _opGreaterEqual  </td><td>  a >= b  </td><td>  a._opGreaterEqual(b)  </td></tr>
<tr><td>  Less than or equal to <=  </td><td>  _opLessEqual  </td><td>  a <= b  </td><td>  a._opLessEqual(b)  </td></tr>
<tr><td>  Logical AND &&  </td><td>  _opAnd  </td><td>  a && b  </td><td>  a._opAnd(b)  </td></tr>
<tr><td>  Logical OR ||  </td><td>  _opOr  </td><td>  a || b  </td><td>  a._opOr(b)  </td></tr>
<tr><td>  Logical negation (NOT) !  </td><td>  _opNot  </td><td>  !a  </td><td>  a._opNot()  </td></tr>
<tr><td>  Bitwise AND &  </td><td>  _opBitAnd  </td><td>  a & b  </td><td>  a._opBitAnd(b)  </td></tr>
<tr><td>  Bitwise OR |  </td><td>  _opBitOr  </td><td>  a | b  </td><td>  a._opBitOr(b)  </td></tr>
<tr><td>  Bitwise XOR ^  </td><td>  _opBitXor  </td><td>  a ^ b  </td><td>  a._opBitXor(b)  </td></tr>
<tr><td>  Bitwise left shift <<  </td><td>  _opLeftShift  </td><td>  a << b  </td><td>  a._opLeftShift(b)  </td></tr>
<tr><td>  Bitwise right shift >>  </td><td>  _opRightShift  </td><td>  a >> b  </td><td>  a._opRightShift(b)  </td></tr>
<tr><td>  Bitwise NOT ~  </td><td>  _opBitNot  </td><td>  ~a  </td><td>  a._opBitNot()  </td></tr>
<tr><td>  Bitwise AND assignment &=  </td><td>  _opBitAndAssign  </td><td>  a &= b  </td><td>  a._opBitAndAssign(b)  </td></tr>
<tr><td>  Bitwise OR assignment |=  </td><td>  _opBitOrAssign  </td><td>  a |= b  </td><td>  a._opBitOrAssign(b)  </td></tr>
<tr><td>  Bitwise XOR assignment ^=  </td><td>  _opBitXorAssign  </td><td>  a ^= b  </td><td>  a._opBitXorAssign(b)  </td></tr>
<tr><td>  Bitwise left shift assignment <<=  </td><td>  _opLeftShiftAssign  </td><td>  a <<= b  </td><td>  a._opLeftShiftAssign(b)  </td></tr>
<tr><td>  Bitwise right shift assignment >>=  </td><td>  _opRightShiftAssign  </td><td>  a >>= b  </td><td>  a._opRightShiftAssign(b)  </td></tr>
<tr><td>  Prefix increment ++  </td><td>  _opInc  </td><td>  ++a  </td><td>  a._opInc()  </td></tr>
<tr><td>  Prefix decrement --  </td><td>  _opDec  </td><td>  --a  </td><td>  a._opDec()  </td></tr>
<tr><td>  Suffix increment ++  </td><td>  _opIncSuffix  </td><td>  a++  </td><td>  a._opIncSuffix()  </td></tr>
<tr><td>  Suffix decrement --  </td><td>  _opDecSuffix  </td><td>  a--  </td><td>  a._opDecSuffix()  </td></tr>
<tr><td>  Comma ,  </td><td>  _opComma  </td><td>  a , b  </td><td>  a._opComma(b)  </td></tr>
<tr><td>  Array item get [] <sup>2</sup>  </td><td>  _opArrayGet  </td><td>  v = a[index]  </td><td>  v = a._opArrayGet(index)  </td></tr>
<tr><td>  Array item set [] <sup>2</sup>  </td><td>  _opArraySet  </td><td>  a[index] = v  </td><td>  a._opArraySet(index, v)  </td></tr>
<tr><td>  Function call ()  </td><td>  _opFunction  </td><td>  a(a1, a2, a3)  </td><td>  a._opFunction(a1, a2, a3)  </td></tr>
<tr><td>  Reference &  </td><td>  _opAddress  </td><td>  &a  </td><td>  a._opAddress(b)  </td></tr>
<tr><td>  Indirection *  </td><td>  _opDerefer  </td><td>  *a  </td><td>  a._opDerefer(b)  </td></tr>
</table>
</HTML>

<sup>1</sup> Why do we use so ugly name like _opAdd() instead of just add()? This is deliberate to avoid name conflicting. Those functions' meta data are reflected into any classes, in which there may be already some member functions such as add, plus, div, etc.

<sup>2</sup> Array subscript operator is wrapped in two functions, getter and setter. Metagen generates the setter function when and only when the operator result value is a reference to non-const, for example, "int & operator[] (int index)"

## Unwrapped operators

The operators that are not wrapped is "->", "->*", and type casting. They are very hard to wrap, and it makes no sense to wrap them. The operators new and delete are not wrapped too, but we don't need them. We can always create new instance and ignore the overloaded new and delete operators.

## Code samples

Assume we have a class vector, which supports +-*/ operators and the normalize function.
Then if have C++ code
```c++

// This is a modification code in Irrlicht sample code 07.Collision.
// It calculates a ray cast from the camera.
vector start = camera->getPositionVector();
vector end = start + (camera->getTargetVector() - start).normalize() * 1000.0;
```
Then in JavaScript we can write
```javascript

// This is a modification code in Irrlicht sample code 07.Collision
// It calculates a ray cast from the camera.
var start = camera.getPositionVector();
var end = start._opAdd(camera.getTargetVector()._opSub(start).normalize()._opMul(1000.0));
```
