#include "testserializationcommonclasses.h"
#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include "gmetatextstreamarchive.h"

#include <sstream>
#include <string>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename AR>
void doTestSimpleArray(IMetaService * service, IMetaWriter * writer, IMetaReader * reader, const AR & ar)
{
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(GMetaArchiveConfig().getFlags(), service, writer));

	enum {
		A = 3, B = 5, C = 6, D = 7, E = 8, F = 10, G = 11,
		H = 12, I = 15, J = 16, K = 17, L = 18, M = 20, N = 21,
		O = 22, P = 25, Q = 28,
		R = 3
	};

	bool b[A];
	char c[B];
	wchar_t wc[C];
	signed char sc[D];
	unsigned char uc[E];
	short si[F];
	unsigned short usi[G];
	int i[H];
	unsigned int ui[I];
	long l[J];
	unsigned long ul[K];
	long long ll[L];
	unsigned long long ull[M];
	float f[N];
	double df[O];
	long double ldf[P];
	string s[Q];
	string * ps[Q];
	TestSerializeClass o[R];

#define INIT(v, l) for(int z = 0; z < l; ++z) initTestValue(v[z], getTestSeed(z + 1));
	INIT(b, A)
	INIT(c, B)
	INIT(wc, C)
	INIT(sc, D)
	INIT(uc, E)
	INIT(si, F)
	INIT(usi, G)
	INIT(i, H)
	INIT(ui, I)
	INIT(l, J)
	INIT(ul, K)
	INIT(ll, L)
	INIT(ull, M)
	INIT(f, N)
	INIT(df, O)
	INIT(ldf, P)
	INIT(s, Q)
	INIT(o, R)
#undef INIT

#define INIT(v, l) for(int z = 0; z < l; ++z) ps[z] = &s[z];
	INIT(ps, Q)
#undef INIT

	serializeWriteValue(archiveWriter.get(), "b", b);
	serializeWriteValue(archiveWriter.get(), "c", c);
	serializeWriteValue(archiveWriter.get(), "wc", wc);
	serializeWriteValue(archiveWriter.get(), "sc", sc);
	serializeWriteValue(archiveWriter.get(), "uc", uc);
	serializeWriteValue(archiveWriter.get(), "si", si);
	serializeWriteValue(archiveWriter.get(), "usi", usi);
	serializeWriteValue(archiveWriter.get(), "i", i);
	serializeWriteValue(archiveWriter.get(), "ui", ui);
	serializeWriteValue(archiveWriter.get(), "l", l);
	serializeWriteValue(archiveWriter.get(), "ul", ul);
	serializeWriteValue(archiveWriter.get(), "ll", ll);
	serializeWriteValue(archiveWriter.get(), "ull", ull);
	serializeWriteValue(archiveWriter.get(), "f", f);
	serializeWriteValue(archiveWriter.get(), "df", df);
	serializeWriteValue(archiveWriter.get(), "ldf", ldf);
	serializeWriteValue(archiveWriter.get(), "s", s);
	serializeWriteValue(archiveWriter.get(), "ps", ps);
	serializeWriteValue(archiveWriter.get(), "o", o);

	ar.rewind();

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(GMetaArchiveConfig().getFlags(), service, reader));

	bool rb[A];
	char rc[B];
	wchar_t rwc[C];
	signed char rsc[D];
	unsigned char ruc[E];
	short rsi[F];
	unsigned short rusi[G];
	int ri[H];
	unsigned int rui[I];
	long rl[J];
	unsigned long rul[K];
	long long rll[L];
	unsigned long long rull[M];
	float rf[N];
	double rdf[O];
	long double rldf[P];
	string rs[Q];
	string * rps[Q];
	TestSerializeClass ro[R];

#define INIT(v, l) for(int z = 0; z < l; ++z) initTestValue(v[z], getTestSeed(0));
	INIT(rb, A)
	INIT(rc, B)
	INIT(rwc, C)
	INIT(rsc, D)
	INIT(ruc, E)
	INIT(rsi, F)
	INIT(rusi, G)
	INIT(ri, H)
	INIT(rui, I)
	INIT(rl, J)
	INIT(rul, K)
	INIT(rll, L)
	INIT(rull, M)
	INIT(rf, N)
	INIT(rdf, O)
	INIT(rldf, P)
	INIT(rs, Q)
	INIT(ro, R)
#undef INIT

#define INIT(v, l) for(int z = 0; z < l; ++z) v[z] = NULL;
	INIT(rps, Q)
#undef INIT


	serializeReadValue(archiveReader.get(), "rb", rb);
	serializeReadValue(archiveReader.get(), "rc", rc);
	serializeReadValue(archiveReader.get(), "rwc", rwc);
	serializeReadValue(archiveReader.get(), "rsc", rsc);
	serializeReadValue(archiveReader.get(), "ruc", ruc);
	serializeReadValue(archiveReader.get(), "rsi", rsi);
	serializeReadValue(archiveReader.get(), "rusi", rusi);
	serializeReadValue(archiveReader.get(), "ri", ri);
	serializeReadValue(archiveReader.get(), "rui", rui);
	serializeReadValue(archiveReader.get(), "rl", rl);
	serializeReadValue(archiveReader.get(), "rul", rul);
	serializeReadValue(archiveReader.get(), "rll", rll);
	serializeReadValue(archiveReader.get(), "rull", rull);
	serializeReadValue(archiveReader.get(), "rf", rf);
	serializeReadValue(archiveReader.get(), "rdf", rdf);
	serializeReadValue(archiveReader.get(), "rldf", rldf);
	serializeReadValue(archiveReader.get(), "rs", rs);
	serializeReadValue(archiveReader.get(), "rps", rps);
	serializeReadValue(archiveReader.get(), "ro", ro);

#define EQ(v, u, l) for(int z = 0; z < l; ++z) GEQUAL(v[z], u[z]);
	EQ(b, rb, A)
	EQ(c, rc, B)
	EQ(wc, rwc, C)
	EQ(sc, rsc, D)
	EQ(uc, ruc, E)
	EQ(si, rsi, F)
	EQ(usi, rusi, G)
	EQ(i, ri, H)
	EQ(ui, rui, I)
	EQ(l, rl, J)
	EQ(ul, rul, K)
	EQ(ll, rll, L)
	EQ(ull, rull, M)
	EQ(s, rs, Q)
	EQ(o, ro, R)
#undef EQ

#define EQ(v, u, l) for(int z = 0; z < l; ++z) GCHECK(fabs(v[z] - u[z]) < 0.1);
	EQ(f, rf, N)
	EQ(df, rdf, O)
	EQ(ldf, rldf, P)
#undef EQ

#define EQ(v, u, l) for(int z = 0; z < l; ++z) GEQUAL(&v[z], u[z]);
	EQ(rs, rps, Q)
#undef EQ

}

GTEST(testSimpleArray)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GTextStreamMetaReader<stringstream> inputStream(NULL, stream);
	
	doTestSimpleArray(service.get(), &outputStream, &inputStream, TestArchiveStream<stringstream>(stream));
return;	
	cout << stream.str().c_str() << endl;
	TestSerializeClass a[3];
	cout << sizeof(TestSerializeClass) << endl;
	cout << sizeof(a[0]) << endl;
	cout << sizeof(a) << endl;
}


} // unnamed namespace
