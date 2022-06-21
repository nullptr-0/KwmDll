#pragma comment(lib,"KwmDll.lib")

void __declspec(dllexport) kwmEncS(const char* fn);

void __declspec(dllexport) kwmDecS(const char* fn);

void __declspec(dllexport) kwmEncD(const char* fn, const char* pswFn);

void __declspec(dllexport) kwmDecD(const char* fn, const char* pswFn);
