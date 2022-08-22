#pragma once
#include<Windows.h>
#include<iostream>
#include<string>
#include<vector>
#define delay 100
bool isOn = false;
bool isPaused = false;
namespace OneKey {
	using namespace std;
	//	���ļ��а����� 
	void ChangeClipboard(std::string msg)
	{
		HWND hWnd = NULL;
		OpenClipboard(hWnd);
		EmptyClipboard();
		HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 1000);
		char* pData = (char*)GlobalLock(hHandle);
		//strcpy_s(pData, msg.length()+1,  msg.c_str());	// C++ 14
		strcpy(pData, msg.c_str());	//GCC 4.9.2
		SetClipboardData(CF_TEXT, hHandle);
		GlobalUnlock(hHandle);
		CloseClipboard();
	}
	
	//	��һ���� 
	void PressKey(int key)
	{
		INPUT input[2];
		memset(input, 0, sizeof(input));
		input[0].type = input[1].type = INPUT_KEYBOARD;
		input[0].ki.wVk = input[1].ki.wVk = key;
		input[1].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(2, input, sizeof(INPUT));
	}
	
	//	����ϼ� 
	void PressKey(int key1, int key2)
	{
		INPUT input[4];
		memset(input, 0, sizeof(input));
		input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
		input[0].ki.wVk = input[2].ki.wVk = key1;
		input[1].ki.wVk = input[3].ki.wVk = key2;
		input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(4, input, sizeof(INPUT));
	}
	
	//	�ڿ���ִ̨��һ������ 
	void ExcuteCmd(std::string cmd)
	{
		ChangeClipboard(cmd);
		PressKey(VK_F5);
		Sleep(delay);
		PressKey(VK_LCONTROL, 'V');
		Sleep(delay);
		PressKey(VK_RETURN);
		Sleep(delay);
		PressKey(VK_ESCAPE);
		Sleep(delay);
	}
	
	//	�ڿ���ִ̨��һ������ 
	void ExcuteCmd(vector<string> cmds)
	{
		PressKey(VK_F5);
		Sleep(delay);
		for (int i = 0; i < cmds.size(); i++)
		{
			ChangeClipboard(cmds[i]);
			std::cout << cmds[i] << std::endl;
			PressKey(VK_LCONTROL, 'V');
			Sleep(delay);
			PressKey(VK_RETURN);
			Sleep(delay);
		}
		PressKey(VK_ESCAPE);
		Sleep(delay);
	}

	//	��ʾ������Ϣ 
	void ShowHelp()
	{
		using namespace std;
		system("cls");
		cout << "\n  * ������Ϸ�� Ins ����/ͣ�� *\n\n   *  Code By ���ڳ鴫ͳ����  *\n" << endl;
		cout << " * Ins : ����/�˳� �ϵ�ģʽ\n * Home : ��ʾ������Ϣ\n * Del : ��ʾ��Ϸ�Դ����������Ϣ\n * PgUp : ��ͣ / �ָ� ��ͳ����\n * PgDn : �ٻ� ����" << endl;
		cout << " * End : ֱ���˳���ͳ����\n * K : ɱ����Χ��������\n * Z : ����/�˳� ����ģʽ\n * B : ����/�ر� ����������������\n * F1 : �ҵ�F1�������Բ����ù���\n * F2 : ��ʾ��ǰ������Ϣ" << endl;
		cout << " * F3 : �ٻ� ���ؾ���\n * F4 : �ٻ� ѩɽ����\n * F6 : �ٻ� ���þ���\n * F7 : �ٻ� �������\n * F8 : �ٻ� �������" << endl;
		cout << " * F9 : �ٻ� ��ԭ����\n * F10 : �ٻ� �Ұ��˷�ʦ��\n * F11 : �ٻ� ���鷨ʦ��" << endl;
	}

	//	�����������벢��Ӧ 
	LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		using namespace OneKey;
		KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
		DWORD code = ks->vkCode;
		/*
		* Ins 45  Home 36  PageUp 33  PageDown 34   Del 46  End 35
		* F1 112   F2 113   F3 114  F4 115  F5 116  F6 117   F7 118  F8 119  F9 120  F10 121  F11 122  F12 123
		*/
		if (wParam == WM_KEYUP)
		{
			//std::cout << code << std::endl;
			//Ins 45
			if (code == 45)
			{
				if (!isOn) //turn on
				{
					vector<string> v;
					v.push_back("devcommands");
					v.push_back("debugmode");

					ExcuteCmd(v);

					std::cout << "TURN ON" << std::endl;
					isOn = true;
					isPaused = false;
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				else  //turn off
				{
					vector<string> v;
					v.push_back("debugmode");
					v.push_back("devcommands");
					ExcuteCmd(v);
					std::cout << "TURN OFF" << std::endl;
					isPaused = true;
					isOn = false;
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
			}

			//Home 36 help
			if (code == 36)
			{
				ShowHelp();
			}

			//Del 46 commands help
			if (code == 46)
			{
				ExcuteCmd("help");
				PressKey(VK_F5);
			}

			//End 35 EXIT
			if (code == 35)
			{
				exit(0);
			}

			//PageUp 33 PAUSE
			if (code == 33)
			{
				if (!isPaused)
				{
					isPaused = true;
					std::cout << "PAUSE" << std::endl;
				}
				else
				{
					isPaused = false;
					std::cout << "RESUME" << std::endl;
				}
			}
			//check key
			if (isOn && !isPaused)
			{
				//std::cout << code << std::endl;
				// F1 112   F2 113   F3 114  F4 115  F5 116  F6 117   F7 118  F8 119  F9 120  F10 121  F11 122  F12 123
				
				//F3 114
				if (code == 114)
				{
					vector<string> v;
					v.push_back("spawn Goblin 5");
					v.push_back("spawn GoblinBrute 4");
					v.push_back("spawn GoblinArcher 3");
					v.push_back("spawn GoblinShaman 2");
					v.push_back("spawn Deathsquito 3");

					ExcuteCmd(v);
					std::cout << "�ٻ� ���ؾ���: \n��ͨ * 5��\nսʿ * 4��\n������ * 3��\n���� * 2��\nɱ�˷� * 3" << std::endl;
				}

				//F4 115
				if (code == 115)
				{
					vector<string> v;
					v.push_back("spawn Fenring 10");
					v.push_back("spawn Wolf 17");
					v.push_back("spawn Wolf_cub 3");
					v.push_back("spawn Hatchling 3");

					ExcuteCmd(v);
					std::cout << "�ٻ� ѩɽ����: \n���� * 10��\n˪�� * 3��\n��ͨ�� * 17��\n�Ǳ��� * 3" << std::endl;
				}

				//F6 117
				if (code == 117)
				{
					vector<string> v;
					v.push_back("spawn Skeleton 20");
					v.push_back("spawn spawn Spawner_Skeleton_respawn_30 15");
					v.push_back("spawn Skeleton_Poison 10");
					ExcuteCmd(v);
					std::cout << "�ٻ� ���þ���: \n��ͨ * 25��\nǿ�� * 15��\nʷʫ * 10" << std::endl;
				}


				//F7 118
				if (code == 118)
				{
					vector<string> v;
					v.push_back("spawn Draugr 8");
					v.push_back("spawn Draugr_Elite 5");
					v.push_back("spawn Draugr_Ranged 4");
					v.push_back("spawn Spawner_Draugr_respawn_30 5");
					v.push_back("spawn Blob 4");
					v.push_back("spawn BlobElite 3");
					v.push_back("spawn Ghost 5");
					v.push_back("spawn Wraith 5");
					ExcuteCmd(v);
					std::cout << "�ٻ� �������: \n��ͨʬ�� * 5��\nǿ��ʬ�� * 5��\nսʿʬ�� * 5��\n������ * 4��\nʷ��ķ * 4��\nǿ��ʷ��ķ * 3��\n���� * 5��\n�Ļ� * 5" << std::endl;
				}
				//F8 119
				if (code == 119)
				{
					vector<string> v;
					v.push_back("spawn Surtling 13");
					v.push_back("spawn Spawner_imp 13");
					ExcuteCmd(v);
					std::cout << "�ٻ� ������ţ�\n���� * 13 \nǿ������ * 13" << std::endl;
				}	
				// F9 120
				if (code == 120)
				{
					vector<string> v;
					v.push_back("spawn troll 10");
					v.push_back("spawn Boar 10");
					v.push_back("spawn Neck 10");
					v.push_back("spawn Greydwarf 10");
					v.push_back("spawn Greydwarf_Elite 10");
					v.push_back("spawn Greydwarf_Shaman 5");
					v.push_back("spawn Greyling 10");

					ExcuteCmd(v);
					std::cout << "�ٻ� ��ԭ����: \n��ħ * 10��\n�Ұ�����ͨ * 10��\nսʿ * 10��\n���� * 10��\n���� * 5��\n�� * 10��\n���� * 10" << std::endl;
				}
				//F10 121
				if (code == 121)
				{
					ExcuteCmd("spawn Greydwarf_Shaman 50");
					std::cout << "�ٻ� �Ұ������� * 50" << std::endl;
				}
				//F11 122
				if (code == 122)
				{
					ExcuteCmd("spawn Spawner_imp 20");
					std::cout << "�ٻ� ǿ������ * 20" << std::endl;
				}

				//PageDown 34   
				if (code == 34)
				{
					vector<string> v;
					v.push_back("spawn abomination 10");
					v.push_back("spawn StoneGolem 10");

					ExcuteCmd(v);
					std::cout << "���� ���� * 20" << std::endl;
				}

			}
		}
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

}
