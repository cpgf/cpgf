namespace cpgf {

/**
GPlainFunction is a class to convert any functor object to old C style plain callback (C function pointer). <br />
The template parameter 'Signature' is the function prototype, same as the one in GCallback, such as 'void (int)'. <br />
The template parameter 'Functor' is the functor object type. <br />
The template parameter 'func' is the functor object. <br />

Three C style functions are provided with different calling convention. <br />
'invoke' is for normal C function with cdecl calling convention. It's useful to interact with C library functions, such as qsort, bsearch, as callback function. <br />
'stdcallInvoke' is for normal C function with cdecl calling convention. It's very useful to interact with Windows API as callback function. <br />
'fastcallInvoke' is for normal C function with cdecl calling convention. <br />

Note: <br />
The functor object must have <a href="http://publib.boulder.ibm.com/infocenter/comphelp/v8v101/index.jsp?topic=%2Fcom.ibm.xlcpp8a.doc%2Flanguage%2Fref%2Fcplr082.htm" target="_blank">external linkage</a>. <br />
In brief, it must be global object, without declared "static". <br />

See GPlainCallback for sample code.
*/
template <typename Signature, typename Functor, const Functor & func>
class GPlainFunction
{
public:
	static ResultType invoke(P1 p1, P2 p2, ..., Pn pn);
	static ResultType __stdcall stdcallInvoke(P1 p1, P2 p2, ..., Pn pn);
	static ResultType __fastcall fastcallInvoke(P1 p1, P2 p2, ..., Pn pn);
};

/**
GPlainCallback is a class to convert any GCallback object to old C style plain callback (C function pointer). <br />
It's provided as a shortcut to GPlainFunction. <br />
The template parameter 'Signature' is the function prototype, same as the one in GCallback, such as 'void (int)'. <br />
The template parameter 'cb' is the callback object. <br />

Three C style functions are provided with different calling convention. <br />
'invoke' is for normal C function with cdecl calling convention. It's useful to interact with C library functions, such as qsort, bsearch, as callback function. <br />
'stdcallInvoke' is for normal C function with cdecl calling convention. It's very useful to interact with Windows API as callback function. <br />
'fastcallInvoke' is for normal C function with cdecl calling convention. <br />

Note: <br />
The callback object must have <a href="http://publib.boulder.ibm.com/infocenter/comphelp/v8v101/index.jsp?topic=%2Fcom.ibm.xlcpp8a.doc%2Flanguage%2Fref%2Fcplr082.htm" target="_blank">external linkage</a>. <br />
In brief, it must be global object, without declared "static". <br />

Sample usage: <br />

@code

typedef int Compare(const void *, const void *);
// below also works
//typedef int (*Compare)(const void *, const void *);

struct CompareObject {
	int operator() (const void * a, const void * b) const {
		return (*(int*)a - *(int*)b);
	}
};

struct RevertObject {
	int revertCompare(const void * a, const void * b) {
		return (*(int*)b - *(int*)a);
	}
};

//extern GCallback<Compare> cb;
GCallback<Compare> cb; // can be extern, but can't be static (internal linkage)

CompareObject co; // same as cb, co must have external linkage

static void testPlaincallback() {
	int values[] = { 6, 2, 1, 3, 5, 4 };
	const int count = sizeof(values) / sizeof(values[0]);

	qsort(values, count, sizeof(values[0]), &GPlainFunction<Compare, CompareObject, co>::invoke);
	cout << "Normal qsort: \t\t";
	for(int i = 0; i < count; ++i) {
		cout << values[i] << " ";
	}
	cout << endl;

	RevertObject ro;
	cb = GCallback<Compare>(&ro, &RevertObject::revertCompare);
	qsort(values, count, sizeof(values[0]), &GPlainCallback<Compare, cb>::invoke);
	cout << "Reverted qsort: \t";
	for(int i = 0; i < count; ++i) {
		cout << values[i] << " ";
	}
	cout << endl;
}

//The output is,
//Normal qsort: 		1 2 3 4 5 6 
//Reverted qsort: 	6 5 4 3 2 1 


@endcode
*/
template <typename Signature, const GCallback<Signature> & cb>
class GPlainCallback : public GPlainFunction<Signature, GCallback<Signature>, cb>
{
public:
	static ResultType invoke(P1 p1, P2 p2, ..., Pn pn);
	static ResultType __stdcall stdcallInvoke(P1 p1, P2 p2, ..., Pn pn);
	static ResultType __fastcall fastcallInvoke(P1 p1, P2 p2, ..., Pn pn);
};


}
