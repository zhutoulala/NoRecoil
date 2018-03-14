// NoRecoil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "windows.h"
#include <iostream>
#include <string>
#include <signal.h>
#include <random>

//with different weapons, there are different fire rate and recoil.
DWORD fireRate;



bool LeftMouseDown = true;
int leftMouseVKCode = 1;
int RecoilState = 4;       //default state is turned off;
std::string sWeaponName;
TCHAR windowsName[13] = { 'U', 'n', 'r', 'e', 'a', 'l', 'w', 'i', 'n', 'd', 'o', 'w', '\0' };

//LPCTSTR windowsName = L"UnrealWindow";

size_t iWeaponIndex = 0;
const size_t MAX_INDEX_WEAPON = 6;
const size_t MAX_INDEX_RECOIL = 31;
std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(-2.1,2.1);
std::uniform_real_distribution<double> distributionx(-1.1,1.1);
DWORD recoilTable[MAX_INDEX_WEAPON][MAX_INDEX_RECOIL] = {
	{ 24,24,24,24,24,24,24,24,24,24,24,28,28,28,28,29,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30 }, //AKM
	{ 20,21,22,21,22,22,23,22,23,23,24,24,25,25,25,25,26,27,28,29,30,32,34,34,35,34,35,34,35,34,35 }, //SCAR-L	
	{ 25,25,25,29,33,33,32,33,32,32,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30 }, //M16A4
	{ 21,21,21,21,21,21,21,21,21,23,23,24,23,24,25,25,26,27,27,32,31,31,31,31,31,31,31,32,32,32,35 }, //M416
	{ 18,19,18,19,18,19,19,21,23,24,23,24,23,24,23,24,23,24,23,24,23,24,24,25,24,25,24,25,24,25,24 }, //ump9
	{ 16,17,18,20,21,22,23,24,25,26,28,30,32,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34 } //UZI
};

void __stdcall RemoveRecoil()
{
	HWND foregroundWin;

	leftMouseVKCode = 1;
	while (1)
	{
		foregroundWin = GetForegroundWindow();
		if (foregroundWin == FindWindow(windowsName, 0) && RecoilState == 3)
		{
			size_t iCount = 0;
			while (LeftMouseDown)
			{
				
				mouse_event(1u, 0+distributionx(generator) , recoilTable[iWeaponIndex][iCount]+ distribution(generator), 0, 3u);
				if (iCount < MAX_INDEX_RECOIL) iCount++;
				Sleep(fireRate);
			}
		}
		Sleep(1u);
	}
}

void __stdcall KeyHandlerThread()
{
	while (1)
	{
		if (GetAsyncKeyState(38) < 0)           // F9 turns recoil reducer on. changed to up arrow
		{
			RecoilState = 3;
			Beep(0x320u, 0xC8u);
		}
		if (GetAsyncKeyState(40) < 0)           // F10 turns recoil reducer off. changed to down arrow
		{
			RecoilState = 4;
			Beep(0x64u, 0xC8u);
		}
		if (GetAsyncKeyState(96) < 0) {
			fireRate = 100u;
			sWeaponName = "AKM";
			iWeaponIndex = 0;
			RecoilState = 3;
			Beep(0x320u, 0xC8u);
		}
		if (GetAsyncKeyState(97) < 0) {
			fireRate = 96u;
			sWeaponName = "SCAR-L";
			iWeaponIndex = 1;
			RecoilState = 3;
			Beep(0x320u, 0xC8u);
		}
		if (GetAsyncKeyState(98) < 0) {
			fireRate = 75u;
			sWeaponName = "M16A4";
			iWeaponIndex = 2;
			RecoilState = 3;
			Beep(0x320u, 0xC8u);
		}
		if (GetAsyncKeyState(99) < 0) {
			fireRate = 86u;
			sWeaponName = "M416";
			iWeaponIndex = 3;
			RecoilState = 3;
			Beep(0x320u, 0xC8u);
		}
		if (GetAsyncKeyState(100) < 0) {
			fireRate = 92u;
			sWeaponName = "UMP9";
			iWeaponIndex = 4;
			RecoilState = 3;
			Beep(0x320u, 0xC8u);
		}
		if (GetAsyncKeyState(101) < 0)
		{
			fireRate = 48u;
			sWeaponName = "UZI";
			iWeaponIndex = 5;
			Beep(0x320u, 0xC8u);
		}
		LeftMouseDown = GetAsyncKeyState(leftMouseVKCode) < 0;
		Sleep(1u);
		
	}
}

bool bQuit = false;
void quit(int a) {
	bQuit = true;
}

int main() {
	SetConsoleTitle(_T("dev build with double recoil"));
	std::cout << "please select favorite weapon: [0-5]\n";
	std::cout << "0. AKM\n";
	std::cout << "1. SCAR-L\n";
	std::cout << "2. M16A4\n";
	std::cout << "3. M416\n";
	std::cout << "4. UMP9\n";
	std::cout << "5. UZI\n";
	/*
	id   weapon   fireRate   recoilUp
	-----------------------------
	 1    AKM      0.100s     8
	 2    SCAR-L   0.096s     7
	 3    M16A4    0.075s     7
	 4    M416     0.086s     7
	 5    UMP9     0.092s     5
	 6    SKS      0.090s     8
	 7    UZI      0.048s     5
	*/
	char choice;
	std::cin.get(choice);
	iWeaponIndex = choice - '0';
	if (iWeaponIndex >= MAX_INDEX_WEAPON) {
		std::cout << "please input a number between 0 and 5\n";
		return -1;
	}
	
	switch (iWeaponIndex) {
	case 0:
		fireRate = 100u;
		sWeaponName = "AKM";
		break;
	case 1:
		fireRate = 96u;
		sWeaponName = "SCAR-L";
		break;
	case 2:
		fireRate = 75u;
		sWeaponName = "M16A4";
		break;
	case 3:
		fireRate = 86u;
		sWeaponName = "M416";
		break;
	case 4:
		fireRate = 92u;
		sWeaponName = "UMP9";
		break;
	case 5:
		fireRate = 48u;
		sWeaponName = "UZI";
		break;

	default:
		std::cout << "Please the input a number between 0 to 5.\n";
		return -1;
	}

	std::cout << "Selected weapon: " << sWeaponName << std::endl;
	std::cout << "press 'up arrow' to enable, 'down arrow' to disable, 'Ctrl+C' to quit\n";
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RemoveRecoil, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)KeyHandlerThread, 0, 0, 0);

	signal(SIGINT, quit);
	while (!bQuit) { Sleep(1000u); }
}