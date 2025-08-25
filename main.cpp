#include "LawnApp.h"
#include "Resources.h"
#include "Sexy.TodLib/TodStringFile.h"
#include <cstdio>
#include <windows.h> // <-- Important: Add this for OutputDebugStringA
#include <string>

using namespace Sexy;

bool (*gAppCloseRequest)();				
bool (*gAppHasUsedCheatKeys)();			
SexyString (*gGetCurrentLevelName)();

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

	TodStringListSetColors(gLawnStringFormats, gLawnStringFormatCount);
	gGetCurrentLevelName = LawnGetCurrentLevelName;
	gAppCloseRequest = LawnGetCloseRequest;
	gAppHasUsedCheatKeys = LawnHasUsedCheatKeys;

	gLawnApp = new LawnApp();
	gLawnApp->mChangeDirTo = (!Sexy::FileExists("properties\\resources.xml") && Sexy::FileExists("..\\properties\\resources.xml")) ? ".." : ".";
	
	gLawnApp->Init();
	gLawnApp->Start();

	gLawnApp->Shutdown();
	if (gLawnApp)
		delete gLawnApp;

	return 0;
};