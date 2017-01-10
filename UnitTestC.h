#ifdef EXT_IUTIL
//boolean test section
static const wchar_t trueW[] = L"true";
static const wchar_t TRUEW[] = L"TRUE";
static const wchar_t trUeW[] = L"trUe";
static const wchar_t trueNumW[] = L"1";
static const char trueA[] = "true";
static const char TRUEA[] = "TRUE";
static const char trUeA[] = "trUe";
static const char trueNumA[] = "1";
static const wchar_t falseW[] = L"false";
static const wchar_t FALSEW[] = L"FALSE";
static const wchar_t faLseW[] = L"faLse";
static const wchar_t falseNumW[] = L"0";
static const char falseA[] = "false";
static const char FALSEA[] = "FALSE";
static const char faLseA[] = "faLse";
static const char falseNumA[] = "0";

//team test section
static const wchar_t blueW[] = L"blue";
static const wchar_t BLUEW[] = L"BLUE";
static const wchar_t btW[] = L"bt";
static const char blueA[] = "blue";
static const char BLUEA[] = "BLUE";
static const char btA[] = "bt";
static const wchar_t redW[] = L"red";
static const wchar_t REDW[] = L"RED";
static const wchar_t rtW[] = L"rt";
static const char redA[] = "red";
static const char REDA[] = "RED";
static const char rtA[] = "rt";

//detect string test section
static const wchar_t lettersW[] = L"lEtteRs";
static const wchar_t lettersW2[] = L"LeTterS";
static const char lettersA[] = "lEtTeRs";
static const char lettersA2[] = "LeTtErS";
static const wchar_t numbersW[] = L"12348765";
static const wchar_t numbersW2[] = L"87651234";
static const char numbersA[] = "12348765";
static const char numbersA2[] = "87651234";
static const wchar_t floatW[] = L"1234.8765";
static const char floatA[] = "1234.8765";
static const wchar_t doubleW[] = L"1.2348765";
static const char doubleA[] = "1.2348765";
static const wchar_t hashW[] = L"87a651234";
static const char hashA[] = "876512z34";

static const wchar_t MatterW[] = L"Matter";
static const char MatterA[] = "Matter";
static const wchar_t MattarW[] = L"Mattar";
static const char MattarA[] = "Mattar";
static const wchar_t MattarReplaceBeforeW[] = L"MattarTest 'Foobar'";
static const char MatterReplaceBeforeA[] = "MatterTest 'Foobar'";

//directory and file test section
static const wchar_t dirExtension[] = L"extension";
static const wchar_t fileHExt[] = L"H-Ext.dll";
static const wchar_t dirExtesion[] = L"extesion";
static const wchar_t fileHEt[] = L"H-Et.dll";

static wchar_t replaceTestW[0x20] = L"Test 'Foobar'";
static const wchar_t replaceBeforeW[] = L"Test 'Foobar'";
static const wchar_t replaceAfterW[] = L"Test ''Foobar''";
static char replaceTestA[0x20] = "Test 'Foobar'";
static const char replaceBeforeA[] = "Test 'Foobar'";
static const char replaceAfterA[] = "Test ''Foobar''";

//regex test section
static wchar_t regexTestNoDB[40] = L"? *? {test} )(string]here[there";
static const wchar_t regexTestNoDBAfter[] = L". .*. \\{test\\} \\)\\(string\\]here\\[there";
static wchar_t regexTestDB[40] = L"? *? {test} )(string]here[there";
static const wchar_t regexTestDBAfter[] = L"_ %_ \\{test\\} \\)\\(string\\]here\\[there";
static const wchar_t wildcard[] = L".*";
static const wchar_t wildcardEndTest[] = L".*Test";
static const wchar_t wildcardBeginUnit[] = L"Unit .*";
static const wchar_t dotdotdot[] = L"...";
static const wchar_t hi_[] = L"Hi!";
static const wchar_t Unit_Test[] = L"Unit Test";
static const wchar_t unit_test[] = L"unit test";

//variant test section
static const wchar_t variantFormatExpected[] = L"慁 1.000000 2.000002 1 25 50 4294967295 2147483647 2147483647 4294967295 aA";
static const wchar_t variantFormat[] = L"{0:s} {2:f} {3:f} {4:hhd} {5:hd} {6:hu} {8:u} {7:d} {9:ld} {10:lu} {1:s}";
#endif
//ICIniFile test section
#ifdef EXT_ICINIFILE
static const wchar_t iniFileStr[] = L"UnitTestC.ini";
static const wchar_t firstUnitTestCStr[] = L"First Unit Test C";
static const wchar_t str1_0[] = L"1.0";
static const wchar_t str1_1[] = L"1.1";
static const wchar_t str1_2[] = L"1.2";
static const wchar_t str1_3[] = L"1.3";
static const wchar_t iniFileDataStr[] = L" [unit_test]\r\n 1.0=First Unit Test C\r\n [test_unit]\r\n 1.1=First Unit Test C\r\n [unit test]";
#endif

//ICommand test section
#ifdef EXT_ICOMMAND
static const wchar_t eaoTestExecuteStr[] = L"eao_test_execute";
static const wchar_t eaoTestExecuteAliasStr[] = L"testexec";
static const wchar_t eaoLoadFileStr[] = L"unit_test.txt";
#endif

//IPlayer test section
#ifdef EXT_IPLAYER
static char cdHashKeyA[0x60] = "";
#endif

//IAdmin test section
#ifdef EXT_IADMIN
static const wchar_t username[] = L"unittest";
static const wchar_t usernamebad[] = L"unittes";
static const wchar_t cmdEaoLoad[] = L"ext_addon_load unittest";
static const wchar_t cmdEaoLoadOutOfRange[] = L"ext_addon_load";
#endif

//IHaloEngine test section
#ifdef EXT_IHALOENGINE
static const char rconTestStr[] = "Rcon Test";
static const wchar_t playerChatTest[] = L"Player Chat Test";
static const wchar_t globalChatTest[] = L"Global Chat Test";
static wchar_t passwordWGet[8] = L"deadbed";
static char passwordAGet[8] = "deadbed";
#endif
