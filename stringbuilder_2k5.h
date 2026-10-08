#ifndef STRINGBUILDER_HEADER_688576C4_CDA5_4e93_AF67_69176E142156
#define STRINGBUILDER_HEADER_688576C4_CDA5_4e93_AF67_69176E142156

//#include "stdafx.h"
#include "windows.h"

#ifdef _STLSTRING
#include <string>
using std::string;
#endif

#ifdef _GRETA
#include <greta.h>
using namespace regex;
#endif

/*
_MSC_VER
vc5 	= 1100
vc6 	= 1200
vc7.1 	= 1310
vc8 	= 1400
*/

//#define _itoa_s(number, buffer, length, radix) itoa(number,buffer,10)
//#define _gcvt_s(buffer, length, source, safelength) gcvt(source, 5, buffer)
//#define _ultoa_s(source, buffer, length, radix) ultoa(source, buffer, radix)

#define SPC_BASE16_TO_10(x) (((x) >= '0' && (x) <= '9') ? (char)((x) - '0') : (char)(::toupper((x)) - 'A' + 10))

#define STRINGBUILDER_CONSTRUCTOR \
    { \
        m_pszString = NULL; \
        m_pszString1 = NULL; \
        m_pszString2 = NULL; \
    \
        this->assign(source); \
    }

#define STRINGBUILDER_ASSIGN \
    { \
        if (m_lPosition > 0) \
        { \
            clear(); \
        } \
        \
        concat(source); \
        return *this; \
    }

#define STRINGBUILDER_EQUALS_SET \
    { \
        clear(); \
        this->assign(source); \
        \
        return *this; \
    }

#define STRINGBUILDER_EQUALS_CHECK \
    { \
        stringbuilder sb(source); \
        return (this->length() == sb.length() && this->equals(sb.c_str(), true)); \
    }
    
    #define STRINGBUILDER_NOT_EQUALS_CHECK \
        { \
            stringbuilder sb(source); \
            return !(this->length() == sb.length() && this->equals(sb.c_str(), true)); \
        }

    #define STRINGBUILDER_FRIEND_EQUALS_CHECK \
        { \
            stringbuilder sb(leftSource); \
            return (sb.length() == rightSource.length() && sb.equals(rightSource.c_str(), true)); \
        }
    
    #define STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK \
        { \
            stringbuilder sb(leftSource); \
            return !(sb.length() == rightSource.length() && sb.equals(rightSource.c_str(), true)); \
        }

    #define STRINGBUILDER_ADD_CONCAT \
        { \
            this->concat(source); \
            return *this; \
        }
    
    #define STRINGBUILDER_ADD_LEFT \
        { \
            return (stringbuilder(leftSource).concat(rightSource.c_str())); \
        }

    #define STRINGBUILDER_ADD_RIGHT \
        { \
            return leftSource.concat(rightSource); \
        }

    #define STRINGBUILDER_STREAM_IN \
        { \
            destination.concat(source); \
            return destination; \
        }

    class stringbuilder
    {
    // constructors and destructor
    public:
        stringbuilder() : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0), m_WideChar(0)
        {
            m_pszString = NULL;
            m_pszString1 = NULL;
            m_pszString2 = NULL;
        }

        stringbuilder(HINSTANCE hInstance, UINT uID, int nBufferMax) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0), m_WideChar(0)
        {
            m_pszString = NULL;
            m_pszString1 = NULL;
            m_pszString2 = NULL;

            this->LoadString(hInstance, uID, nBufferMax);
        }

        stringbuilder::stringbuilder(stringbuilder& source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0), m_WideChar(0)
        {
            m_pszString = NULL;
            m_pszString1 = NULL;
            m_pszString2 = NULL;

            if (this != &source)
            {
                clear();
                this->assign(source.c_str());
            }
        }

        stringbuilder(const int source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const int* source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const unsigned int source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const unsigned int* source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const long source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const long* source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const unsigned long source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
#pragma warning(disable:4718) // warning about this being a possible recursive call
        stringbuilder(const unsigned long* source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
#pragma warning(default:4718)
        stringbuilder(const bool source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const bool* source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const char source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const char* source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
#ifdef _STLSTRING
        stringbuilder(const string source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const string* source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
#endif
        stringbuilder(const double source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR
        stringbuilder(const double* source) : m_lCapacity(0), m_lCurrStr(1), m_lPosition(0), m_dwSizeTemp(0) STRINGBUILDER_CONSTRUCTOR

        ~stringbuilder()
        {
            this->clear();
        }

    public:
        DWORD   m_dwSizeTemp;
        
    // private member variables
    private:
	    char*   m_pszString;
	    char*   m_pszString1;
	    char*   m_pszString2;
	    long    m_lCurrStr;
	    long    m_lPosition;
	    long    m_lCapacity;
	    char*   m_WideChar;

    // public functions
    public:
        void clear(void)
        {
            if (m_lCurrStr == 1)
                free(m_pszString1);
            else
                free(m_pszString2);

            _clearWideChar();
                
            m_lCapacity  = 0;
            m_lCurrStr   = 1;
            m_lPosition  = 0;
            m_pszString  = NULL;
            m_pszString1 = NULL;
            m_pszString2 = NULL;
        }

        stringbuilder& LoadString(HINSTANCE hInstance, UINT uID, int nBufferMax)
        {
            char* szBuffer = new char[nBufferMax + 1];

            ZeroMemory(szBuffer, nBufferMax);
            ::LoadString(hInstance == NULL ? GetModuleHandle(NULL) : hInstance, uID, szBuffer, nBufferMax);
            concat(szBuffer);
            delete [] szBuffer;

            return *this;
        }
        
        long length(void)
        {
            return m_lPosition;
        }

        long capacity(void)
        {
            return m_lCapacity;
        }

        char* copy(void)
        {
            char* szReturn = NULL;

            if (m_lPosition > 0)
            {
                szReturn = new char[m_lPosition + 1];
                memcpy(szReturn, m_pszString, m_lPosition);
                szReturn[m_lPosition] = '\0';
            }

            return szReturn;
        }

        const char* c_str(void)
        {
            if (m_lPosition > 0)
            {
                m_pszString[m_lPosition] = '\0';
            }
            else
            {
                this->_increaseAllocation(1);
                m_pszString[m_lPosition] = '\0';
            }

            return m_pszString;
        }

        LPCWSTR w_str(void)
        {
            if (m_WideChar == NULL)
            {
                const char* szText = c_str();
                int iCharIndex = 0;
                int iSourceLen = 0;
                int iAllocatedBits = 0;
    
                if (m_WideChar != NULL)
                {
                    free(m_WideChar);
                    m_WideChar = NULL;
                }
                
                iSourceLen = (int)strlen(szText);
                iAllocatedBits = (sizeof(char) * (iSourceLen * 2)) + 2;
    
                m_WideChar = (char*)malloc(iAllocatedBits);
                ZeroMemory(m_WideChar, iAllocatedBits);
    
                iCharIndex = (iSourceLen * 2);
        
                for (; iCharIndex > -1; iCharIndex--)
                {
                    m_WideChar[iCharIndex--] = szText[iCharIndex / 2];
                }
            }
                
            return (LPCWSTR)m_WideChar;
        }

        stringbuilder& tolower(void)
        {
            if (m_lPosition > 0)
            {
                _clearWideChar();

                #pragma warning(disable: 4996)
                _strlwr((char*)this->c_str());
                #pragma warning(default: 4996)
            }

            return *this;
        }

        stringbuilder& toupper(void)
        {
            if (m_lPosition > 0)
            {
                _clearWideChar();

                #pragma warning(disable: 4996)
                _strupr((char*)this->c_str());
                #pragma warning(default: 4996)
            }

            return *this;
        }

        static bool isNumeric(const char* szString)
        {
            size_t ui64Length = strlen(szString);
            bool bAlreadyDecimal = false;

            if (ui64Length == 0)
                return false;

            for (size_t ui64Loop = 0; ui64Loop < ui64Length; ++ui64Loop)
            {
                if (szString[ui64Loop] > 44 && szString[ui64Loop] < 58)
                {
                    if (ui64Loop == 0 && szString[ui64Loop] != 45)
                        return false;

                    if (szString[ui64Loop] == 47)
                        return false;

                    if (szString[ui64Loop] == 46 && bAlreadyDecimal == false)
                        bAlreadyDecimal = true;
                    else
                        return false;
                }
                else
                {
                    return false;
                }
            }

            return true;
        }

#ifdef _STLSTRING
#ifdef _GRETA
        stringbuilder& replaceText(const char* szFindPattern, const char* szReplaceExpression)
        {
            subst_results results;
	        string str = this->c_str();
            rpattern pat(szFindPattern, szReplaceExpression, GLOBAL | NOCASE);
	        pat.substitute(str, results);

            this->assign(str.c_str());

            return *this;
        }

        stringbuilder& cropText(const char* szFindPattern, const char* szDefaultValue = "")
        {
            match_results results;
            
            string str(this->c_str());
            rpattern pat(szFindPattern);  

	        match_results::backref_type br = pat.match(str, results, NOCASE);

            if (results.cbackrefs() > 0)
                this->assign(results.backref(1).str().c_str());
            else
                this->assign("");

            if (this->length() == 0)
                this->assign(szDefaultValue);

            return *this;
        }

        stringbuilder findText(const char* szFindPattern, const char* szDefaultValue = "")
        {
            stringbuilder sbReturn;
            match_results results;

            string str(this->c_str());
            rpattern pat(szFindPattern, NOCASE);  

	        match_results::backref_type br = pat.match(str, results);

            if (results.cbackrefs() > 0)
            {
                sbReturn.assign(results.backref(1).str().c_str());
            }
            else
            {
                sbReturn.assign("");
            }

            if (sbReturn.length() == 0)
                sbReturn.assign(szDefaultValue);

            return sbReturn;
        }
#endif
#endif

        void calculateLength(void)
        {
            this->m_lPosition = (long)strlen(this->m_pszString);
        }

        void calculateLengthAs(long lLength)
        {
            if (lLength < this->m_lCapacity)
                this->m_lPosition = (long)lLength;

            if (lLength < 0)
                this->m_lPosition = 0;
        }

        stringbuilder& assignFill(const char* szFill, const int iPatternReplicationCount)
        {
            int iFillLength = (int)strlen(szFill);
            this->m_dwSizeTemp = iPatternReplicationCount * iFillLength;
            this->assign("");

            for (int i = 0; i < iPatternReplicationCount; ++i)
            {
                this->concat(szFill, iFillLength);
            }
            
            return *this;
        }
        
        stringbuilder& assignFill(const char szFill, const int iPatternReplicationCount)
        {
            this->m_dwSizeTemp = iPatternReplicationCount;
            this->assign("");

            for (int i = 0; i < iPatternReplicationCount; ++i)
            {
                this->concat(szFill);
            }
            
            return *this;
        }

        stringbuilder& urlDecode(void)
        {
		    size_t stSize = (size_t)this->length();
            char* cDecode = _urlDecode(this->c_str(), &stSize);
            this->assign(cDecode, (long)stSize);
		    delete [] cDecode;

            return *this;
        }

        stringbuilder& urlEncode(void)
        {
            stringbuilder sbOutput;
            char szChar[4] = {'\0'};

            if (this->length() > 0)
            {
                char* szCharacter = (char*)this->c_str();

                while (*szCharacter)
                {
                    if (isalnum(*szCharacter))
                        sbOutput.concat(szCharacter, 1);
                    else
                    {
//                        _itoa_s((int)*szCharacter, szChar, 4, 16);
                        sbOutput.concat('%').concat(szChar);
                    }

                    szCharacter++;
                }
            }

            this->assign(sbOutput.c_str());

            return *this;
        }

        char* endChar(int iRequiredReserveSpace)
        {
            this->_increaseAllocation(iRequiredReserveSpace);
            this->m_dwSizeTemp = iRequiredReserveSpace;

	        return (char*)m_pszString[m_lPosition];
        }

        bool equals(stringbuilder* sbSource, bool bCaseSensitive = false)
        {
            if (sbSource->length() != this->length())
            {
                return false;
            }
            else
            {
                if (!bCaseSensitive)
                    return (_strnicmp(this->c_str(), sbSource->c_str(), this->length()) == 0);
                else
                    return (strncmp(this->c_str(), sbSource->c_str(), this->length()) == 0);
            }
        }

        bool equals(const char* source, bool bCaseSensitive = false)
        {
            if (strlen(source) != (size_t)this->length())
            {
                return false;
            }
            else
            {
                if (!bCaseSensitive)
                {
                    return (_strnicmp(this->c_str(), source, this->length()) == 0);
                }
                else
                {
                    return (strncmp(this->c_str(), source, this->length()) == 0);
                }
            }
        }

        // operator overloads
    public: 
        const bool operator!= (stringbuilder& source)
        {
            return !(this->length() == source.length() && this->equals(source.c_str(), true));
        }

        const bool operator!= (const int source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const int* source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const unsigned int source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const unsigned int* source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const long source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const long* source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const unsigned long source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const unsigned long* source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const bool source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const bool* source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const char source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const char* source) STRINGBUILDER_NOT_EQUALS_CHECK
#ifdef _STLSTRING
        const bool operator!= (const string source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const string* source) STRINGBUILDER_NOT_EQUALS_CHECK
#endif
        const bool operator!= (const double source) STRINGBUILDER_NOT_EQUALS_CHECK
        const bool operator!= (const double* source) STRINGBUILDER_NOT_EQUALS_CHECK

        const bool operator== (stringbuilder& source)
        {
            return (this->length() == source.length() && this->equals(source.c_str(), true));
        }

        const bool operator== (const int source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const int* source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const unsigned int source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const unsigned int* source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const long source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const long* source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const unsigned long source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const unsigned long* source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const bool source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const bool* source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const char source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const char* source) STRINGBUILDER_EQUALS_CHECK
#ifdef _STLSTRING
        const bool operator== (const string source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const string* source) STRINGBUILDER_EQUALS_CHECK
#endif
        const bool operator== (const double source) STRINGBUILDER_EQUALS_CHECK
        const bool operator== (const double* source) STRINGBUILDER_EQUALS_CHECK

        const stringbuilder& operator= (stringbuilder& source)
        {
            if (this != &source)
            {
                clear();
                this->assign(source.c_str());
            }

            return *this;
        }

        const stringbuilder& operator= (const int source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const int* source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const unsigned int source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const unsigned int* source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const long source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const long* source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const unsigned long source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const unsigned long* source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const bool source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const bool* source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const char source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const char* source) STRINGBUILDER_EQUALS_SET
#ifdef _STLSTRING
        const stringbuilder& operator= (const string source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const string* source) STRINGBUILDER_EQUALS_SET
#endif
        const stringbuilder& operator= (const double source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator= (const double* source) STRINGBUILDER_EQUALS_SET
        const stringbuilder& operator+= (stringbuilder& source)
        {
            this->concat(source.c_str());
            return *this;
        }

        const stringbuilder& operator+= (const int source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const int* source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const unsigned int source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const unsigned int* source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const long source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const long* source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const unsigned long source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const unsigned long* source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const bool source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const bool* source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const char source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const char* source) STRINGBUILDER_ADD_CONCAT
#ifdef _STLSTRING
        const stringbuilder& operator+= (const string source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const string* source) STRINGBUILDER_ADD_CONCAT
#endif
        const stringbuilder& operator+= (const double source) STRINGBUILDER_ADD_CONCAT
        const stringbuilder& operator+= (const double* source) STRINGBUILDER_ADD_CONCAT

    // concat functions
    public:
        stringbuilder& assign(stringbuilder& source)
        {
            if (&source != this)
            {
                this->assign(source.c_str(), source.length());
            }

            return *this;
        }

        stringbuilder& assign(const int source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const int* source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const unsigned int source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const unsigned int* source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const long source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const long* source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const unsigned long source) STRINGBUILDER_ASSIGN
        #pragma warning(disable:4718) // warning about this being a possible recursive call
        stringbuilder& assign(const unsigned long* source) STRINGBUILDER_ASSIGN
        #pragma warning(default:4718)
        stringbuilder& assign(const bool source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const bool* source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const char source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const char* source) STRINGBUILDER_ASSIGN
#ifdef _STLSTRING
        stringbuilder& assign(const string source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const string* source) STRINGBUILDER_ASSIGN
#endif
        stringbuilder& assign(const double source) STRINGBUILDER_ASSIGN
        stringbuilder& assign(const double* source) STRINGBUILDER_ASSIGN
        
        stringbuilder& assign(const char* source, const long length)
        {
            clear();
            concat(source, length);
            return *this;
        }
        
        stringbuilder& concat(const double source)
        {
            char c[128] = {'\0'};
            _gcvt_s(c, 128, source, 120);
            char* szE = strstr(c, "e");

            if (szE > NULL)
            {
                szE[0] = 0;
            }

            concat(c);
            return *this;
        }
        
        stringbuilder& concat(const double* source)
        {
            char c[128] = {'\0'};
            _gcvt_s(c, 128, *source, 120);

            char* szE = strstr(c, "e");

            if (szE > NULL)
            {
                szE[0] = 0;
            }

            concat(c);
            return *this;
        }

#ifdef _STLSTRING
        stringbuilder& concat(const string source)
        {
            concat(source.c_str());
            return *this;
        }

        stringbuilder& concat(const string* source)
        {
            concat(source->c_str());
            return *this;
        }
#endif

        stringbuilder& concat(const int source)
        {
            char c[32];
            _itoa_s(source, c, 32, 10);
            concat(c);
            return *this;
        }

        stringbuilder& concat(const int* source)
        {
            if (source != NULL)
            {
                concat(*source);
            }

            return *this;
        }

        stringbuilder& concat(const unsigned int source)
        {
            char c[32] = {'\0'};
            _itoa_s(source, c, 32, 10);
            concat(c);
            return *this;
        }

        stringbuilder& concat(const unsigned int* source)
        {
            if (source != NULL)
            {
                concat(*source);
            }

            return *this;
        }

        stringbuilder& concat(const long source)
        {
            char c[32] = {""};
            _itoa_s(source, c, 32, 10);
            concat(c);
            return *this;
        }

        stringbuilder& concat(const long* source)
        {
            if (source != NULL)
            {
                concat(*source);
            }

            return *this;
        }

        stringbuilder& concat(const unsigned long source)
        {
            char c[32] = {""};
            _ultoa_s(source, c, 32, 10);
            concat(c);
            return *this;
        }

        stringbuilder& concat(const unsigned long* source)
        {
            if (source != NULL)
            {
                concat(source);
            }

            return *this;
        }

        stringbuilder& concat(const bool source)
        {
            concat(int(source));
            return *this;
        }

        stringbuilder& concat(const bool* source)
        {
            if (source != NULL)
            {
                concat(*source);
            }

            return *this;
        }

        stringbuilder& concat(const char source) 
        {
            concat(&source, 1);
            return *this;
        }

        stringbuilder& concat(const char* source, const long length)
        {
            this->_increaseAllocation(length);

            if (length > 0)
            {
                for(int y = 0; y < length; y++)
	            {
		            m_pszString[m_lPosition++] = source[y];
	            }
            }
            else
            {
                if (this->length() == 0)
                {
                    this->_increaseAllocation(1);
                    m_pszString[m_lPosition] = '\0';
                }
            }

            return *this;
        }

        stringbuilder& concat(const char* source)
        {
            concat(source, (long)strlen(source));
            return *this;
        }

        stringbuilder& concat(stringbuilder& source)
        {
            if (&source != this)
            {
                this->concat(source.c_str(), source.length());
            }

            return *this;
        }

        // private functions
    private:
        void _clearWideChar(void)
        {
            if (m_WideChar != NULL)
            {
                free(m_WideChar);
                m_WideChar = NULL;
            }
        }
        
        void _increaseAllocation(const long length)
        {
            _clearWideChar();
            
            if ((length + m_lPosition + 1) > m_lCapacity)
	        {
                m_lCapacity = (((m_lPosition + length) * 2));
                
                if (m_lCurrStr == 1)
		        {
                    m_pszString2 = (char*)malloc(m_lCapacity * sizeof(char));
                    memcpy(m_pszString2, m_pszString1, m_lPosition);
                    m_pszString = m_pszString2;
                    free(m_pszString1);
                    m_lCurrStr = 2;
		        }
		        else
		        {
                    m_pszString1 = (char*)malloc(m_lCapacity * sizeof(char));
                    memcpy(m_pszString1, m_pszString2, m_lPosition);
                    m_pszString = m_pszString1;
                    free(m_pszString2);
                    m_lCurrStr = 1;
		        }
	        }
        }

        char* _urlDecode(const char *url, size_t *nbytes) 
        {
	        char  *out, *ptr;
	        const char *c;

	        out = ptr = _strdup(url);

	        if (!out) 
	        {
		        return 0;
	        }

	        for (c = url;  *c;  c++) 
	        {
		        if (*c != '%' || !isxdigit(c[1]) || !isxdigit(c[2])) 
		        {
			        *ptr++ = *c;
		        }
		        else 
		        {
			        *ptr++ = (SPC_BASE16_TO_10(c[1]) * 16) + (SPC_BASE16_TO_10(c[2]));
			        c += 2;
		        }
	        }

	        *ptr = 0;
          
	        if (nbytes) 
	        {
		        *nbytes = (ptr - out); /* does not include null byte */
	        }

	        return out;
        }
    };

    const bool operator!= (stringbuilder& leftSource, stringbuilder& rightSource)
    {
        return !(leftSource.length() == rightSource.length() && leftSource.equals(rightSource.c_str(), true));
    }

    const bool operator!= (const int leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const int* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const unsigned int leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const unsigned int* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const long leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const long* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const unsigned long leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const unsigned long* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const bool leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const bool* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const char leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const char* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
#ifdef _STLSTRING
    const bool operator!= (const string leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const string* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
#endif
    const bool operator!= (const double leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK
    const bool operator!= (const double* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_NOT_EQUALS_CHECK

    const bool operator== (stringbuilder& leftSource, stringbuilder& rightSource)
    {
        return (leftSource.length() == rightSource.length() && leftSource.equals(rightSource.c_str(), true));
    }

    const bool operator== (const int leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const int* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const unsigned int leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const unsigned int* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const long leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const long* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const unsigned long leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
#pragma warning(disable:4718) // warning about this being a possible recursive call
    const bool operator== (const unsigned long* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
#pragma warning(default:4718)
    const bool operator== (const bool leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const bool* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const char leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const char* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
#ifdef _STLSTRING
    const bool operator== (const string leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const string* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
#endif
    const bool operator== (const double leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK
    const bool operator== (const double* leftSource, stringbuilder& rightSource) STRINGBUILDER_FRIEND_EQUALS_CHECK

    stringbuilder operator+ (stringbuilder& leftSource, stringbuilder& rightSource)
    {
        return (leftSource.concat(rightSource.c_str()));
    }

    stringbuilder operator+ (const int leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const int* leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const unsigned int leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const unsigned int* leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const long leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const long* leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const unsigned long leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
#pragma warning(disable:4718) // warning about this being a possible recursive call
    stringbuilder operator+ (const unsigned long* leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
#pragma warning(default:4718)
    stringbuilder operator+ (const bool leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const bool* leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const char leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const char* leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
#ifdef _STLSTRING
    stringbuilder operator+ (const string leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const string* leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
#endif
    stringbuilder operator+ (const double leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT
    stringbuilder operator+ (const double* leftSource, stringbuilder& rightSource) STRINGBUILDER_ADD_LEFT

    stringbuilder operator+ (stringbuilder& leftSource, const int rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const int* rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const unsigned int rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const unsigned int* rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const long rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const long* rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const unsigned long rightSource) STRINGBUILDER_ADD_RIGHT
#pragma warning(disable:4718) // warning about this being a possible recursive call
    stringbuilder operator+ (stringbuilder& leftSource, const unsigned long* rightSource) STRINGBUILDER_ADD_RIGHT
#pragma warning(default:4718)
    stringbuilder operator+ (stringbuilder& leftSource, const bool rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const bool* rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const char rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const char* rightSource) STRINGBUILDER_ADD_RIGHT
#ifdef _STLSTRING
    stringbuilder operator+ (stringbuilder& leftSource, const string rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const string* rightSource) STRINGBUILDER_ADD_RIGHT
#endif
    stringbuilder operator+ (stringbuilder& leftSource, const double rightSource) STRINGBUILDER_ADD_RIGHT
    stringbuilder operator+ (stringbuilder& leftSource, const double* rightSource) STRINGBUILDER_ADD_RIGHT

    stringbuilder& operator<< (stringbuilder& destination, stringbuilder& source)
    {
        destination.concat(source.c_str(), source.length());
        return destination;
    }

    stringbuilder& operator<< (stringbuilder& destination, const int source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const int* source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const unsigned int source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const unsigned int* source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const long source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const long* source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const unsigned long source) STRINGBUILDER_STREAM_IN
#pragma warning(disable:4718) // warning about this being a possible recursive call
    stringbuilder& operator<< (stringbuilder& destination, const unsigned long* source) STRINGBUILDER_STREAM_IN
#pragma warning(default:4718)
    stringbuilder& operator<< (stringbuilder& destination, const bool source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const bool* source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const char source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const char* source) STRINGBUILDER_STREAM_IN
#ifdef _STLSTRING
    stringbuilder& operator<< (stringbuilder& destination, const string source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const string* source) STRINGBUILDER_STREAM_IN
#endif
    stringbuilder& operator<< (stringbuilder& destination, const double source) STRINGBUILDER_STREAM_IN
    stringbuilder& operator<< (stringbuilder& destination, const double* source) STRINGBUILDER_STREAM_IN

#endif