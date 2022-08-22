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
	//	更改剪切板内容 
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
	
	//	按一个键 
	void PressKey(int key)
	{
		INPUT input[2];
		memset(input, 0, sizeof(input));
		input[0].type = input[1].type = INPUT_KEYBOARD;
		input[0].ki.wVk = input[1].ki.wVk = key;
		input[1].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(2, input, sizeof(INPUT));
	}
	
	//	按组合键 
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
	
	//	在控制台执行一条命令 
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
	
	//	在控制台执行一组命令 
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

	//	显示帮助信息 
	void ShowHelp()
	{
		using namespace std;
		system("cls");
		cout << "\n  * 进入游戏后按 Ins 启用/停用 *\n\n   *  Code By 还在抽传统香烟  *\n" << endl;
		cout << " * Ins : 进入/退出 上帝模式\n * Home : 显示帮助信息\n * Del : 显示游戏自带命令帮助信息\n * PgUp : 暂停 / 恢复 传统香烟\n * PgDn : 召唤 地雷" << endl;
		cout << " * End : 直接退出传统香烟\n * K : 杀死周围所有生物\n * Z : 进入/退出 飞行模式\n * B : 开启/关闭 建材无条件无消耗\n * F1 : 我的F1坏了所以不设置功能\n * F2 : 显示当前房间信息" << endl;
		cout << " * F3 : 召唤 北地军团\n * F4 : 召唤 雪山军团\n * F6 : 召唤 骷髅军团\n * F7 : 召唤 沼泽军团\n * F8 : 召唤 火焰军团" << endl;
		cout << " * F9 : 召唤 草原军团\n * F10 : 召唤 灰矮人法师团\n * F11 : 召唤 焰灵法师团" << endl;
	}

	//	监听键盘输入并响应 
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
					std::cout << "召唤 北地军团: \n普通 * 5、\n战士 * 4、\n弓箭手 * 3、\n萨满 * 2、\n杀人蜂 * 3" << std::endl;
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
					std::cout << "召唤 雪山军团: \n芬里 * 10、\n霜龙 * 3、\n普通狼 * 17、\n狼宝宝 * 3" << std::endl;
				}

				//F6 117
				if (code == 117)
				{
					vector<string> v;
					v.push_back("spawn Skeleton 20");
					v.push_back("spawn spawn Spawner_Skeleton_respawn_30 15");
					v.push_back("spawn Skeleton_Poison 10");
					ExcuteCmd(v);
					std::cout << "召唤 骷髅军团: \n普通 * 25、\n强化 * 15、\n史诗 * 10" << std::endl;
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
					std::cout << "召唤 沼泽军团: \n普通尸鬼 * 5、\n强化尸鬼 * 5、\n战士尸鬼 * 5、\n弓箭手 * 4、\n史莱姆 * 4、\n强化史莱姆 * 3、\n鬼魅 * 5、\n幽魂 * 5" << std::endl;
				}
				//F8 119
				if (code == 119)
				{
					vector<string> v;
					v.push_back("spawn Surtling 13");
					v.push_back("spawn Spawner_imp 13");
					ExcuteCmd(v);
					std::cout << "召唤 火焰军团：\n焰灵 * 13 \n强化焰灵 * 13" << std::endl;
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
					std::cout << "召唤 草原军团: \n巨魔 * 10、\n灰矮人普通 * 10、\n战士 * 10、\n宝宝 * 10、\n萨满 * 5、\n猪 * 10、\n蜥蜴 * 10" << std::endl;
				}
				//F10 121
				if (code == 121)
				{
					ExcuteCmd("spawn Greydwarf_Shaman 50");
					std::cout << "召唤 灰矮人萨满 * 50" << std::endl;
				}
				//F11 122
				if (code == 122)
				{
					ExcuteCmd("spawn Spawner_imp 20");
					std::cout << "召唤 强化焰灵 * 20" << std::endl;
				}

				//PageDown 34   
				if (code == 34)
				{
					vector<string> v;
					v.push_back("spawn abomination 10");
					v.push_back("spawn StoneGolem 10");

					ExcuteCmd(v);
					std::cout << "生成 地雷 * 20" << std::endl;
				}

			}
		}
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

}
