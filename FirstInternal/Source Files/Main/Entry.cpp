// Entry.cpp : Defines the entry point for the DLL application.
#include "Includes.h"

DWORD WINAPI ThreadLoop(LPVOID lpParameter)
{
	//-------------------------------------------------------------------
	//					Initalize
	//-------------------------------------------------------------------
	CUtils::SetupConsole();
	CEngine *Engine = (CEngine*)CUtils::CaptureInterface("engine.dll", "VEngineClient014");
	IBaseClientDLL *Client = (IBaseClientDLL*)(CUtils::CaptureInterface("client_panorama.dll", "VClient018"));

	Vector Angles;
	Angles.x = 0;
	Angles.y = 0;
	Angles.z = 0;
	while (true) 
	{
		std::cout << Engine->GetMaxClients() << std::endl;
		std::cout << CUtils::CaptureInterface("engine.dll", "VEngineClient014") << std::endl;
		std::cout << CUtils::CaptureInterface("client_panorama.dll", "VClient018") << std::endl;
		Engine->SetViewAngles(Angles);
		system("CLS");
	}

	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, ThreadLoop, NULL, NULL, NULL);
        break;

	case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}

