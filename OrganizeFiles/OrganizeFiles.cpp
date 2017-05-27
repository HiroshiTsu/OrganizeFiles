// OrganizeFiles.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <iostream>
#include <algorithm>
#include <filesystem> // std::tr2::sys::path etc.
#include <string>
#include <locale>
#include <cstdio>

using namespace std;

int main() {
	namespace sys = std::tr2::sys;
	wcout.imbue(locale("japanese"));
	sys::path p("."); // �񋓂̋N�_
	std::for_each(sys::directory_iterator(p), sys::directory_iterator(),
		//  �ċA�I�ɑ�������Ȃ�R�`����
		//  std::for_each(sys::recursive_directory_iterator(p), sys::recursive_directory_iterator(),
		[](const sys::path& p) {
		if (sys::is_regular_file(p)) { // �t�@�C���Ȃ�...
			wstring strFn = p.filename().wstring();
			int intLocA = strFn.find_first_of(L"��", 0);

			int intLocB = strFn.find_first_of(L"��", 0);
			if (intLocA != -1 && intLocB != -1 && (intLocB > intLocA))
			{
				wstring strSub = strFn.substr(intLocA + 1, intLocB - intLocA - 1);
				if (strSub.length() < 2) {
					wstring strLeft = L"00";
					strLeft.append(strSub);
					strSub = strLeft.substr(strLeft.length() - 2, 2);
					strFn = strFn.substr(0, intLocA + 1).append(strSub).append(strFn.substr(intLocB, strFn.length() - intLocB));
				}
				_wrename(p.filename().wstring().c_str(), strFn.c_str());
			}
			wcout << "file: " << strFn << endl;
			
		}
		else if (sys::is_directory(p)) { // �f�B���N�g���Ȃ�...
			wcout << "dir.: " << p.wstring() << endl;
		}
	});
	getchar();
}