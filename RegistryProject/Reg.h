#pragma once
#include <string_view>

const HKEY SHLM = HKEY_LOCAL_MACHINE;    // HKEY_LOCAL_MACHINE
const HKEY SHCU = HKEY_CURRENT_USER;     // HKEY_CURRENT_USER
const HKEY SHCR = HKEY_CLASSES_ROOT;     // HKEY_CLASSES_ROOT

UINT RegReadInt(HKEY hKey, std::wstring_view wsvKey, std::wstring_view wsvValue, INT nDefault)
{
    HKEY key;
    DWORD dwDisp;
    UINT Result;
    DWORD Size;
    if (RegCreateKeyEx(hKey, wsvKey.data(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp)
        != ERROR_SUCCESS)
    {
        return 0;
    }
    Size = sizeof(LONG);
    if (RegQueryValueEx(hKey, wsvValue.data(), 0, NULL, (LPBYTE)&Result, &Size)
        != ERROR_SUCCESS)
    {
        Result = nDefault;
    }
    RegCloseKey(key);
    return Result;
}

BOOL RegReadString(HKEY hKey, std::wstring_view wsvKey, std::wstring_view wsvValue, std::wstring_view wsvDefault, std::wstring_view wsvRet, DWORD nSize)
{
    HKEY key;
    DWORD dwDisp;
    DWORD Size;
    if (RegCreateKeyEx(hKey, wsvKey.data(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    Size = nSize;
    if (RegQueryValueEx(key, wsvValue.data(), 0, NULL, (LPBYTE)wsvRet.data(), &Size)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    RegCloseKey(key);
    return TRUE;
}

BOOL RegWriteInt(HKEY hKey, std::wstring_view wsvKey, std::wstring_view wsvValue, UINT nData)
{
    HKEY key;
    DWORD dwDisp;
    if (RegCreateKeyEx(hKey, wsvKey.data(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    if (RegSetValueEx(hKey, wsvValue.data(), 0, REG_DWORD, (LPBYTE)&nData, sizeof(UINT))
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    RegCloseKey(key);
    return TRUE;
}

BOOL RegWriteString(HKEY hKey, std::wstring_view wsvKey, std::wstring_view wsvValue, std::wstring_view wsvData)
{
    HKEY key;
    DWORD dwDisp;
    if (RegCreateKeyEx(hKey, wsvKey.data(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    if (RegSetValueEx(hKey, wsvValue.data(), 0, REG_SZ, (LPBYTE)&wsvData, lstrlen(wsvData.data()) + 1)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    RegCloseKey(key);
    return TRUE;
}