#pragma once
#include <string>
using namespace std;

__declspec(dllexport) string readFile(const string & filePath, int i);				// i��°�� ���� �о����
__declspec(dllexport) string returnTime();											// ���� ��¥�� �ð��� ���ڿ��� ��ȯ
__declspec(dllexport) void writeFile(const string & filePath, const string & log);  // ���� ��� �α� ���뿡 ���� ������ �ָ� �α� ����


