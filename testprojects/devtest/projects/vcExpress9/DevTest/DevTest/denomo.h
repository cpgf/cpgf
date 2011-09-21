/*****************************************************************************
Denomo For C++, include header file

Website:
http://www.kbasm.com/

Discussion forum:
http://www.kbasm.com/forum/

License:
This work is copyright to Wang Qi (family name: Wang).
It is released under Mozilla Public License 1.1 (MPL 1.1),
you may obtain a copy of the License at
http://www.mozilla.org/MPL/MPL-1.1.html

What it is for:
This header file is for BCB and VC project.
Include this file in your project and put DenomoCPP.dll to the exe directory,
you can detect memory and resource leak in your C++ project.

Usage:
What you need to do to use this file is include this file in your project.
You need to only include it once.

To run your project with Denomo, you must put DenomoCPP.dll to the exe directory.

The macros you should know:
DENOMO_LIB: Define the path where contains the lib file.
There are some pre-compiled lib file in the include directory (same as where
this file is).
You only need to define DENOMO_LIB as the path, don't include the lib file name.
For example,
#define DENOMO_LIB "..\\..\\include\\"

STATIC_DLL and RUNTIME_DLL: Define whether Denomo For C++ static or dynamically
link to DenomoCPP.dll.
If STATIC_DLL is defined, Denomo For C++ will link to DenomoCPP.dll from within
the import table.
If RUNTIME_DLL is defined, Denomo For C++ will use LoadLibrary and GetProcAddress
to link to DenomoCPP.dll.
Define DENOMO_LIB will explicitly enable STATIC_DLL.

NOTE: it's highly recommended that you should use STATIC_DLL rather than RUNTIME_DLL.

Getting the source code:
The source code is included in the Denomo package. Denomo is a memory and
resource leak detection tool for Delphi.
You can download Denomo from http://www.kbasm.com/

NOTE: Though this package is for C++, the source code is written in pure Delphi
(Object Pascal and ASM).
To compile the source code, you must have Delphi 7 or up version installed.
******************************************************************************/

#ifndef __DENOMO_H
#define __DENOMO_H

#ifdef _DEBUG
    #define DENOMO_DEBUG_MODE
#endif

#if defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
    #define DENOMO_BC
#else
    #if defined(_MSC_VER)
        #define DENOMO_VC

        #ifdef DENOMO_DEBUG_MODE
            #define DENOMO_DBG_MM
		#endif
	#else
		#if defined __GNUC__
			#define DENOMO_GCC
			#error Sorry, GCC is not supported unless the asm code is converted.
		#else
			#error Only Borland C++ and Microsoft VC++ are supported.
		#endif
    #endif
#endif

#include <windows.h>
#include <unknwn.h>
#include <malloc.h>

#ifdef DENOMO_BC
#include <systobj.h>
#endif

#ifdef DENOMO_DBG_MM
#include <crtdbg.h>
#endif

#ifdef DENOMO_LIB
	#define BC_LIB DENOMO_LIB##"\\DenomoCPP_BC.lib"
	#define VC_LIB DENOMO_LIB##"\\DenomoCPP_VC.lib"

	#ifdef DENOMO_BC
	#pragma warn -asc
	#pragma comment(lib, BC_LIB)
	#endif

	#ifdef DENOMO_VC
	#pragma comment(lib, VC_LIB)
	#endif
	
	#define STATIC_DLL
#endif

#ifdef DENOMO_BC
#pragma warn -asc
#endif

#ifdef STATIC_DLL
#undef RUNTIME_DLL
#endif

#ifndef RUNTIME_DLL
#define STATIC_DLL
#else
#undef STATIC_DLL
#endif

#ifdef DENOMO_BC
#define denomo_emit(b) __emit__(b)
#else
#ifdef DENOMO_VC
#define denomo_emit(b) _asm { _emit b }
#endif
#endif

#define calltype WINAPI

struct TCPPInfo {
    HMODULE ModuleHandle;
	void *AddrGetMemBlockSize;
	void *_new;
	void *_delete;
    void *_new_array;
    void *_delete_array;
	void *malloc;
	void *calloc;
	void *realloc;
	void *free;
	void *_expand;
	void *_malloc_dbg;
	void *_calloc_dbg;
	void *_realloc_dbg;
	void *_free_dbg;
	void *_expand_dbg;
    void *TObject_InitInstance;
    void *GetMem;
    void *FreeMem;
    void *ReallocMem;
};
typedef TCPPInfo * PCPPInfo;

class ICPPService: public IUnknown {
public:
	virtual int calltype GetInstructionLength(unsigned char *ACode) = 0;
    virtual void calltype InitDenomoCPP(PCPPInfo AInfo) = 0;
    virtual void calltype DeInitDenomoCPP() = 0;
};

typedef void (calltype *ProcGetCPPService)(ICPPService **);

ICPPService *CPPService = NULL;
TCPPInfo CPPInfo;

#ifdef STATIC_DLL
extern "C" {
	void calltype GetCPPService(ICPPService **);
}
#endif

#ifdef DENOMO_BC
int calltype GetInstanceSize(void *cls)
{
    return TClass(cls)->InstanceSize();
}

void InitBCBInfo()
{
    const char *bpls[] = {
        "rtl100.bpl", "rtl70.bpl", "rtl60.bpl", "rtl50.bpl"
    };
    HMODULE h = NULL;
    for(int i = 0; i < sizeof(bpls) / sizeof(char *); i++) {
        h = GetModuleHandle(bpls[i]);
        if(h)
            break;
    }

    if(!h)
        return;
	CPPInfo.TObject_InitInstance = (void *)GetProcAddress(h, "@System@TObject@InitInstance$qqrp17System@TMetaClasspv");
	CPPInfo.GetMem = (void *)GetProcAddress(h, "@System@@GetMem$qqri");
	CPPInfo.FreeMem = (void *)GetProcAddress(h, "@System@@FreeMem$qqrpv");
	CPPInfo.ReallocMem = (void *)GetProcAddress(h, "@System@@ReallocMem$qqrrpvi");
}
#endif

unsigned int GetMemBlockSize(void *p)
{
	return _msize(p);
}

void *__stdcall GetCallAddress(unsigned char *addr)
{
	for(int i = 0; i < 100; i++) {
		if(0xe8 == *addr) {
			addr++;
			int ret = (*(int *)addr + (int)addr + 4);
			return (void *)ret;
		}
		addr += CPPService->GetInstructionLength(addr);
	}

	return NULL;
}

void *GetThunkAddress(void *addr)
{
    unsigned char *p = (unsigned char *)addr;

    if(0xff == *p && 0x25 == *(p+1))
        return (void *)(*(unsigned int *)(*(unsigned int *)(p + 2)));
    return addr;
}

static HMODULE GetCurrentModule()
{
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery((void *)&GetCurrentModule, &mbi, sizeof(mbi));
    if(mbi.State != MEM_COMMIT)
        return NULL; 

    return reinterpret_cast<HMODULE>(mbi.AllocationBase);
}

void ErrorOrExit(const char *msg, const char *title, int isexit = 0)
{
	const int __MB_SERVICE_NOTIFICATION = 0x00200000;
	const int __MB_SERVICE_NOTIFICATION_NT3X = 0x00040000;

	char buf[2048];
	strcpy(buf, "Denomo: ");
	strcat(buf, title);
	// specify MB_SERVICE_NOTIFICATION so that the message box can be displayed
	// even when the dll is unloading.
	MessageBoxA(0, msg, buf, MB_OK | MB_ICONWARNING | __MB_SERVICE_NOTIFICATION | __MB_SERVICE_NOTIFICATION_NT3X);
	if(isexit)
		ExitProcess(1);
}

void InitCPPInfo()
{
    ZeroMemory(&CPPInfo, sizeof(TCPPInfo));

    CPPInfo.ModuleHandle = GetCurrentModule();

    void *p;
    char *s;

    _asm mov edx, offset GetCallAddress
    denomo_emit(0xe8); //call $ + 5
    denomo_emit(0x0);
    denomo_emit(0x0);
    denomo_emit(0x0);
    denomo_emit(0x0);
    _asm call edx
    _asm mov p, eax

	CPPInfo._new = p;
	s = new char;

    _asm mov edx, offset GetCallAddress
    denomo_emit(0xe8); //call $ + 5
    denomo_emit(0x0);
    denomo_emit(0x0);
    denomo_emit(0x0);
    denomo_emit(0x0);
    _asm call edx
    _asm mov p, eax

	CPPInfo._delete = p;
	delete s;

    _asm mov edx, offset GetCallAddress
    denomo_emit(0xe8); //call $ + 5
    denomo_emit(0x0);
    denomo_emit(0x0);
    denomo_emit(0x0);
    denomo_emit(0x0);
    _asm call edx
    _asm mov p, eax

	CPPInfo._new_array = p;
	s = new char[10];

    _asm mov edx, offset GetCallAddress
    denomo_emit(0xe8); //call $ + 5
    denomo_emit(0x0);
    denomo_emit(0x0);
    denomo_emit(0x0);
    denomo_emit(0x0);
    _asm call edx
    _asm mov p, eax

	CPPInfo._delete_array = p;
	delete[] s;

    if(CPPInfo._new_array == CPPInfo._new)
        CPPInfo._new_array = NULL;
    if(CPPInfo._delete_array == CPPInfo._delete)
        CPPInfo._delete_array = NULL;

	CPPInfo.malloc = &malloc;
	CPPInfo.calloc = &calloc;
	CPPInfo.realloc = &realloc;
	CPPInfo.free = &free;
	CPPInfo._expand = &_expand;
#ifdef DENOMO_DBG_MM
	CPPInfo._malloc_dbg = &_malloc_dbg;
	CPPInfo._calloc_dbg = &_calloc_dbg;
	CPPInfo._realloc_dbg = &_realloc_dbg;
	CPPInfo._free_dbg = &_free_dbg;
	CPPInfo._expand_dbg = &_expand_dbg;
#else
	CPPInfo._malloc_dbg = NULL;
	CPPInfo._calloc_dbg = NULL;
	CPPInfo._realloc_dbg = NULL;
	CPPInfo._free_dbg = NULL;
	CPPInfo._expand_dbg = NULL;
#endif

#ifdef DENOMO_BC
    InitBCBInfo();
#endif
}

static int DenomoInited = 0;
static void calltype InitDenomo(int callfromdll)
{
	if(DenomoInited)
		return;

	DenomoInited = 1;

	CPPService = NULL;
	ProcGetCPPService AddrGetCPPService = NULL;
	if(callfromdll != 0) {
#ifdef STATIC_DLL
		AddrGetCPPService = &GetCPPService;
#endif
	}
	if(!AddrGetCPPService) {
		HMODULE DllHandle;
		const char *DenomoDll = "DenomoCPP.dll";
		char *dll = (char *)DenomoDll;
		DllHandle = LoadLibraryA(dll);
		if(DllHandle) {
			AddrGetCPPService = (ProcGetCPPService)GetProcAddress(DllHandle, "GetCPPService");
			if(!AddrGetCPPService) {
				ErrorOrExit("Can't find function GetCPPService in dll DenomoCPP.dll", "Error load DLL", 1);
			}
		}
		else {
			ErrorOrExit("Can't find dll DenomoCPP.dll", "Error load DLL", 1);
		}
	}
	if(AddrGetCPPService)
		AddrGetCPPService(&CPPService);
	if(!CPPService)
		return;

	CPPService->AddRef();

	ZeroMemory(&CPPInfo, sizeof(TCPPInfo));
	CPPInfo.AddrGetMemBlockSize = (void *)&GetMemBlockSize;

	InitCPPInfo();

	CPPService->InitDenomoCPP(&CPPInfo);
}

static void calltype DeInitDenomo() {
	if(!DenomoInited)
		return;
	DenomoInited = 0;
	
	if(!CPPService)
		return;
	CPPService->DeInitDenomoCPP();
}

class DenomoInit
{
public:
	static DenomoInit *Denomo;

	DenomoInit() {
		InitDenomo(0);
	}

	~DenomoInit() {
		DeInitDenomo();
	}
private:
};

static DenomoInit Denomo;

#endif

