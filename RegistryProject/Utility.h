#pragma once
// C
#include <wtypes.h>

// C++
#include <iostream>
#include <string_view>

/** 레지스트리 키의 경로를 구성하는 함수
* key : 기본 경로
* subKey : 추가 경로*/
std::wstring FullKeyPath(const std::wstring_view wsvKey, const std::wstring_view wsvSubKey)
{
    std::basic_string<TCHAR> tempString = wsvKey.data();
    tempString += wsvSubKey;
    return tempString;
}