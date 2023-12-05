#pragma once
// C
#include <wtypes.h>

// C++
#include <iostream>
#include <string_view>

/** ������Ʈ�� Ű�� ��θ� �����ϴ� �Լ�
* key : �⺻ ���
* subKey : �߰� ���*/
std::wstring FullKeyPath(const std::wstring_view wsvKey, const std::wstring_view wsvSubKey)
{
    std::basic_string<TCHAR> tempString = wsvKey.data();
    tempString += wsvSubKey;
    return tempString;
}