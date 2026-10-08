#include "stdafx.h"

#define READ                FALSE
#define COOKIE_TIME_STR     FALSE
#define MAX_LEN             1024
#define FILTER_NAME         "Gatekeeper"
#define FLTR_AUTH           "Authorization:"
#define FLTR_PATH           "URL"
#define FLTR_HEAD           "WWW-Authenticate: Basic realm=\"Gatekeeper\""
#define FLTR_PASS           "Basic <REDACTED_BASE64_CREDENTIAL>" // "<REDACTED_USER_PASSWORD>"
#define KILL_HEAD           "\0"
#define WAIT2CLOSE          20000
#define VER_NUMBER          1.05 
stringsmap smCache0;
stringsmap smCache1;
stringsmap smFree0;
stringsmap smFree1;

HANDLE hThread1;
CRITICAL_SECTION cs;
DWORD aSwitch = 0;
DWORD dwLen = 1000;

char* szServer;
char* szDb;
char* szUid;
char* szPwd;
char* szSite;
char* szModName;
char szLastUpdate[1000] = {0};

unsigned long ulSleepTimer = 0;

bool stillProcessing = true;
bool NeedStrings = true;
bool SvrStrValid = false;
bool ReadyToGo = false;
bool HasFree = false;
bool HasNotFree = false;

char szOpen = '(';
char szClose = ')';
char szOpenSvr = '[';
char szCloseSvr = ']';
char szComma = ',';
char szSlash = '\\';
char szSpace = ' ';
#ifdef _DEBUG
    char szNumBuffer[100] = {0};
    int iTest = 0;
    char szInstance[30] = {0};
#endif

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


void PopulateUserMap()
{

#ifdef _DEBUG
    DWORD start = GetTickCount();
#endif    
    
    
    CADOBinding conn;
    int iLoopAuth = 0;
    int iLoopFree = 0;
    int iCount = 0;
    int iFreeCount = 0;
    int iAuthCount =0;
    bool blnFree = false;
    bool blnNotFree = false;
    if (conn.Open("SQLOLEDB", szServer, szDb, szUid, szPwd))
    {        
        CDataReturn data;

        if(conn.Execute(data.SQL(szSite, szLastUpdate).c_str(), &data, &data))
        {

#ifdef _DEBUG//
            OutputDebugString("gotData");
#endif//
            
            iCount = conn.RecordCount();
            
            iFreeCount = data.Number.Value;
            iAuthCount = (iCount - 1) - iFreeCount;
            if(iFreeCount > 0)
            {
                blnFree = true;
            }
            if(iAuthCount > 0)
            {
                blnNotFree = true;
            }
#ifdef _DEBUG//
            OutputDebugString("*******************");
            OutputDebugString("LastUpdateTime");
            OutputDebugString(szLastUpdate);
            OutputDebugString("data.UserAndPath.Value");
            OutputDebugString(data.UserAndPath.Value);
            OutputDebugString("*******************");
#endif//        

            if (lstrcmp(szLastUpdate, data.UserAndPath.Value) != 0)
            {
                lstrcpyn(szLastUpdate, data.UserAndPath.Value, (lstrlen(data.UserAndPath.Value) + 1));
                
                if(iCount > 1)
                {
                    conn.MoveNext();

                    if (aSwitch == 0)
                    {
                 
                        smCache1.FreeArray();
                        smFree1.FreeArray();

                        smCache1.MakeArray(iAuthCount);                         
                        smFree1.MakeArray(iFreeCount);
                        do
                        {
                
                            if (data.Number.Value == 0)
                            {
#ifdef _DEBUG//
                                OutputDebugString("NOT FREE");
                                OutputDebugString(data.UserAndPath.Value);
                                OutputDebugString("-------------------------------------");
#endif//
                                smCache1.SetItem(iLoopAuth, data.UserAndPath.Value);
                                iLoopAuth++;
                            }
                            else
                            {
#ifdef _DEBUG//
                                OutputDebugString("FREE");
                                OutputDebugString(data.UserAndPath.Value);
                                OutputDebugString("-------------------------------------");
#endif//
                                smFree1.SetItem(iLoopFree, data.UserAndPath.Value);
                                iLoopFree++;
                            }
                    
                        }while(conn.MoveNext());
                    
                        EnterCriticalSection(&cs);\
                        aSwitch = 1;
                        HasFree = blnFree;
                        HasNotFree = blnNotFree;
                        LeaveCriticalSection(&cs);

                        smCache0.FreeArray();
                        smFree0.FreeArray();
#ifdef _DEBUG//
                        OutputDebugString("Populted a cache1");   
#endif//
                    }
                    else // aSwitch not == 0
                    {
                    
                        smCache0.FreeArray();
                        smFree0.FreeArray();

                        smFree0.MakeArray(iFreeCount);
                        smCache0.MakeArray(iAuthCount);                         
                        do
                        {
                            if (data.Number.Value == 0)
                            {
#ifdef _DEBUG//
                                OutputDebugString("NOT FREE");
                                OutputDebugString(data.UserAndPath.Value);
                                OutputDebugString("-------------------------------------");
#endif//
                                smCache0.SetItem(iLoopAuth, data.UserAndPath.Value);
                                iLoopAuth++;
                            }
                            else
                            {
#ifdef _DEBUG//
                                OutputDebugString("FREE");
                                OutputDebugString(data.UserAndPath.Value);
                                OutputDebugString("-------------------------------------");
#endif//
                                smFree0.SetItem(iLoopFree, data.UserAndPath.Value);
                                iLoopFree++;
                            }

                        }while(conn.MoveNext());
                    
                        EnterCriticalSection(&cs);
                        aSwitch = 0;
                        HasFree = blnFree;
                        HasNotFree = blnNotFree;
                        LeaveCriticalSection(&cs);

                        smCache1.FreeArray();
                        smFree1.FreeArray();

                        OutputDebugString("Populted a cache0");
                    }// aSwith != 0
                    
                }// iCount > 1
                     
            }// strcmp buffer to lastupdate
            else
            {
#ifdef _DEBUG//
                OutputDebugString("Strings were the same");
#endif//
            }
             
        }
        else
        {
            OutputDebugString("The Sql statment failed.");
        }
        
          //TODO: OutputDebugString for error
        conn.Close();
    }
    else
    {
        OutputDebugString(stringbuilder("SQL Open Error: ").concat(conn.LastError.c_str()).c_str());     
    }

#ifdef _DEBUG//
        ZeroMemory(&szNumBuffer, lstrlen(szNumBuffer));
        itoa((int)(GetTickCount() - start), szNumBuffer, 10);
        OutputDebugString("Finished populateing Cache");
        OutputDebugString(szNumBuffer);
#endif//

}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


bool IsValidUser(char* codedPattern, char* szLocation)
{
#ifdef _DEBUG
    DWORD start = GetTickCount();
#endif


    bool foundUser = false;
    DWORD allocSize = 0;
    char* szCriteria;
    allocSize = lstrlen(codedPattern) + lstrlen(szLocation) + 1; //TODO: should retain in variable the length of the strings
    
    szCriteria = (char*)VirtualAlloc(NULL, allocSize, MEM_COMMIT, PAGE_READWRITE);
    
    lstrcpy(szCriteria, codedPattern); //TODO: should do a memcpy or lstrncpy so there is no added calculation of length
    strlwr(szLocation);
    lstrcat(szCriteria, szLocation);//TODO: should do a memcpy or lstrncpy so there is no added calculation of length

#ifdef _DEBUG
    OutputDebugString("szCriteria");
    OutputDebugString(szCriteria);
    OutputDebugString("szLocation");
    OutputDebugString(szLocation);
    OutputDebugString("szCodePattern");
    OutputDebugString(codedPattern);
#endif
    
    EnterCriticalSection(&cs);
    
    if (aSwitch == 0)
    {
        if (HasFree == true)
        {
            if (smFree0.Find(szLocation).start >= 0)
            {
                foundUser = true;
            }
        }
        if (HasNotFree == true)
        {             
            if (smCache0.Find(szCriteria).start >= 0)
            {
                foundUser = true;
            }
        }
    }
    else if (aSwitch == 1)
    {
        if (HasFree == true)
        {
            if (smFree1.Find(szLocation).start >= 0)
            {
                foundUser = true;
            }
        }
        
        if (HasNotFree == true)
        {
            if (smCache1.Find(szCriteria).start >= 0)
            {
                foundUser = true;
            }
        
        }
        
    }

   

    if (foundUser == false)
    {
        //TODO: should use the len(codedPattern) and compare to a static length value to do a first compare to see if they are the same len
        if (lstrcmp(FLTR_PASS, codedPattern) == NULL)
        {
            OutputDebugString("       _==/          i     i          \\==_");
            OutputDebugString("     /XX/            |\\___/|            \\XX\\");
            OutputDebugString("   /XXXX\\            |XXXXX|            /XXXX\\");
            OutputDebugString("  |XXXXXX\\_         _XXXXXXX_         _/XXXXXX|");
            OutputDebugString(" XXXXXXXXXXXxxxxxxxXXXXXXXXXXXxxxxxxxXXXXXXXXXXX");
            OutputDebugString("|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|");
            OutputDebugString("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
            OutputDebugString("|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|");
            OutputDebugString(" \\XXXXX/^^^^\\\"\\XXXXXXXXXXXXXXXXXXXXX/^^^^\\XXXXX/");
            OutputDebugString("  |XXX|       \\XXX/^^\\XXXXX/^^\\XXX/       |XXX|");
            OutputDebugString("    \\XX\\       \\X/    \\XXX/    \\X/       /XX/");
            OutputDebugString("       \"\\       \"      \\X/      \"      /\"");

            foundUser = true;
        }
    
    }          

    LeaveCriticalSection(&cs);
    VirtualFree(szCriteria, 0, MEM_RELEASE);


#ifdef _DEBUG
        ZeroMemory(&szNumBuffer, lstrlen(szNumBuffer));
        itoa((int)(GetTickCount() - start), szNumBuffer, 10);
        OutputDebugString("Validation of User");
        OutputDebugString(szNumBuffer);
#endif


    
    return foundUser;
}

////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

bool CheckStillProcessing()
{
    bool stillProcessingReturn = true;

    EnterCriticalSection(&cs);
    stillProcessingReturn = stillProcessing;
    LeaveCriticalSection(&cs);

    return stillProcessingReturn;
}

///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

int FindBetween(char* szStr, char szStart, char szEnd, char* szNewStr)
{
    bool FoundSecond = false;
    bool FoundFirst = false;
    int iLen = lstrlen(szStr);
    int iStart = 0;
    int iEnd = 0;
    char szIndex = szStart;
    ZeroMemory(szNewStr, strlen(szNewStr));

    
    for(int i = 0; i < iLen; i++)
    {
        if( szIndex == szStr[i] )
        {
            if(szIndex == szStart && (!FoundFirst) )
            {
                FoundFirst = true;
                iStart = i + 1;
                szIndex = szEnd;
            }
            else if(!FoundSecond )
            {                
                iEnd = i;
                FoundSecond = true;
            }
        }
    }
    
    if(iEnd > 0)
    {
        lstrcpyn(szNewStr, &szStr[iStart], (iEnd - iStart) + 1);
    }
#ifdef _DEBUG
    OutputDebugString(szNewStr);
#endif

    return iEnd;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void replaceFirst(char* szStr, char szFind, char szReplace)
{
    int iLen = lstrlen(szStr);
    bool isFirst = true;
    int iFound = 0;

    for(int i = 0; i < iLen; i++)
    {
        if( szFind == szStr[i] )
        {   
            if(isFirst == true)
            {
                szStr[i] = szReplace;
                isFirst = false;
                iFound = iFound++;
            }

        }

    }

    if(szReplace == '\\')// char(92) == szSlash  == \ 
    {
        if(iFound == 2)
        {
            SvrStrValid = true;
        }    
    }   
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void remove(char* szStr, char szKey)
{
    int iLen = lstrlen(szStr);
    int iLoc = 0;
    int iRemoved = 0;
    int iWillNull = 0;

    //TODO: below algors should be replaced with a pointer offset and not array member indexing
    for(int i = 0; i < iLen; i++)
    {
        if(szStr[i] != szKey)
        {
            szStr[iLoc] = szStr[i];
            iLoc++;
        
        }
        else
        {
            iRemoved++;
        }
    
    }

    iWillNull = iLen - iRemoved;
    
    //TODO: this should be replaced with a ZeroMemory
    for(int iLoop = iLen; iLoop >= iWillNull; iLoop--)
    {
        szStr[iLoop] = char(0);
    
    }

    szStr[iWillNull] = szComma;

}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


bool PopulateDllData(char* szDllStr)
{
#ifdef _DEBUG
    DWORD start = GetTickCount();
#endif

    bool bIsValid = false;
    int iOpenLoc = 0;
    int iCloseLoc = 0;
    int iLen = lstrlen(szDllStr);
    int iOSvrLoc = 0;
    int iCSvrLoc = 0;
    int iIndex = 0;
    int iCurrLoc = 0;

    char szIndex;    
    szIndex = szOpen;

    char* szDllInfo;
    char* szWaitTime;
    char* szPort;
    
    bool SvrIsValid = false;
    bool DbIsValid = false;
    bool UidIsValid = false;
    bool PwdIsValid = false;
    bool SiteIsValid = false;
    bool TimerIsValid = false;
    
    
    szDllInfo = (char*)VirtualAlloc(NULL, iLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szDllInfo, iLen);
    
    szServer = (char*)VirtualAlloc(NULL, iLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szServer, iLen);
    
    szUid = (char*)VirtualAlloc(NULL, iLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szUid, iLen);
    
    szPwd = (char*)VirtualAlloc(NULL, iLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szPwd, iLen);
    
    szSite = (char*)VirtualAlloc(NULL, iLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szSite, iLen);
    
    szWaitTime = (char*)VirtualAlloc(NULL, iLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szWaitTime, iLen);
    
    szPort = (char*)VirtualAlloc(NULL, iLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szPort, iLen);
    
    szDb = (char*)VirtualAlloc(NULL, iLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szDb, iLen);   
    
    iCloseLoc = FindBetween(szDllStr, szOpen, szClose, szDllInfo);
    if(iCloseLoc > 0)
    {
        
        
        remove(szDllInfo, szSpace);
        iLen = lstrlen(szDllInfo);
        szIndex = szOpenSvr;
        SvrIsValid = true;
        iCSvrLoc = FindBetween(szDllInfo, szOpenSvr, szCloseSvr, szServer);
        replaceFirst(szServer, szComma, szSlash);    
        
        //TODO: findbetween might want to do the malloc for your variables because it will know the length needed for each.
        
        iIndex = FindBetween(szDllInfo, szCloseSvr, szComma, szUid);
        if(iIndex > 0)
        {
            iCurrLoc = iCurrLoc + iIndex;
            iIndex = FindBetween(&szDllInfo[iIndex], szComma, szComma, szDb);

            if(iIndex > 0)
            {
                iCurrLoc = iCurrLoc + iIndex;
                DbIsValid = true;
                iIndex = FindBetween(&szDllInfo[iCurrLoc], szComma, szComma, szUid);
                if(iIndex > 0)
                {
                    iCurrLoc = iCurrLoc + iIndex;
                    UidIsValid = true;
                    iIndex = FindBetween(&szDllInfo[iCurrLoc], szComma, szComma, szPwd);
                    if(iIndex > 0)
                    {
                        iCurrLoc = iCurrLoc + iIndex;
                        PwdIsValid = true;    
                        iIndex = FindBetween(&szDllInfo[iCurrLoc], szComma, szComma, szSite);
                        if(iIndex > 0)
                        {
                            if(iIndex > 0)
                            {
                                iCurrLoc = iCurrLoc + iIndex;
                                iIndex = FindBetween(&szDllInfo[iCurrLoc], szComma, szComma, szPort);
                                if(iIndex > 0)
                                {
                                    iCurrLoc = iCurrLoc + iIndex;
                                    SiteIsValid = true;
                                    lstrcat(szSite, szPort);
                                    iIndex = FindBetween(&szDllInfo[iCurrLoc], szComma, szComma, szWaitTime);
                                    if(iIndex > 0)
                                    {

#ifdef _DEBUG//
                                        OutputDebugString("Timer");
                                        OutputDebugString(szWaitTime);
                                        OutputDebugString("---------");
#endif//
                                        TimerIsValid = true;
                                        ulSleepTimer = (atoi(szWaitTime)) * 1000;

                                    }   
                                }//szWait 
                            }//szPort
                        }//szSite
                    }//szPwd
                }//szUid
            }//szDB
        }// Uid used as buffer to get everything outside of [SvrStr]               
    }//szSvrStr
    
    if(TimerIsValid && SiteIsValid && PwdIsValid && UidIsValid && DbIsValid && SvrIsValid)
    {
        bIsValid = true;
    }
    else
    {
        bIsValid = false;
    }

    //free((char*)szWaitTime);
    VirtualFree(szWaitTime, iLen, MEM_RELEASE);
    //free((char*)szPort);
    VirtualFree(szPort, iLen, MEM_RELEASE);
    //free((char*)szDllInfo);
    VirtualFree(szDllInfo, iLen, MEM_RELEASE);

#ifdef _DEBUG
        ZeroMemory(&szNumBuffer, lstrlen(szNumBuffer));
        itoa((int)(GetTickCount() - start), szNumBuffer, 10);
        OutputDebugString("Done parsing string");
        OutputDebugString(szNumBuffer);
#endif

    return bIsValid;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

unsigned int __stdcall thread(void* vpVoid)
{
    ((vpVoid));
    DWORD ulStopTop = 0;
    
    do
    {
        NextPopulate:
        PopulateUserMap();

        ulStopTop = GetTickCount();

        do
        {
            if (CheckStillProcessing())
            {
                if ((GetTickCount() - ulStopTop) < ulSleepTimer)
                {
                    Sleep(100);
                }
                else
                {

                    goto NextPopulate;
                }
            }
            else
            {
                goto QuitThread;
            }
        } while (true);
    } while(true); 
   
    QuitThread:
    _endthreadex(0);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//RUNDLL32 example
//run in console, "rundll32 <dllname>,help"
//

int WINAPI __stdcall help(int argc, char** args)
{
    stringbuilder sb;

    sb.assign("This ISAPI Filter requires a specific naming to the DLL.\r\n");
    sb.concat("Please note the following requirements for the naming:\r\n\r\n");
    sb.concat("gatekeeper([sqlServer,sqlInstance,sqlPort],databaseName,uid,pwd,webSiteName,webSitePort,dbCheckTimeMS).dll\r\n\r\n");
    sb.concat("All symbols are required and the only optional parameters are the 'sqlInstance' and 'sqlPort'.\r\n");
    sb.concat("If you do no use the default for both then both must be supplied or not supplied at all.\r\n\r\n");
    sb.concat("Examples:\r\n");
    sb.concat("gatekeeper([localhost,isis,2123],<DB_NAME>,<DB_USER>,<DB_PASSWORD>,<APP_HOST>,80,5000).dll\r\n");
    sb.concat("gatekeeper([localhost],<DB_NAME>,<DB_USER>,<DB_PASSWORD>,localhost,8080,15000).dll");

    MessageBox(0, sb.c_str(), "Help", 32);

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//RUNDLL32 example
//run in console, "rundll32 <dllname>,help"
//

int WINAPI __stdcall GerVersion(int argc, char** args)
{
    stringbuilder sb;
    sb.assign(VER_NUMBER);
    MessageBox(0, sb.c_str(), "Help", 32);

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void Help(void)
{
    stringbuilder sb;

    sb.assign("This ISAPI Filter requires a specific naming to the DLL.\r\n");
    sb.concat("Please note the following requirements for the naming:\r\n\r\n");
    sb.concat("gatekeeper([sqlServer,sqlInstance,sqlPort],databaseName,uid,pwd,webSiteName,webSitePort,dbCheckTimeMS).dll\r\n\r\n");
    sb.concat("All symbols are required and the only optional parameters are the 'sqlInstance' and 'sqlPort'.\r\n");
    sb.concat("If you do no use the default for both then both must be supplied or not supplied at all.\r\n\r\n");
    sb.concat("Examples:\r\n");
    sb.concat("gatekeeper([localhost,isis,2123],<DB_NAME>,<DB_USER>,<DB_PASSWORD>,<APP_HOST>,80,5000).dll\r\n");
    sb.concat("gatekeeper([localhost],<DB_NAME>,<DB_USER>,<DB_PASSWORD>,localhost,8080,15000).dll");

    MessageBox(0, sb.c_str(), "Help", MB_OK || MB_TOPMOST || MB_SETFOREGROUND);
    
}

//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

int WINAPI __stdcall test(int argc, char** args)
{
    char* szTest = "C:\\path\\to\\gatekeeper([<DB_SERVER>,isis,1234],<DB_NAME>, <DB_USER>, <DB_PASSWORD>,<APP_HOST>,80,5).dll";
    
    PopulateDllData(szTest);
    return 0;
}

///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

DWORD WINAPI __stdcall HttpFilterProc(HTTP_FILTER_CONTEXT *pfc, DWORD dwNotificationType, LPVOID *lpNotificationData)
{
    
#ifdef _DEBUG
    DWORD start = GetTickCount();
#endif

    if (dwNotificationType == SF_NOTIFY_PREPROC_HEADERS)
    {
   
        {

#ifdef _DEBUG
        ZeroMemory(&szNumBuffer, lstrlen(szNumBuffer));
        itoa(iTest, szNumBuffer, 10);
        OutputDebugString("Global int");
        OutputDebugString(szNumBuffer);
        OutputDebugString("----------");
        iTest++;
#endif
            HTTP_FILTER_PREPROC_HEADERS* fpH;
            char szRetHead[MAX_LEN + 1];
            char szUrlLoc[(MAX_LEN * 2) + 1] = {0};
            unsigned long ulLenHead = MAX_LEN;
            unsigned long ulLenUrl = MAX_LEN * 2;
    
            
            fpH = (HTTP_FILTER_PREPROC_HEADERS*)lpNotificationData;
    
	        RtlZeroMemory(szUrlLoc, (MAX_LEN * 2) + 1);
            RtlZeroMemory(szRetHead, MAX_LEN + 1);
   
            //Check for header 
            if (fpH->GetHeader(pfc, (char*)FLTR_AUTH, szRetHead, &ulLenHead) == TRUE && fpH->GetHeader(pfc, (char*)FLTR_PATH, szUrlLoc, &ulLenUrl) == TRUE)
            {
                
                char* szQueryStringLocation = NULL;
                szQueryStringLocation = strstr(szUrlLoc, "?");

                if (szQueryStringLocation != NULL)
                {
                    szQueryStringLocation[0] = NULL;
                }
                
                if(IsValidUser(szRetHead, szUrlLoc))
                {
#ifdef _DEBUG//
                    OutputDebugString("!! passed"); 
#endif//
                }
                else
                {
                    OutputDebugString(stringbuilder("Failed :").concat(szRetHead).c_str());
                        
#ifdef _DEBUG//
                    OutputDebugString(szRetHead);
#endif//
                    goto TellClientNeedAuthorization;
                }       
            }
            else //No header of error so reject them
            {
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                {
#ifdef _DEBUG//
                    OutputDebugString("** buffer too small");
#endif//
                    SetLastError(ERROR_ACCESS_DENIED);
                }
                     
                goto TellClientNeedAuthorization;
                          
            }

            if (false)
            {
                TellClientNeedAuthorization:
                stringbuilder sbX;
                DWORD dwSize = 0;
                char* szHeaders;

                //TODO: does not belong here because 9/10 it will not be used.
                if(!pfc->GetServerVariable(pfc, "ALL_RAW", NULL, &dwSize))
                {
                    dwSize++;
                    szHeaders = (char*)malloc(dwSize);
                    ZeroMemory(szHeaders, dwSize);
                    pfc->GetServerVariable(pfc, "ALL_RAW", szHeaders, &dwSize);                              
                }
                
                //TODO: does not belong here because 9/10 it will not be used.
                sbX.assign("HTTP/1.1 401 OK\r\nServer: Microsoft-IIS/6.0\r\nContent-Type: text/html\r\n");
                sbX.concat(szHeaders);
#ifdef _DEBUG //
                sbX.concat("Instance: ").concat(szInstance);
                sbX.concat("\r\n");
#endif //
                sbX.concat(FLTR_HEAD).concat("\r\nCache-control: private\r\n\r\n");
            
                free(szHeaders);
                dwSize = sbX.length();     
                
                pfc->WriteClient(pfc, (void*)sbX.c_str(), &dwSize, NULL);
                return SF_STATUS_REQ_ERROR;
            }

        }
    }

#ifdef _DEBUG
        ZeroMemory(&szNumBuffer, lstrlen(szNumBuffer));
        itoa((int)(GetTickCount() - start), szNumBuffer, 10);
        OutputDebugString("HTTPFILT");   
        OutputDebugString(szNumBuffer);
#endif

    
    return SF_STATUS_REQ_NEXT_NOTIFICATION;
}

//////////////////////////////////////////////////

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    ((hModule));
    ((ul_reason_for_call));
    ((lpReserved));
    char* szModNameBuffer = NULL;
    szModNameBuffer = (char*)VirtualAlloc(NULL, dwLen, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szModNameBuffer, dwLen);
    //TODO: this is the place where you should have a hard buffer of X array elements and populate with the below call
    GetModuleFileNameA((HINSTANCE__*)hModule, szModNameBuffer, dwLen);    
    szModName = (char*)VirtualAlloc(NULL, lstrlen(szModNameBuffer) + 1, MEM_COMMIT, PAGE_READWRITE);
    RtlZeroMemory(szModName, lstrlen(szModName));
    lstrcpyn(szModName, szModNameBuffer, lstrlen(szModNameBuffer));
    VirtualFree(szModNameBuffer, dwLen, MEM_RELEASE);

    //TODO: here you should malloc szModName with the size of the returned lstrlen of the above return so you don't have
    //      thousands of bytes gone to waste.
#ifdef _DEBUG
    itoa((int)hModule, szInstance, 10);
#endif
    return TRUE;
}

//////////////////////////////////////////////////

BOOL WINAPI __stdcall GetFilterVersion(HTTP_FILTER_VERSION *pVer)
{

    if(PopulateDllData(szModName))
    {
        lstrcpy(pVer->lpszFilterDesc, FILTER_NAME);
        pVer->dwFilterVersion =  HTTP_FILTER_REVISION;
        pVer->dwFlags = SF_NOTIFY_PREPROC_HEADERS;
    
        ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
        
        //initalize the PopulatingUserMap filter
        InitializeCriticalSection(&cs);
        stillProcessing = true;
        PopulateUserMap();
        hThread1 = (HANDLE)_beginthreadex(NULL, NULL, &thread, NULL, NULL, NULL);
    }
    else
    {
#ifdef _DEBUG
        OutputDebugString("Failed");
#endif
    }
    
    
    return TRUE;
}

///////////////////////////////////////////////////////////

BOOL WINAPI __stdcall TerminateFilter(DWORD dwFlags)
{
    int iStrLen = lstrlen(szModName);
    
    OutputDebugString("*********************************************************");
    OutputDebugString("GOODBYE!!!");
    OutputDebugString("*********************************************************");
    
    ((dwFlags));

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 1");
    OutputDebugString("*********************************************************");

    EnterCriticalSection(&cs);
    stillProcessing = false;
    LeaveCriticalSection(&cs);

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 2");
    OutputDebugString("*********************************************************");

    WaitForSingleObject(hThread1, WAIT2CLOSE);
    
    OutputDebugString("*********************************************************");
    OutputDebugString("GB 3");
    OutputDebugString("*********************************************************");
    
    CloseHandle(hThread1);

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 4");
    OutputDebugString("*********************************************************");
    
    VirtualFree(szServer, iStrLen, MEM_RELEASE);

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 5");
    OutputDebugString("*********************************************************");

    VirtualFree(szUid, iStrLen, MEM_RELEASE);

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 6");
    OutputDebugString("*********************************************************");

    VirtualFree(szPwd, iStrLen, MEM_RELEASE);

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 7");
    OutputDebugString("*********************************************************");

    VirtualFree(szSite, iStrLen, MEM_RELEASE);

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 8");
    OutputDebugString("*********************************************************");

    VirtualFree(szModName, iStrLen, MEM_RELEASE);
    
    OutputDebugString("*********************************************************");
    OutputDebugString("GB 9");
    OutputDebugString("*********************************************************");

    smCache0.FreeArray();
    
    OutputDebugString("*********************************************************");
    OutputDebugString("GB 10");
    OutputDebugString("*********************************************************");

    smCache1.FreeArray();

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 11");
    OutputDebugString("*********************************************************");

    smFree1.FreeArray();

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 12");
    OutputDebugString("*********************************************************");

    smFree0.FreeArray();

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 13");
    OutputDebugString("*********************************************************");

    DeleteCriticalSection(&cs);
    
    OutputDebugString("*********************************************************");
    OutputDebugString("GB 14");
    OutputDebugString("*********************************************************");

    ::CoUninitialize();

    OutputDebugString("*********************************************************");
    OutputDebugString("GB 15");
    OutputDebugString("*********************************************************");

    return TRUE;
}
