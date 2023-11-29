#pragma once
// C
#include <wtypes.h>

// C++
#include <iostream>
#include <string>

/** 레지스트리 키의 경로를 구성하는 함수
* key : 기본 경로
* subKey : 추가 경로*/
const LPCTSTR FullKeyPath(const LPCTSTR key, const LPCTSTR subKey)
{
    std::basic_string<TCHAR> tempString = key;
    tempString += subKey;
    LPCTSTR result = tempString.c_str();
    return result;
}