#pragma once
// C
#include <wtypes.h>

// C++
#include <iostream>
#include <string>

/** ������Ʈ�� Ű�� ��θ� �����ϴ� �Լ�
* key : �⺻ ���
* subKey : �߰� ���*/
const LPCTSTR FullKeyPath(const LPCTSTR key, const LPCTSTR subKey)
{
    std::basic_string<TCHAR> tempString = key;
    tempString += subKey;
    LPCTSTR result = tempString.c_str();
    return result;
}