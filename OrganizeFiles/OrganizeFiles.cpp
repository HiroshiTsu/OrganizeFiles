// OrganizeFiles.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <iostream>
#include <algorithm>
#include <filesystem> // std::tr2::sys::path etc.
#include <string>
#include <locale>
#include <cstdio>
#include <algorithm> 
#include <functional> 
#include <cctype>

using namespace std;

std::wstring trim(const std::wstring& string, const wchar_t* trimCharacterList = L" \t\v\r\n　")
{
	std::wstring result;

	// 左側からトリムする文字以外が見つかる位置を検索します。
	std::wstring::size_type left = string.find_first_not_of(trimCharacterList);

	if (left != std::wstring::npos)
	{
		// 左側からトリムする文字以外が見つかった場合は、同じように右側からも検索します。
		std::wstring::size_type right = string.find_last_not_of(trimCharacterList);

		// 戻り値を決定します。ここでは右側から検索しても、トリムする文字以外が必ず存在するので判定不要です。
		result = string.substr(left, right - left + 1);
	}

	return result;
}


int main() {
	namespace sys = std::tr2::sys;
	wcout.imbue(locale("japanese"));
	sys::path p("."); // 列挙の起点
	std::for_each(sys::directory_iterator(p), sys::directory_iterator(),
		//  再帰的に走査するならコチラ↓
		//  std::for_each(sys::recursive_directory_iterator(p), sys::recursive_directory_iterator(),
		[](const sys::path& p) {
		if (sys::is_regular_file(p)) { // ファイルなら...
			wstring strFn = p.filename().wstring();    //カレントディレクトリにあるファイルの名前
			size_t  intLocA = strFn.find_first_of(L"第", 0);     //放送大学の「第」を探索

			//講義の何回目を求める
			size_t  intLocB = strFn.find_first_of(L"回", 0);
			if (intLocA != -1 && intLocB != -1 && (intLocB > intLocA) &&  ( (intLocB - intLocA) < 4))
			{
				wstring strSub = strFn.substr(intLocA + 1, intLocB - intLocA - 1);
				if (strSub.length() < 2) {
					wstring strLeft = L"00";
					strLeft.append(strSub);
					strSub = strLeft.substr(strLeft.length() - 2, 2);
					strFn = strFn.substr(0, intLocA + 1).append(strSub).append(strFn.substr(intLocB, strFn.length() - intLocB));
					_wrename(p.filename().wstring().c_str(), strFn.c_str());
				}
				wstring strDirName = trim(strFn.substr(0, intLocA));
				struct _stat64 fileInfo;

				//struct stat info;

				//if (stat(pathname, &info) != 0)
				//	printf("cannot access %s\n", pathname);
				//else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
				//	printf("%s is a directory\n", pathname);
				//else
				//	printf("%s is no directory\n", pathname);

				if ((_wstat64(strDirName.c_str(), &fileInfo) != 0) || (fileInfo.st_mode & S_IFDIR))
				{
					sys::create_directory(strDirName.c_str());
					sys::permissions(strDirName.c_str(), sys::perms::remove_perms | sys::perms::others_all);
				}

				wstring strFullName = strDirName.append(L"\\").append(strFn.c_str());
				while (1)
				{
					if ((_wstat64(strFullName.c_str(), &fileInfo) == 0)) {
						size_t  intLocC = strFullName.find_last_of(L".");
						if (intLocC == -1)
						{
							intLocC = strFullName.length();
						}
						wstring strFullName2 = strFullName.substr(0, intLocC ).append(L"G");
					    wstring strFullName3 = strFullName2.append(strFullName.substr(intLocC, strFullName.length() - intLocC));
						strFullName = strFullName3;
					}
					else
					{
						break;
					}
				}
				_wrename(strFn.c_str(), strFullName.c_str());


			}

			struct _stat64 fileInfo;

			if (_wstat64(strFn.c_str(), &fileInfo) != 0) {  // Use stat( ) to get the info
				std::cerr << "Error: " << strerror(errno) << '\n';
				return(EXIT_FAILURE);
			}

			wcout << "file: " << strFn << endl;

			wcout << "Size          : " <<
				fileInfo.st_size << '\n';               // Size in bytes
			wcout << "Device        : " <<
				(char)(fileInfo.st_dev + 'A') << '\n';  // Device number
			wcout << "Created       : " <<
				std::ctime(&fileInfo.st_ctime);         // Creation time
			wcout << "Modified      : " <<
				std::ctime(&fileInfo.st_mtime);         // Last mod time
			
		}
		else if (sys::is_directory(p)) { // ディレクトリなら...
			wcout << "dir.: " << p.wstring() << endl;
		}
	});
	getchar();
}