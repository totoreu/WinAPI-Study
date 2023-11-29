#pragma once

const HKEY SHLM = HKEY_LOCAL_MACHINE;    // HKEY_LOCAL_MACHINE
const HKEY SHCU = HKEY_CURRENT_USER;     // HKEY_CURRENT_USER
const HKEY SHCR = HKEY_CLASSES_ROOT;     // HKEY_CLASSES_ROOT

UINT RegReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault)
{
    HKEY key;
    DWORD dwDisp;
    UINT Result;
    DWORD Size;
    if (RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp)
        != ERROR_SUCCESS)
    {
        return 0;
    }
    Size = sizeof(LONG);
    if (RegQueryValueEx(hKey, lpValue, 0, NULL, (LPBYTE)&Result, &Size)
        != ERROR_SUCCESS)
    {
        Result = nDefault;
    }
    RegCloseKey(key);
    return Result;
}

BOOL RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPCTSTR lpRet, DWORD nSize)
{
    HKEY key;
    DWORD dwDisp;
    DWORD Size;
    if (RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    Size = nSize;
    if (RegQueryValueEx(key, lpValue, 0, NULL, (LPBYTE)lpRet, &Size)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    RegCloseKey(key);
    return TRUE;
}

BOOL RegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData)
{
    HKEY key;
    DWORD dwDisp;
    if (RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    if (RegSetValueEx(hKey, lpValue, 0, REG_DWORD, (LPBYTE)&nData, sizeof(UINT))
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    RegCloseKey(key);
    return TRUE;
}

BOOL RegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
    HKEY key;
    DWORD dwDisp;
    if (RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    if (RegSetValueEx(hKey, lpValue, 0, REG_SZ, (LPBYTE)&lpData, lstrlen(lpData) + 1)
        != ERROR_SUCCESS)
    {
        return FALSE;
    }
    RegCloseKey(key);
    return TRUE;
}