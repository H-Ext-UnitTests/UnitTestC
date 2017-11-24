#include "stdafx.h"
#include "try_throw_catch.h"

// List of API support

// List of EXTs API support for all modes

//#define EXT_IUTIL
//#define EXT_ICOMMAND
//#define EXT_ICINIFILE
//#define EXT_ITIMER
//#define EXT_HKTIMER

// List of EXTs API support for mp mode only

//#define EXT_IHALOENGINE
//#define EXT_IOBJECT
//#define EXT_IPLAYER         // if define EXT_IADMIN, EXT_IPLAYER test will not process
//#define EXT_IADMIN          // Require EXT_IUTIL

// Not included in this UnitTest.
//#define EXT_IDATABASE
//#define EXT_IDATABASESTATEMENT
//#define EXT_HKDATABASE

//Future API support

//#define EXT_INETWORK              // Will require mp mode test and possible client?
//#define EXT_ISOUND                // Require client side test only.
//#define EXT_IDIRECTX9             // Require client side test only
//#define EXT_HKEXTERNAL            // TBD

#ifdef EXT_IPLAYER
#include <time.h>
typedef struct tm tm;
#endif

#include "UnitTestC.h"
#include "..\Add-on API\Add-on API.h"
#pragma comment(lib, "../Add-on API/Add-on API.lib")

addon_info EXTPluginInfo = { L"Unit Test C", L"1.0.0.0",
                            L"DZS|All-In-One, founder of DZS",
                            L"Used for verifying each API are working right in C language under C99 standard.",
                            L"UnitTest",
                            L"unit_test",
                            L"test_unit",
                            L"unit test",
                            L"[unit]test",
                            L"test[unit]" };
UINT EAOhashID = 0;

/*
 * Verification list as of 0.5.3.3
 *
 * EXT_IHALOENGINE          - Passed (except a few functions are not included in test.)
 * EXT_IOBJECT              - Passed (except a few functions are not included in test.)
 * EXT_IPLAYER              - Passed (except a few functions are not included in test.)
 * EXT_IADMIN               - Passed
 * EXT_ICOMMAND             - Passed
 * EXT_IDATABASE            - Not included in this UnitTest.
 * EXT_IDATABASESTATEMENT   - Not included in this UnitTest.
 * EXT_HKDATABASE           - Not included in this UnitTest.
 * EXT_ICINIFILE            - Passed
 * EXT_ITIMER               - Passed (Expect imbalance tick synchronize for 1/30 ticks per second after first load time.)
 * EXT_HKTIMER              - Passed
 * EXT_IUTIL                - Passed (except a few functions are not included in test.)
 * Future API support
 * EXT_INETWORK
 * EXT_ISOUND
 * EXT_IDIRECTX9
 * EXT_HKEXTERNAL
 */

//List of API pointer if defined one of above.
PlayerInfo plINull = { 0 };
#ifdef EXT_IUTIL
IUtil* pIUtil = 0;
#endif
#ifdef EXT_ITIMER
ITimer* pITimer = 0;
UINT TimerID[4] = { 0 };
LONGLONG TimerTickStart = 0;
LONGLONG TimerTickSys[4] = { 0 };
#ifdef ITIMER_LOOP_CHECK
UINT TimerIDLoop = 0;
DWORD TimerTickLoop = 0;
#endif
#endif
#ifdef EXT_ICINIFILE
ICIniFile* pICInifile = 0;
#endif
#ifdef EXT_ICOMMAND
ICommand* pICommand = 0;
CMD_RETURN WINAPIC eao_testExecute(PlayerInfo plI, ArgContainer* arg, MSG_PROTOCOL protocolMsg, unsigned int timerID, bool* showChat) {
    return CMD_SUCCESS;
}
CMD_RETURN WINAPIC eao_testExecuteOverride(PlayerInfo plI, ArgContainer* arg, MSG_PROTOCOL protocolMsg, unsigned int timerID, bool* showChat) {
    return CMD_SUCCESS;
}
CMD_RETURN WINAPIC eao_testExecuteOverride2(PlayerInfo plI, ArgContainer* arg, MSG_PROTOCOL protocolMsg, unsigned int timerID, bool* showChat) {
    return CMD_SUCCESS;
}
#endif
#ifdef EXT_IPLAYER
IPlayer* pIPlayer = 0;
#endif
#ifdef EXT_IOBJECT
IObject* pIObject = 0;
#endif
#ifdef EXT_IADMIN
IAdmin* pIAdmin = 0;
#endif
#ifdef EXT_IHALOENGINE
IHaloEngine* pIHaloEngine = 0;
#endif

bool compareString(const wchar_t* str1, const wchar_t* str2, unsigned int length) {
    if (length == -1) {
        length = 0;
        while(str1[length] != 0 && str2[length] != 0) {
            if (str1[length] != str2[length])
                return 0;
            length++;
        }
    } else {
        do {
            length--;
            if (str1[length] != str2[length])
                return 0;
        } while (length);
    }
    return 1;
}

BOOL s_use_qpc = FALSE;
LARGE_INTEGER s_frequency;
long long milliseconds_now() {
    if (s_use_qpc) {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000LL * now.QuadPart) / s_frequency.QuadPart;
    } else {
        return GetTickCount();
    }
}


dllAPI EAO_RETURN WINAPIC EXTOnEAOLoad(UINT hash) {
    EAOhashID = hash;
    wchar_t testStrW[0x30] = L"Master \\\\Chief `love` 'biofoam' \\canister.";
    char testStrA[0x30] = "Master \\\\Chief `love` 'biofoam' \\canister.";
    e_boolean boolean;
    unsigned int retCode;
    wchar_t* strPtrW = NULL;
    s_use_qpc = QueryPerformanceFrequency(&s_frequency);
    TRY {
#ifdef EXT_IUTIL
        pIUtil = getIUtil(hash);
        if (!pIUtil)
            THROW(1);
        wchar_t* utilMemStrW = pIUtil->m_allocMem(ARRAYSIZE(testStrW) * 2);
        char* utilMemStrA = pIUtil->m_allocMem(ARRAYSIZE(testStrW));
        if (!utilMemStrW)
            THROW(1);
        utilMemStrW[0] = 0;
        utilMemStrA[0] = 0;
        if (!pIUtil->m_strcatW(utilMemStrW, ARRAYSIZE(testStrW), testStrW))
            THROW(1);
        if (!pIUtil->m_strcmpW(testStrW, utilMemStrW))
            THROW(1);
        if (!pIUtil->m_strcatA(utilMemStrA, ARRAYSIZE(testStrA), testStrA))
            THROW(1);
        if (!pIUtil->m_strcmpA(testStrA, utilMemStrA))
            THROW(1);
#pragma region boolean values
        if (pIUtil->m_strToBooleanW(testStrW) != BOOL_FAIL)
            THROW(1);
        if (pIUtil->m_strToBooleanA(testStrA) != BOOL_FAIL)
            THROW(1);
        if (pIUtil->m_strToBooleanW(trueW) != BOOL_TRUE)
            THROW(1);
        if (pIUtil->m_strToBooleanW(TRUEW) != BOOL_TRUE)
            THROW(1);
        if (pIUtil->m_strToBooleanW(trUeW) != BOOL_TRUE)
            THROW(1);
        if (pIUtil->m_strToBooleanW(trueNumW) != BOOL_TRUE)
            THROW(1);
        if (pIUtil->m_strToBooleanA(trueA) != BOOL_TRUE)
            THROW(1);
        if (pIUtil->m_strToBooleanA(TRUEA) != BOOL_TRUE)
            THROW(1);
        if (pIUtil->m_strToBooleanA(trUeA) != BOOL_TRUE)
            THROW(1);
        if (pIUtil->m_strToBooleanA(trueNumA) != BOOL_TRUE)
            THROW(1);
        if (pIUtil->m_strToBooleanW(falseW) != BOOL_FALSE)
            THROW(1);
        if (pIUtil->m_strToBooleanW(FALSEW) != BOOL_FALSE)
            THROW(1);
        if (pIUtil->m_strToBooleanW(faLseW) != BOOL_FALSE)
            THROW(1);
        if (pIUtil->m_strToBooleanW(falseNumW) != BOOL_FALSE)
            THROW(1);
        if (pIUtil->m_strToBooleanA(falseA) != BOOL_FALSE)
            THROW(1);
        if (pIUtil->m_strToBooleanA(FALSEA) != BOOL_FALSE)
            THROW(1);
        if (pIUtil->m_strToBooleanA(faLseA) != BOOL_FALSE)
            THROW(1);
        if (pIUtil->m_strToBooleanA(falseNumA) != BOOL_FALSE)
            THROW(1);
#pragma endregion
#pragma region team values
        if (pIUtil->m_strToTeamW(testStrW) != COLOR_TEAM_NONE)
            THROW(1);
        if (pIUtil->m_strToTeamA(testStrA) != COLOR_TEAM_NONE)
            THROW(1);
        if (pIUtil->m_strToTeamW(blueW) != COLOR_TEAM_BLUE)
            THROW(1);
        if (pIUtil->m_strToTeamW(BLUEW) != COLOR_TEAM_BLUE)
            THROW(1);
        if (pIUtil->m_strToTeamW(btW) != COLOR_TEAM_BLUE)
            THROW(1);
        if (pIUtil->m_strToTeamA(blueA) != COLOR_TEAM_BLUE)
            THROW(1);
        if (pIUtil->m_strToTeamA(BLUEA) != COLOR_TEAM_BLUE)
            THROW(1);
        if (pIUtil->m_strToTeamA(btA) != COLOR_TEAM_BLUE)
            THROW(1);
        if (pIUtil->m_strToTeamW(redW) != COLOR_TEAM_RED)
            THROW(1);
        if (pIUtil->m_strToTeamW(REDW) != COLOR_TEAM_RED)
            THROW(1);
        if (pIUtil->m_strToTeamW(rtW) != COLOR_TEAM_RED)
            THROW(1);
        if (pIUtil->m_strToTeamA(redA) != COLOR_TEAM_RED)
            THROW(1);
        if (pIUtil->m_strToTeamA(REDA) != COLOR_TEAM_RED)
            THROW(1);
        if (pIUtil->m_strToTeamA(rtA) != COLOR_TEAM_RED)
            THROW(1);
#pragma endregion
#pragma region Strings values verification
        pIUtil->m_toCharA(utilMemStrW, ARRAYSIZE(testStrA), utilMemStrA);
        if (!pIUtil->m_strcmpA(utilMemStrA, testStrA))
            THROW(1);
        pIUtil->m_toCharW(utilMemStrA, ARRAYSIZE(testStrA), utilMemStrW);
        if (!pIUtil->m_strcmpW(utilMemStrW, testStrW))
            THROW(1);
        utilMemStrA[3] = 'T';
        utilMemStrW[3] = L'T';
        if (!pIUtil->m_stricmpA(utilMemStrA, testStrA))
            THROW(1);
        if (!pIUtil->m_stricmpW(utilMemStrW, testStrW))
            THROW(1);
        utilMemStrA[3] = 't';
        utilMemStrW[3] = L't';
        if (!pIUtil->m_stricmpW(lettersW, lettersW2))
            THROW(1);
        if (!pIUtil->m_stricmpA(lettersA, lettersA2))
            THROW(1);
        if (pIUtil->m_strcmpW(lettersW, lettersW2))
            THROW(1);
        if (pIUtil->m_strcmpA(lettersA, lettersA2))
            THROW(1);
        if (pIUtil->m_stricmpW(numbersW, numbersW2))
            THROW(1);
        if (pIUtil->m_stricmpA(numbersA, numbersA2))
            THROW(1);
        if (pIUtil->m_strcmpW(numbersW, numbersW2))
            THROW(1);
        if (pIUtil->m_strcmpA(numbersA, numbersA2))
            THROW(1);
        boolean = pIUtil->m_shiftStrW(utilMemStrW, 1, 3, 1, 0);
        if (boolean != BOOL_TRUE)
            THROW(1);
        if (!pIUtil->m_findSubStrFirstW(utilMemStrW, MatterW))
            THROW(1);
        if (pIUtil->m_findSubStrFirstW(utilMemStrW, MattarW))
            THROW(1);
        boolean = pIUtil->m_shiftStrW(utilMemStrW, 1, 1, 3, 1);
        if (boolean != BOOL_TRUE)
            THROW(1);
        if (!pIUtil->m_findSubStrFirstW(utilMemStrW, MattarW))
            THROW(1);
        if (pIUtil->m_findSubStrFirstW(utilMemStrW, MatterW))
            THROW(1);
        boolean = pIUtil->m_shiftStrA(utilMemStrA, 1, 3, 1, 0);
        if (boolean != BOOL_TRUE)
            THROW(1);
        if (!pIUtil->m_findSubStrFirstA(utilMemStrA, MatterA))
            THROW(1);
        if (pIUtil->m_findSubStrFirstA(utilMemStrA, MattarA))
            THROW(1);
        boolean = pIUtil->m_shiftStrA(utilMemStrA, 1, 1, 3, 1);
        if (boolean != BOOL_TRUE)
            THROW(1);
        if (!pIUtil->m_findSubStrFirstA(utilMemStrA, MattarA))
            THROW(1);
        if (pIUtil->m_findSubStrFirstA(utilMemStrA, MatterA))
            THROW(1);

        utilMemStrW[0] = 0;
        utilMemStrA[0] = 0;
        retCode = pIUtil->m_strcatW(utilMemStrW, ARRAYSIZE(testStrA), MattarW);
        if (retCode != 6)
            THROW(1);
        retCode = pIUtil->m_strcatA(utilMemStrA, ARRAYSIZE(testStrA), MatterA);
        if (retCode != 6)
            THROW(1);
        if (!pIUtil->m_strcmpW(utilMemStrW, MattarW))
            THROW(1);
        if (!pIUtil->m_strcmpA(utilMemStrA, MatterA))
            THROW(1);
        retCode = pIUtil->m_strcatW(utilMemStrW, ARRAYSIZE(testStrA), replaceBeforeW);
        if (retCode != 13)
            THROW(1);
        retCode = pIUtil->m_strcatA(utilMemStrA, ARRAYSIZE(testStrA), replaceBeforeA);
        if (retCode != 13)
            THROW(1);
        if (!pIUtil->m_strcmpW(utilMemStrW, MattarReplaceBeforeW))
            THROW(1);
        if (!pIUtil->m_strcmpA(utilMemStrA, MatterReplaceBeforeA))
            THROW(1);
#pragma endregion
#pragma region isLetters, isFloat, isDouble, isNumbers, and isHash
        if (!pIUtil->m_isLettersW(lettersW))
            THROW(1);
        if (pIUtil->m_isLettersW(hashW))
            THROW(1);
        if (!pIUtil->m_isLettersA(lettersA))
            THROW(1);
        if (pIUtil->m_isLettersA(hashA))
            THROW(1);

        if (!pIUtil->m_isNumbersW(numbersW))
            THROW(1);
        if (pIUtil->m_isNumbersW(hashW))
            THROW(1);
        if (pIUtil->m_isNumbersW(floatW))
            THROW(1);
        if (!pIUtil->m_isNumbersA(numbersA))
            THROW(1);
        if (pIUtil->m_isNumbersA(hashA))
            THROW(1);
        if (pIUtil->m_isNumbersA(floatA))
            THROW(1);

        if (!pIUtil->m_isDoubleW(doubleW))
            THROW(1);
        if (!pIUtil->m_isDoubleW(numbersW))
            THROW(1);
        if (pIUtil->m_isDoubleW(hashW))
            THROW(1);
        if (!pIUtil->m_isDoubleA(doubleA))
            THROW(1);
        if (!pIUtil->m_isDoubleA(numbersA))
            THROW(1);
        if (pIUtil->m_isDoubleA(hashA))
            THROW(1);

        if (!pIUtil->m_isFloatW(floatW))
            THROW(1);
        if (pIUtil->m_isFloatW(doubleW))
            THROW(1);
        if (!pIUtil->m_isFloatW(numbersW))
            THROW(1);
        if (pIUtil->m_isFloatW(hashW))
            THROW(1);
        if (!pIUtil->m_isFloatA(floatA))
            THROW(1);
        if (pIUtil->m_isFloatA(doubleA))
            THROW(1);
        if (!pIUtil->m_isFloatA(numbersA))
            THROW(1);
        if (pIUtil->m_isFloatA(hashA))
            THROW(1);

        if (!pIUtil->m_isHashW(hashW))
            THROW(1);
        if (pIUtil->m_isHashW(floatW))
            THROW(1);
        if (!pIUtil->m_isHashA(hashA))
            THROW(1);
        if (pIUtil->m_isHashA(floatA))
            THROW(1);
#pragma endregion
#pragma region file & directory check
        if (!pIUtil->m_isDirExist(dirExtension, &retCode))
            THROW(1);
        if (retCode)
            THROW(1);
        if (pIUtil->m_isDirExist(dirExtesion, &retCode))
            THROW(1);
        if (!retCode)
            THROW(1);
        if (pIUtil->m_isDirExist(fileHExt, &retCode))
            THROW(1);
        if (!retCode)
            THROW(1);
        if (!pIUtil->m_isFileExist(fileHExt, &retCode))
            THROW(1);
        if (retCode)
            THROW(1);
        if (pIUtil->m_isFileExist(fileHEt, &retCode))
            THROW(1);
        if (!retCode)
            THROW(1);
        if (pIUtil->m_isFileExist(dirExtension, &retCode))
            THROW(1);
        if (!retCode)
            THROW(1);
#pragma endregion 
#pragma region Replace & undo relative + database regex replace.
        pIUtil->m_replaceW(replaceTestW);
        if (!pIUtil->m_strcmpW(replaceTestW, replaceAfterW))
            THROW(1);
        pIUtil->m_replaceUndoW(replaceTestW);
        if (!pIUtil->m_strcmpW(replaceTestW, replaceBeforeW))
            THROW(1);
        pIUtil->m_replaceA(replaceTestA);
        if (!pIUtil->m_strcmpA(replaceTestA, replaceAfterA))
            THROW(1);
        pIUtil->m_replaceUndoA(replaceTestA);
        if (!pIUtil->m_strcmpA(replaceTestA, replaceBeforeA))
            THROW(1);

        pIUtil->m_regexReplaceW(regexTestNoDB, 0);
        if (!pIUtil->m_strcmpW(regexTestNoDB, regexTestNoDBAfter))
            THROW(1);
        pIUtil->m_regexReplaceW(regexTestDB, 1);
        if (!pIUtil->m_strcmpW(regexTestDB, regexTestDBAfter))
            THROW(1);

        //regex test
        if (!pIUtil->m_regexMatchW(Unit_Test, wildcard))
            THROW(1);
        if (!pIUtil->m_regexMatchW(Unit_Test, wildcardBeginUnit))
            THROW(1);
        if (!pIUtil->m_regexMatchW(Unit_Test, wildcardEndTest))
            THROW(1);
        if (!pIUtil->m_regexMatchW(unit_test, wildcard))
            THROW(1);
        if (pIUtil->m_regexMatchW(unit_test, wildcardBeginUnit))
            THROW(1);
        if (pIUtil->m_regexMatchW(unit_test, wildcardEndTest))
            THROW(1);
        if (pIUtil->m_regexMatchW(unit_test, dotdotdot))
            THROW(1);

        if (!pIUtil->m_regexMatchW(hi_, dotdotdot))
            THROW(1);
        if (!pIUtil->m_regexiMatchW(hi_, dotdotdot))
            THROW(1);
        if (!pIUtil->m_regexiMatchW(Unit_Test, wildcard))
            THROW(1);
        if (!pIUtil->m_regexiMatchW(Unit_Test, wildcardBeginUnit))
            THROW(1);
        if (!pIUtil->m_regexiMatchW(Unit_Test, wildcardEndTest))
            THROW(1);
        if (!pIUtil->m_regexiMatchW(unit_test, wildcardBeginUnit))
            THROW(1);
        if (!pIUtil->m_regexiMatchW(unit_test, wildcardEndTest))
            THROW(1);
        if (pIUtil->m_regexiMatchW(unit_test, dotdotdot))
            THROW(1);
#pragma endregion
#pragma region formatVar___ functions
        VARIANT testVariant[11];
        wchar_t outputStr[0x512];
        VARIANTstr(&testVariant[0], "Aa");
        VARIANTwstr(&testVariant[1], L"aA");
        VARIANTfloat(&testVariant[2], 1.0000f);
        VARIANTdouble(&testVariant[3], 2.000002);
        VARIANTbool(&testVariant[4], 1);
        VARIANTshort(&testVariant[5], 25);
        VARIANTushort(&testVariant[6], 50);
        VARIANTint(&testVariant[7], MAXINT);
        VARIANTuint(&testVariant[8], MAXUINT);
        VARIANTlong(&testVariant[9], MAXLONG);
        VARIANTulong(&testVariant[10], MAXULONG32);
        if (!pIUtil->m_formatVariantW(outputStr, 0x512, variantFormat, 11, testVariant))
            THROW(1);
        if (!pIUtil->m_strcmpW(variantFormatExpected, outputStr))
            THROW(1);
#pragma endregion
        pIUtil->m_freeMem(utilMemStrW);
        pIUtil->m_freeMem(utilMemStrA);
        MessageBoxW(NULL, L"IUtil API has passed unit test.", L"PASSED - IUtil", MB_OK | MB_ICONINFORMATION);
#endif
#ifdef EXT_ICINIFILE
        pICInifile = getICIniFile(hash);
        if (!pICInifile)
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_open_file, iniFileStr)) {
            if (!CALL_MEMBER_FN(pICInifile, m_delete_file, iniFileStr))
                THROW(3);
            if (CALL_MEMBER_FN(pICInifile, m_open_file, iniFileStr))
                THROW(3);
        }
        if (!CALL_MEMBER_FN(pICInifile, m_create_file, iniFileStr))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_open_file, iniFileStr))
            THROW(3);

        retCode = 0;
        recheckICIniFileDataExists:
        if (CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name1))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name2))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name3))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name4))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name5))
            THROW(3);

        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name1, str1_0))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name2, str1_1))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name3, str1_0))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name4, str1_2))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name5, str1_3))
            THROW(3);

        if (!CALL_MEMBER_FN(pICInifile, m_value_set, EXTPluginInfo.sectors.sect_name1, str1_0, firstUnitTestCStr))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_value_set, EXTPluginInfo.sectors.sect_name2, str1_1, firstUnitTestCStr))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_value_set, EXTPluginInfo.sectors.sect_name3, str1_0, firstUnitTestCStr))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_value_set, EXTPluginInfo.sectors.sect_name4, str1_2, firstUnitTestCStr))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_value_set, EXTPluginInfo.sectors.sect_name5, str1_3, firstUnitTestCStr))
            THROW(3);
        retCode++;
        switch (retCode) {
            case 1: {
                if (!CALL_MEMBER_FN(pICInifile, m_load))
                    THROW(3);
                goto recheckICIniFileDataExists;
            }
            case 2: {
                CALL_MEMBER_FN(pICInifile, m_clear);
                if (!CALL_MEMBER_FN(pICInifile, m_save))
                    THROW(3);
                if (!CALL_MEMBER_FN(pICInifile, m_load))
                    THROW(3);
                goto recheckICIniFileDataExists;
            }
            default: break;
        }

        if (!CALL_MEMBER_FN(pICInifile, m_save))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_load))
            THROW(3);

        if (!CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name1))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name2))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name3))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name4))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name5))
            THROW(3);

        if (!CALL_MEMBER_FN(pICInifile, m_section_delete, EXTPluginInfo.sectors.sect_name3))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name3))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_section_add, EXTPluginInfo.sectors.sect_name3))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_section_exist, EXTPluginInfo.sectors.sect_name3))
            THROW(3);


        if (!CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name1, str1_0))
            THROW(3);

        if (!CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name2, str1_1))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name3, str1_0))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name4, str1_2))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name5, str1_3))
            THROW(3);


        if (!CALL_MEMBER_FN(pICInifile, m_value_set, EXTPluginInfo.sectors.sect_name1, str1_0, firstUnitTestCStr))
            THROW(3);
        if (!CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name1, str1_0))
            THROW(3);

        if (!CALL_MEMBER_FN(pICInifile, m_save))
            THROW(3);

        if (!CALL_MEMBER_FN(pICInifile, m_key_delete, EXTPluginInfo.sectors.sect_name1, str1_0))
            THROW(3);
        if (CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name1, str1_0))
            THROW(3);

        if (!CALL_MEMBER_FN(pICInifile, m_load))
            THROW(3);

        if (!CALL_MEMBER_FN(pICInifile, m_key_exist, EXTPluginInfo.sectors.sect_name1, str1_0))
            THROW(3);

        retCode = 0;
        if (!CALL_MEMBER_FN(pICInifile, m_content, &strPtrW, &retCode))
            THROW(3);
        if (!(strPtrW && retCode))
            THROW(3);

        if ((ARRAYSIZE(iniFileDataStr) - 1) != retCode)
            THROW(3);
        retCode++;

        if (!compareString(strPtrW, iniFileDataStr, retCode))
            THROW(3);

        // Begin 0.5.3.4 Feature
        wchar_t section_name[INIFILESECTIONMAX];
        wchar_t key_name[INIFILEKEYMAX];
        wchar_t value_name[INIFILEVALUEMAX];
        unsigned int ini_sec_count = CALL_MEMBER_FN(pICInifile, m_section_count);
        if (ini_sec_count != 3)
            THROW(3);

        unsigned int ini_key_count;
        // Section 0 test
        if (!CALL_MEMBER_FN(pICInifile, m_section_index, 0, section_name))
            THROW(3);
        if (!compareString(EXTPluginInfo.sectors.sect_name1, section_name, -1))
            THROW(3);
        ini_key_count = CALL_MEMBER_FN(pICInifile, m_key_count, section_name);
        if (ini_key_count != 1)
            THROW(3);

        // Section 0 key 0 test
        if (!CALL_MEMBER_FN(pICInifile, m_key_index, section_name, 0, key_name, value_name))
            THROW(3);
        if (!compareString(str1_0, key_name, -1))
            THROW(3);
        if (!compareString(firstUnitTestCStr, value_name, -1))
            THROW(3);

        // Section 1 test
        if (!CALL_MEMBER_FN(pICInifile, m_section_index, 1, section_name))
            THROW(3);
        if (!compareString(EXTPluginInfo.sectors.sect_name2, section_name, -1))
            THROW(3);
        ini_key_count = CALL_MEMBER_FN(pICInifile, m_key_count, section_name);
        if (ini_key_count != 1)
            THROW(3);

        // Section 1 key 0 test
        if (!CALL_MEMBER_FN(pICInifile, m_key_index, section_name, 0, key_name, value_name))
            THROW(3);
        if (!compareString(str1_1, key_name, -1))
            THROW(3);
        if (!compareString(firstUnitTestCStr, value_name, -1))
            THROW(3);

        // Section 2 test
        if (!CALL_MEMBER_FN(pICInifile, m_section_index, 2, section_name))
            THROW(3);
        if (!compareString(EXTPluginInfo.sectors.sect_name3, section_name, -1))
            THROW(3);
        ini_key_count = CALL_MEMBER_FN(pICInifile, m_key_count, section_name);
        if (ini_key_count != 0)
            THROW(3);


        // End 0.5.3.4 Feature

        if (!CALL_MEMBER_FN(pICInifile, m_delete_file, iniFileStr))
            THROW(3);

        CALL_MEMBER_FN(pICInifile, m_release);

        MessageBoxW(NULL, L"ICIniFile API has passed unit test.", L"PASSED - ICIniFile", MB_OK | MB_ICONINFORMATION);

#endif
#ifdef EXT_ICOMMAND
        pICommand = getICommand(hash);
        if (!pICommand)
            THROW(4);

        //TODO: need to re-review this function internally.
        if (pICommand->m_reload_level(hash))
            THROW(4);

        if (pICommand->m_delete(hash, eao_testExecute, eaoTestExecuteStr))
            THROW(4);
        if (pICommand->m_alias_delete(eaoTestExecuteStr, eaoTestExecuteAliasStr))
            THROW(4);
        if (!pICommand->m_add(hash, eaoTestExecuteStr, eao_testExecute, EXTPluginInfo.sectors.sect_name1, 1, 1, 0, modeAll))
            THROW(4);
        if (pICommand->m_add(hash, eaoTestExecuteStr, eao_testExecute, EXTPluginInfo.sectors.sect_name1, 1, 1, 0, modeAll))
            THROW(4);
        if (!pICommand->m_delete(hash, eao_testExecute, eaoTestExecuteStr))
            THROW(4);
        if (pICommand->m_delete(hash, eao_testExecute, eaoTestExecuteStr))
            THROW(4);
        if (!pICommand->m_add(hash, eaoTestExecuteStr, eao_testExecute, EXTPluginInfo.sectors.sect_name1, 1, 1, 1, modeAll))
            THROW(4);
        if (!pICommand->m_add(hash, eaoTestExecuteStr, eao_testExecuteOverride, EXTPluginInfo.sectors.sect_name1, 1, 1, 1, modeAll))
            THROW(4);
        if (pICommand->m_add(hash, eaoTestExecuteStr, eao_testExecuteOverride2, EXTPluginInfo.sectors.sect_name1, 1, 1, 1, modeAll))
            THROW(4);

        if (!pICommand->m_alias_add(eaoTestExecuteStr, eaoTestExecuteAliasStr))
            THROW(4);
        if (pICommand->m_alias_add(eaoTestExecuteStr, eaoTestExecuteAliasStr))
            THROW(4);
        if (!pICommand->m_alias_delete(eaoTestExecuteStr, eaoTestExecuteAliasStr))
            THROW(4);
        if (pICommand->m_alias_delete(eaoTestExecuteStr, eaoTestExecuteAliasStr))
            THROW(4);
        if (!pICommand->m_alias_add(eaoTestExecuteStr, eaoTestExecuteAliasStr))
            THROW(4);

        if (!pICommand->m_reload_level(hash))
            THROW(4);
        if (!pICommand->m_load_from_file(hash, eaoLoadFileStr, plINull, MP_RCON))
            THROW(4);

        // Proper remove command when done testing.
        if (!pICommand->m_delete(hash, eao_testExecute, eaoTestExecuteStr))
            THROW(4);

        MessageBoxW(NULL, L"ICommand API has passed unit test.", L"PASSED - ICommand", MB_OK | MB_ICONINFORMATION);
#endif
#ifdef EXT_IOBJECT
        pIObject = getIObject(hash);
        if (!pIObject)
            THROW(5);
        objTagGroupList gtag_list;
        if (!pIObject->m_get_lookup_group_tag_list(TAG_WEAP, &gtag_list))
            THROW(5);
        if (gtag_list.count == 0)
            THROW(5);
        if (gtag_list.tag_list[0]->group_tag != TAG_WEAP)
            THROW(5);
        hTagHeader* tag_header = pIObject->m_lookup_tag_type_name(TAG_WEAP, gtag_list.tag_list[0]->tag_name);
        if (!tag_header)
            THROW(5);
        if (tag_header != gtag_list.tag_list[0])
            THROW(5);
        s_ident object_id;
        s_ident parent_id = { -1 };
        objManaged move_object = { 0 };
        move_object.world.x = 1.0f;
        move_object.world.y = 1.0f;
        move_object.world.z = 1.0f;
        if (!pIObject->m_create(tag_header->ident, parent_id, 1000, &object_id, &move_object.world))
            THROW(5);
        s_object* created_object = pIObject->m_get_address(object_id);
        if (!created_object)
            THROW(5);

        tag_header = pIObject->m_lookup_tag(created_object->ModelTag);
        if (!tag_header)
            THROW(5);
        if (tag_header != gtag_list.tag_list[0])
            THROW(5);

        if (created_object->World.x != 1.0f &&created_object->World.y != 1.0f &&created_object->World.z != 1.0f)
            THROW(5);
        move_object.world.x = 2.0f;
        move_object.world.y = 2.0f;
        move_object.world.z = 2.0f;
        pIObject->m_move(object_id, move_object);
        if (created_object->World.x != 2.0f &&created_object->World.y != 2.0f &&created_object->World.z != 2.0f)
            THROW(5);
        move_object.world.x = 5.0f;
        move_object.world.y = 5.0f;
        move_object.world.z = 5.0f;
        pIObject->m_move_and_reset(object_id, &move_object.world);
        if (created_object->World.x != 5.0f &&created_object->World.y != 5.0f &&created_object->World.z != 5.0f)
            THROW(5);
        pIObject->m_update(object_id);
        if (!pIObject->m_destroy(object_id))
            THROW(5);
        //This step is required to prevent memory leaking since C does not support deconstructor.
        if (GlobalFree(gtag_list.tag_list))
            THROW(5);
        MessageBoxW(NULL, L"IObject API has passed unit test.", L"PASSED - IObject", MB_OK | MB_ICONINFORMATION);
#endif
#ifdef EXT_IPLAYER
#ifndef EXT_IADMIN
#ifndef EXT_IOBJECT
#pragma COMPILER_ERROR("EXT_IOBJECT is required for testing EXT_IPLAYER.")
#endif
        pIPlayer = getIPlayer(hash);
        if (!pIPlayer)
            THROW(6);

        PlayerInfoList plList;
        short totalPlayers = pIPlayer->m_get_str_to_player_list(L"*", &plList, NULL);
        if (totalPlayers == 0)
            THROW(6);

        PlayerInfo plITest = { 0 }, plITest2 = { 0 };
        if (pIPlayer->m_get_m_index(2, &plITest, 1))
            THROW(6);
        if (pIPlayer->m_get_m_index(1, &plITest, 1))
            THROW(6);
        if (!pIPlayer->m_get_m_index(0, &plITest, 1))
            THROW(6);
        if (pIPlayer->m_get_id(200, &plITest2))
            THROW(6);
        if (!pIPlayer->m_get_id(plITest.plR->PlayerIndex, &plITest2))
            THROW(6);
        if (!(plITest.mS == plITest2.mS && plITest.plEx == plITest2.plEx && plITest.plS == plITest2.plS && plITest.plR == plITest2.plR))
            THROW(6);

        s_biped* plBiped = (s_biped*)pIObject->m_get_address(plITest.plS->CurrentBiped);
        if (!plBiped)
            THROW(6);
        if (!pIPlayer->m_get_ident(plBiped->PlayerOwner,&plITest2))
            THROW(6);

        if (pIPlayer->m_get_by_unique_id(600, &plITest2))
            THROW(6);
        if (!pIPlayer->m_get_by_unique_id(plITest.mS->UniqueID, &plITest2))
            THROW(6);
        if (!(plITest.mS == plITest2.mS && plITest.plEx == plITest2.plEx && plITest.plS == plITest2.plS && plITest.plR == plITest2.plR))
            THROW(6);
        if (!(retCode = pIPlayer->m_get_id_full_name(plITest.plR->PlayerName)))
            THROW(6);
        if (!pIPlayer->m_get_full_name_id(retCode, testStrW))
            THROW(6);
        if (!(retCode = pIPlayer->m_get_id_ip_address(plITest.plEx->IP_Address)))
            THROW(6);
        if (!pIPlayer->m_get_ip_address_id(retCode, testStrW))
            THROW(6);
        if (!(retCode = pIPlayer->m_get_id_port(plITest.plEx->IP_Port)))
            THROW(6);
        if (!pIPlayer->m_get_port_id(retCode, testStrW))
            THROW(6);

        if (pIPlayer->m_update(&plINull))
            THROW(6);
        if (!pIPlayer->m_update(&plITest))
            THROW(6);

        VARIANT argVariant[1];
        VARIANTwstr(&argVariant[0], plITest.plR->PlayerName);
        if (!pIPlayer->m_send_custom_message(MF_BLANK, MP_CHAT, &plITest, L"Simple blank prefix message for {0:s}", 1, (VARIANT*)&argVariant))
            THROW(6);
        if (!pIPlayer->m_send_custom_message(MF_SERVER, MP_CHAT, &plITest, L"Simple server prefix message for {0:s}", 1, (VARIANT*)&argVariant))
            THROW(6);

        if (!pIPlayer->m_send_custom_message_broadcast(MF_BLANK, L"Simple blank prefix message for everyone", 0, NULL))
            THROW(6);
        if (!pIPlayer->m_send_custom_message_broadcast(MF_SERVER, L"Simple server prefix message for everyone", 0, NULL))
            THROW(6);

        //m_apply_camo test only required biped data to verify data is set to camoflauge.
        if (plBiped->isInvisible & 0x10)
            THROW(6);
        pIPlayer->m_apply_camo(&plITest, 10);
        if (!(plBiped->isInvisible & 0x10))
            THROW(6);

        e_color_team_index oldTeam = plITest.plR->Team;
        pIPlayer->m_change_team(&plITest, !oldTeam, 1);
        if (plITest.plR->Team == oldTeam)
            THROW(6);

        tm gmtm;
        time_t localTime = time(NULL);
        gmtime_s(&gmtm, &localTime);
        gmtm.tm_min += 5;
        if (!pIPlayer->m_ban_player(plITest.plEx, &gmtm))
            THROW(6);
        UINT banID, banID2;
        //Test CD hash key (un)ban verification
        if ((banID = pIPlayer->m_ban_CD_key_get_id(plITest.plEx->CDHashW)) == 0)
            THROW(6);
        if (!pIPlayer->m_unban_id(banID))
            THROW(6);
        //TODO: Does not validate if CD hash is valid first before ban
        if (pIPlayer->m_ban_CD_key(plITest.plEx->CDHashW, &gmtm) != 1)
            THROW(6);
        if ((banID2 = pIPlayer->m_ban_CD_key_get_id(plITest.plEx->CDHashW)) == 0)
            THROW(6);
        if (banID != banID2)
            THROW(6);
        //Test IP Address (un)ban verification
        if ((banID = pIPlayer->m_ban_ip_get_id(plITest.plEx->IP_Address)) == 0)
            THROW(6);
        if (!pIPlayer->m_unban_id(banID))
            THROW(6);
        //TODO: Does not validate if IP Address is valid first before ban
        if (pIPlayer->m_ban_ip(plITest.plEx->IP_Address, &gmtm) != 1)
            THROW(6);
        if ((banID2 = pIPlayer->m_ban_ip_get_id(plITest.plEx->IP_Address)) == 0)
            THROW(6);
        if (banID != banID2)
            THROW(6);

        IN_ADDR ipAddr;
        if (!pIPlayer->m_get_ip(plITest.mS, &ipAddr))
            THROW(6);
        if (!pIPlayer->m_get_port(plITest.mS, &totalPlayers))
            THROW(6);
        if (!pIPlayer->m_get_CD_hash(plITest.mS, cdHashKeyA))
            THROW(6);

        //Uncomment this part if need to verify function return correctly with/out an admin player.
        //if (!pIPlayer->m_is_admin(plITest.mS->machineIndex))
            //THROW(6);


        MessageBoxW(NULL, L"IPlayer API has passed unit test.", L"PASSED - IPlayer", MB_OK | MB_ICONINFORMATION);
#endif
#endif
#ifdef EXT_IADMIN
#ifndef EXT_IPLAYER
#pragma COMPILER_ERROR("EXT_IPLAYER is required for testing EXT_IADMIN.")
#endif
        pIPlayer = getIPlayer(hash);
        if (!pIPlayer)
            THROW(6);
        PlayerInfo plIMockUp;
        if (!pIPlayer->m_get_m_index(0, &plIMockUp, 1))
            THROW(6);

        pIAdmin = getIAdmin(hash);
        if (!pIAdmin)
            THROW(7);

        if (pIAdmin->m_is_username_exist(username) != BOOL_FALSE)
            THROW(7);
        if (pIAdmin->m_delete(username) != BOOL_FALSE)
            THROW(7);

        ArgContainer arg;
        CmdFunc func;
        short tmpLvl = plIMockUp.plEx->adminLvl;
        plIMockUp.plEx->adminLvl = 0;
        if (pIAdmin->m_is_authorized(&plIMockUp, cmdEaoLoad, &arg, &func) != AUTH_DENIED)
            THROW(7);
        plIMockUp.plEx->adminLvl = 9999;
        if (pIAdmin->m_is_authorized(&plIMockUp, cmdEaoLoad, &arg, &func) != AUTH_AUTHORIZED)
            THROW(7);
        if (pIAdmin->m_is_authorized(&plINull, cmdEaoLoadOutOfRange, &arg, &func) != AUTH_OUT_OF_RANGE)
            THROW(7);
        plIMockUp.plEx->adminLvl = tmpLvl;

        if (pIAdmin->m_add(plIMockUp.plEx->CDHashW, plIMockUp.plEx->IP_Address, L"0", username, username, 9999, 0, 0) != BOOL_TRUE)
            THROW(7);
        if (pIAdmin->m_add(plIMockUp.plEx->CDHashW, plIMockUp.plEx->IP_Address, L"0", username, username, 9999, 0, 0) != BOOL_FALSE)
            THROW(7);

        if (pIAdmin->m_is_username_exist(username) != BOOL_TRUE)
            THROW(7);

        if (pIAdmin->m_is_authorized(&plIMockUp, cmdEaoLoad, &arg, &func) != AUTH_AUTHORIZED)
            THROW(7);

        if (pIAdmin->m_login(&plIMockUp, MP_CHAT, usernamebad, username) != LOGIN_FAIL)
            THROW(7);

        if (pIAdmin->m_is_authorized(&plIMockUp, cmdEaoLoad, &arg, &func) != AUTH_DENIED)
            THROW(7);

        if (pIAdmin->m_login(&plIMockUp, MP_CHAT, username, username) != LOGIN_OK)
            THROW(7);

        if (pIAdmin->m_is_authorized(&plIMockUp, cmdEaoLoad, &arg, &func) != AUTH_AUTHORIZED)
            THROW(7);

        if (pIAdmin->m_delete(username) != BOOL_TRUE)
            THROW(7);

        if (pIAdmin->m_delete(username) != BOOL_FALSE)
            THROW(7);

        MessageBoxW(NULL, L"IAdmin API has passed unit test.", L"PASSED - IAdmin", MB_OK | MB_ICONINFORMATION);
#endif
#ifdef EXT_IHALOENGINE
        pIHaloEngine = getIHaloEngine(hash);
        if (!pIHaloEngine)
            THROW(8);
        /*TODO: Need find better test for serverHeader.
        if (pIHaloEngine->serverHeader->totalPlayers != 1)
            THROW(8);
        */
        if (!(pIHaloEngine->playerReserved->MachineIndex == 0 && pIHaloEngine->playerReserved->PlayerIndex == 0))
            THROW(8);

        if (pIHaloEngine->isDedi && pIHaloEngine->haloGameVersion == HV_CE) {
            if (pIHaloEngine->machineHeaderSize != 0xEC)
                THROW(8);
        } else {
            if (pIHaloEngine->machineHeaderSize != 0x60)
                THROW(8);
        }
        s_machine_slot* mSIndex = pIHaloEngine->machineHeader;
        if (!(mSIndex->machineIndex == 0 && !mSIndex->isAvailable && mSIndex->data1 && mSIndex->Unknown9 == 0x0007))
            THROW(8);
        mSIndex = (s_machine_slot*)((BYTE*)pIHaloEngine->machineHeader + pIHaloEngine->machineHeaderSize);
        if (!(mSIndex->machineIndex == -1 && !mSIndex->data1 && mSIndex->Unknown9 == 0x0000))
            THROW(8);

        if (pIHaloEngine->mapCurrent->head != MAKE_ID('head'))
            THROW(8);
        switch (pIHaloEngine->haloGameVersion) {
            case HV_TRIAL: {
                if (pIHaloEngine->mapCurrent->haloVersion != 0x00000006)
                    THROW(8);
                break;
            }
            case HV_CE: {
                if (pIHaloEngine->mapCurrent->haloVersion != 0x00000261)
                    THROW(8);
                break;
            }
            case HV_PC: {
                if (pIHaloEngine->mapCurrent->haloVersion != 0x00000007)
                    THROW(8);
                break;
            }
            case HV_UNKNOWN:
            default: {
                THROW(8);
            }
        }
        if ((*pIHaloEngine->mapTimeLimitPermament) != -1)
            if ((*pIHaloEngine->mapTimeLimitLive) != (*pIHaloEngine->mapTimeLimitPermament))
                THROW(8);
        if ((*pIHaloEngine->mapStatus)->upTime != (*pIHaloEngine->mapUpTimeLive))
            THROW(8);

        /*
         * m_dispatch_rcon
         */
        rconData rcon;
        rcon.unk = 0; //Always zero
        strcpy_s(rcon.msg, 10, rconTestStr);
        rcon.msg_ptr = rcon.msg;
        PlayerInfo pl1 = { 0 };
        pl1.mS = pIHaloEngine->machineHeader;
        pIHaloEngine->m_dispatch_rcon(&rcon, &pl1);
        /*
         * m_dispatch_player
         */
        chatData d;
        d.msg_ptr = playerChatTest;
        d.player = 0;
        d.type = CHAT_TEAM;
        // Gotta pass a pointer to the chatData struct
        DWORD d_ptr = (DWORD)&d;
        BYTE* packetBuffer = (BYTE*)malloc(4096 + 2 * ARRAYSIZE(playerChatTest));
        if (!packetBuffer)
            THROW(8);
        // Build the chat packet
        retCode = pIHaloEngine->m_build_packet(packetBuffer, 0, 0x0F, 0, &d_ptr, 0, 1, 0);
        pIHaloEngine->m_add_packet_to_player_queue(pl1.mS->machineIndex, packetBuffer, retCode, 1, 1, 0, 1, 3);
        free(packetBuffer);
        //
        pIHaloEngine->m_dispatch_player(&d, ARRAYSIZE(playerChatTest), &pl1);
        /*
         * m_dispatch_global
         */
        d.msg_ptr = globalChatTest;
        d.type = CHAT_GLOBAL;
        d_ptr = (DWORD)&d;
        packetBuffer = malloc(4096 + 2 * ARRAYSIZE(globalChatTest));
        if (!packetBuffer)
            THROW(8);
        // Build the chat packet
        retCode = pIHaloEngine->m_build_packet(packetBuffer, 0, 0x0F, 0, &d_ptr, 0, 1, 0);
        pIHaloEngine->m_add_packet_to_global_queue(packetBuffer, retCode, 1, 1, 0, 1, 3);
        free(packetBuffer);
        //
        pIHaloEngine->m_dispatch_global(&d, ARRAYSIZE(globalChatTest));
        //
        if (pIHaloEngine->isDedi) {
            if (!pIHaloEngine->m_map_next())
                THROW(8);
            if (!pIHaloEngine->m_set_idling())
                THROW(8);
        }
        if (!pIHaloEngine->m_send_reject_code(pIHaloEngine->machineHeader, REJECT_VIDEO_TEST))
            THROW(8);
        if (!pIHaloEngine->m_exec_command("sv_maplist"))
                THROW(8);
        wchar_t passwordW[8] = L"unitest";
        char passwordA[8] = "unitest";
        pIHaloEngine->m_set_server_password(passwordW);
        pIHaloEngine->m_get_server_password(passwordWGet);
        wchar_t* testW1 = passwordW, *testW2 = passwordWGet;
        do {
            if (testW1[0] != testW2[0])
                THROW(8);
            testW1++;
            testW2++;
        } while (testW1[0] && testW2[0]);

        pIHaloEngine->m_set_rcon_password(passwordA);
        pIHaloEngine->m_get_rcon_password(passwordAGet);
        char* testA1 = passwordA, *testA2 = passwordAGet;
        do {
            if (testA1[0] != testA2[0])
                THROW(8);
            testA1++;
            testA2++;
        } while (testA1[0] && testA2[0]);


        //Addon test section
        /*TODO: Both functions will not work in middle of load process.
        addon_info eaoInfo;
        if (!pIHaloEngine->m_ext_add_on_get_info_index(0, &eaoInfo))
            THROW(8);
        eaoInfo.author[0] = 0;
        eaoInfo.config_folder[0] = 0;
        if (!pIHaloEngine->m_ext_add_on_get_info_by_name(eaoInfo.name, &eaoInfo))
            THROW(8);
        */
        //TODO: This function cannot be tested otherwise it will go in a loop plus is not fully implemented yet.
        //if (!pIHaloEngine->m_ext_add_on_reload(EXTPluginInfo.name))
        //    THROW(8);




        MessageBoxW(NULL, L"IHaloEngine API has passed unit test.", L"PASSED - IHaloEngine", MB_OK | MB_ICONINFORMATION);
#endif
#ifdef EXT_I
        MessageBoxW(NULL, L" API has passed unit test.", L"PASSED - ", MB_OK | MB_ICONINFORMATION);
#endif
#ifdef EXT_ITIMER
        pITimer = getITimer(hash);
        if (!pITimer)
            THROW(2);
        TimerID[0] = pITimer->m_add(hash, 0, 0); //1/30 second
        if (!TimerID[0])
            THROW(2);
        TimerID[1] = pITimer->m_add(hash, 0, 60); //2 seconds
        if (!TimerID[1])
            THROW(2);
        pITimer->m_delete(hash, TimerID[1]);
#ifdef ITIMER_LOOP_CHECK
        TimerIDLoop = pITimer->m_add(hash, 0, 30);
        TimerTickLoop = milliseconds_now();
#endif
#endif
    } CATCH(1) {
        MessageBoxW(NULL, L"IUtil API has failed unit test.", L"ERROR - IUtil", MB_OK | MB_ICONERROR);
        return EAO_FAIL;
    } CATCH(2) {
        MessageBoxW(NULL, L"ITimer API has failed unit test.", L"ERROR - ITimer", MB_OK | MB_ICONERROR);
        return EAO_FAIL;
    } CATCH(3) {
        MessageBoxW(NULL, L"ICIniFile API has failed unit test.", L"ERROR - ICIniFile", MB_OK | MB_ICONERROR);
#ifdef EXT_ICINIFILE
        if (pICInifile)
            CALL_MEMBER_FN(pICInifile, m_release);
#endif
        return EAO_FAIL;
    } CATCH(4) {
        MessageBoxW(NULL, L"ICommand API has failed unit test.", L"ERROR - ICommand", MB_OK | MB_ICONERROR);
        return EAO_FAIL;
    } CATCH(5) {
        MessageBoxW(NULL, L"IObject API has failed unit test.", L"ERROR - IObject", MB_OK | MB_ICONERROR);
        return EAO_FAIL;
    } CATCH(6) {
        MessageBoxW(NULL, L"IPlayer API has failed unit test.", L"ERROR - IPlayer", MB_OK | MB_ICONERROR);
        return EAO_FAIL;
    } CATCH(7) {
        MessageBoxW(NULL, L"IAdmin API has failed unit test.", L"ERROR - IAdmin", MB_OK | MB_ICONERROR);
        return EAO_FAIL;
    } CATCH(8) {
        MessageBoxW(NULL, L"IHaloEngine API has failed unit test.", L"ERROR - IHaloEngine", MB_OK | MB_ICONERROR);
        return EAO_FAIL;
    } CATCH(9) {
        MessageBoxW(NULL, L" API has failed unit test.", L"ERROR - ", MB_OK | MB_ICONERROR);
        return EAO_FAIL;
    }
    ETRY;
    return EAO_CONTINUE;
}

dllAPI void WINAPIC EXTOnEAOUnload() {
    //TODO: Need to test something here?
}

#ifdef EXT_HKTIMER
dllAPI bool WINAPIC EXTOnTimerExecute(unsigned int id, unsigned int count) {
#ifdef ITIMER_LOOP_CHECK
    if (TimerIDLoop == id) {
        DWORD tempTimerTickLoop = milliseconds_now();
        VARIANT var = { 0 };
        VARIANTulong(&var, tempTimerTickLoop - TimerTickLoop);
        TimerTickLoop = tempTimerTickLoop;
        pIPlayer->m_send_custom_message(MF_INFO, MP_RCON, &plINull, L"{0:d} millisecond(s).", 1, &var);
        return 1; //This is needed to "mock up" else to avoid force failure below and tell H-Ext to repeat timer.
    } else
#endif
    if (TimerID[0] == id) {
        if (!TimerTickStart) {
            TimerTickStart = milliseconds_now();
            TimerID[0] = pITimer->m_add(EAOhashID, 0, 150); //5 seconds
            if (!TimerID[0])
                goto failedITimer;
            TimerID[2] = pITimer->m_add(EAOhashID, 0, 30); //1 second
            if (!TimerID[2])
                goto failedITimer;
        } else {
            TimerTickSys[0] = milliseconds_now();
            LONGLONG tmpTimerCheck = TimerTickSys[0] - TimerTickStart;
            if (!(4500 < tmpTimerCheck && tmpTimerCheck < 5033))
                goto failedITimer;
            if (TimerTickSys[1] != 0)
                goto failedITimer;
            tmpTimerCheck = TimerTickSys[2] - TimerTickStart;
            if (!(500< tmpTimerCheck && tmpTimerCheck < 1033))
                goto failedITimer;
            tmpTimerCheck = TimerTickSys[3] - TimerTickStart;
            if (!(2500 < tmpTimerCheck && tmpTimerCheck < 3033))
                goto failedITimer;
            MessageBoxW(NULL, L"ITimer API has passed unit test.", L"PASSED - ITimer", MB_OK | MB_ICONINFORMATION);
        }
    } else if (TimerID[1] == id) {
        TimerTickSys[1] = milliseconds_now();
    } else if (TimerID[2] == id) {
        TimerTickSys[2] = milliseconds_now();
        TimerID[3] = pITimer->m_add(EAOhashID, 0, 60); //2 seconds
        if (!TimerID[3])
            goto failedITimer;
    } else if (TimerID[3] == id) {
        TimerTickSys[3] = milliseconds_now();
    } else {
        failedITimer:
        MessageBoxW(NULL, L"ITimer API has failed unit test.", L"ERROR - ITimer", MB_OK | MB_ICONERROR);
    }
    return 0; //Tell H-Ext not to repeat timer.
}
dllAPI void WINAPIC EXTOnTimerCancel(unsigned int id) {
    if (TimerID[0] == id) {
    } else if (TimerID[1] == id) {
    } else if (TimerID[2] == id) {
    } else if (TimerID[3] == id) {
    } else {
        MessageBoxW(NULL, L"ITimer API has failed unit test.", L"ERROR - ITimer", MB_OK | MB_ICONERROR);
    }
}
#endif

#include "..\Add-on API\C\expChecker.h"
