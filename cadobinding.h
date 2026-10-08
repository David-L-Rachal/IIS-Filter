#include "stdafx.h"
#include "icrsint.h"

#ifndef CADOBinding_52691378_4C67_4261_8E3E_ED7AAE1BD7D6
#define CADOBinding_52691378_4C67_4261_8E3E_ED7AAE1BD7D6

#include <windows.h>
#include <time.h>               // time functions and structures

// this is the ado reference that I am using.
// we are using only raw_interfaces because it does not hide any implimentation 
// the warning disable is for a known ado issue
#pragma warning(push)
#pragma warning(disable: 4146)
#import "c:\program files\common files\system\ado\msado15.dll" rename_namespace("MSADO") rename("EOF", "adoEOF") raw_interfaces_only
#pragma warning(pop)

// start using the MSADO namespace right away. this must be before the include of the "icrsint.h" file
using namespace MSADO;

#include "icrsint.h"
#include "initguid.h"
#include <unknwn.h>

#define INITGUID 

//#include "iadmw.h"
#include "iiscnfg.h"
#include "atlBase.h"

#include "stringbuilder.h"      // stringbuilder class

#define ADO_FIXED_LENGTH_ENTRY_EX(index, intData) ADO_FIXED_LENGTH_ENTRY(index, MSADO::adInteger, intData.Value, intData.Status, FALSE)
#define ADO_DT_TM_LENGTH_ENTRY_EX(index, dblData) ADO_NUMERIC_ENTRY(index, MSADO::adDate, dblData.DoubleValue, 10, 10, dblData.Status, FALSE)
#define ADO_VARBL_LENGTH_ENTRY_EX(index, charData) ADO_VARIABLE_LENGTH_ENTRY(index, MSADO::adVarChar, charData.Value, sizeof(charData.Value), charData.Status, charData.Length, FALSE)
#define ADO_DOUBL_LENGTH_ENTRY_EX(index, charData, precision, scale) ADO_NUMERIC_ENTRY(index, MSADO::adDouble, charData.Value, precision, scale, charData.Status, FALSE)

//namespace OCTANE
//{
    class CADORecordBindingEx
    {
    public:
        CADORecordBindingEx(void)
        {
            this->_ClearValues();    
        }

        virtual void _ClearValues(void) {};
        virtual void _Fixup(void) {};
    };

    class CADODataTypeDbl: public CADORecordBindingEx
    {
    public:
        CADODataTypeDbl(void)
        {
            this->_ClearValues();
        }

        virtual void _ClearValues(void)
        {
            Value = 0.0;
            Status = 0;
        }

        virtual void _Fixup(void)
        {
            if (Status != adFldOK)
            {
                Value = 0.0;
            }
        }

        bool isNull(void)
        {
            return (Status == adFldNull);
        }
        
        double Value;
        unsigned long Status;
    };

    class CADODataTypeDtTm: public CADORecordBindingEx
    {
    // to get milliseconds we need to create a custom conversion: http://en.wikipedia.org/wiki/Decimal_time
    public:
        CADODataTypeDtTm(void)
        {
            this->_ClearValues();
        }

        virtual void _ClearValues(void)
        {
            Value.wDay = 0;
            Value.wDayOfWeek = 0;
            Value.wHour = 0;
            Value.wMilliseconds = 0;
            Value.wMinute = 0;
            Value.wMonth = 0;
            Value.wSecond = 0;
            Value.wYear = 0;
            DoubleValue = 0;
            Status = 0;
        }

        virtual void _Fixup(void)
        {
            if (Status != adFldOK)
            {
                Value.wDay = 0;
                Value.wDayOfWeek = 0;
                Value.wHour = 0;
                Value.wMilliseconds = 0;
                Value.wMinute = 0;
                Value.wMonth = 0;
                Value.wSecond = 0;
                Value.wYear = 0;
                DoubleValue = 0;
            }
            else
            {
                VariantTimeToSystemTime(DoubleValue, &Value);
            }
        }

        stringbuilder Format(stringbuilder sbDayFormat, stringbuilder sbTimeFormat)
        {
            stringbuilder sbReturn;
            char* szDayTime = NULL;
            int iCharCount = 0;

            if (sbDayFormat.length() > 0) 
            {
                iCharCount = GetDateFormat(LOCALE_SYSTEM_DEFAULT, NULL, &Value, sbDayFormat.c_str(), NULL, iCharCount);
                szDayTime = new char[iCharCount + 1];
                ZeroMemory(szDayTime, sizeof(char) * (iCharCount + 1));
                GetDateFormat(LOCALE_SYSTEM_DEFAULT, NULL, &Value, sbDayFormat.c_str(), szDayTime, iCharCount);
                sbReturn.concat(szDayTime);
                iCharCount = 0;
                delete [] szDayTime;
            }
            
            if (sbTimeFormat.length() > 0) 
            {
                iCharCount = GetTimeFormat(LOCALE_SYSTEM_DEFAULT, NULL, &Value, sbTimeFormat.c_str(), NULL, iCharCount);
                szDayTime = new char[iCharCount + 1];
                ZeroMemory(szDayTime, sizeof(char) * (iCharCount + 1));
                GetTimeFormat(LOCALE_SYSTEM_DEFAULT, NULL, &Value, sbTimeFormat.c_str(), szDayTime, iCharCount);
                sbReturn.concat(szDayTime);
                iCharCount = 0;
                delete [] szDayTime;
            }
            
            return sbReturn;
        }

        bool isNull(void)
        {
            return (Status == adFldNull);
        }
        
        SYSTEMTIME Value;
        double DoubleValue;
        unsigned long Status;
    };

    class CADODataTypeInt: public CADORecordBindingEx
    {
    public:
        CADODataTypeInt(void)
        {
            this->_ClearValues();
        }

        virtual void _ClearValues(void)
        {
            Value = 0;
            Status = 0;
        }

        virtual void _Fixup(void)
        {
            if (Status != adFldOK)
            {
                Value = 0;
            }
        }

        bool isNull(void)
        {
            return (Status == adFldNull);
        }
        
        long Value;
        unsigned long Status;
    };
    
    template <int iLength> class CADODataTypeChars: public CADORecordBindingEx
    {
    public:
        CADODataTypeChars(void)
        {
            this->_ClearValues();
        }

        virtual void _ClearValues(void)
        {
            ZeroMemory(Value, sizeof(char) * (iLength + 1));
            Status = 0;
            Length = 0;
        }

        virtual void _Fixup(void)
        {
            if (Status != adFldOK || Length == 0)
            {
                Length = 0;
                ZeroMemory(Value, sizeof(char) * (iLength + 1));
            }
        }

        bool isNull(void)
        {
            return (Status == adFldNull);
        }

        stringbuilder isNull(stringbuilder sbOptionalValue)
        {
            stringbuilder sbReturn;
            
            if (Status == adFldNull || Length == 0)
            {
                sbReturn = sbOptionalValue.c_str();
            }
            else
            {
                sbReturn = Value;
            }

            return sbReturn;
        }

        char Value[iLength + 1];
        unsigned long Status;
        unsigned long Length;
    };

    class CADOBinding
    {
    public:
        CADOBinding() : m_pConn(NULL), m_pRec(NULL), m_lngRecordCount(0), m_lngCurrRecord(0), m_bIsOpen(false), m_bIsBound(false), cadoRecordBinding(NULL), blnAdvancedBinding(false)
        {
            ::VariantInit(&m_vRecordCount);    
        }

        ~CADOBinding()
        {
            Close();
            ::VariantClear(&m_vRecordCount);
        }

    private:
        MSADO::_ConnectionPtr   m_pConn;
        MSADO::_RecordsetPtr    m_pRec;
        VARIANT                 m_vRecordCount;
        CADORecordBinding*      cadoRecordBinding;
        CADORecordBindingEx*    cadoRecordBindingEx;
        long                    m_lngRecordCount;
        long                    m_lngCurrRecord;
        bool                    m_bIsOpen;
        bool                    m_bIsBound;
        IADORecordBinding*      piAdoRecordBinding;
        bool                    blnAdvancedBinding;

    public:
        stringbuilder           LastError;

    public:
        bool Open(const char* szProvider, const char* szDataSource, const char* szInitialCatalog = "", const char* szUsername = "", const char* szPassword = "", long lConnectTimeout = 120, long lCommandTimeout = 120, char* szExtraProviderStringData = NULL)
        {
            stringbuilder sbConnectionString;

            if (m_bIsOpen)
                this->Close();

            m_pConn.CreateInstance(__uuidof(MSADO::Connection));
            m_pRec.CreateInstance(__uuidof(MSADO::Recordset));
            
            sbConnectionString.assign("Provider=").concat(szProvider);
            sbConnectionString.concat(";Data Source=").concat(szDataSource);
            
            if (strlen(szInitialCatalog) != 0)            
                sbConnectionString.concat(";Initial Catalog=").concat(szInitialCatalog);

            sbConnectionString.concat(";User Id=").concat(szUsername);
            sbConnectionString.concat(";Password=").concat(szPassword);
            
            if (szExtraProviderStringData != NULL)
                sbConnectionString.concat(szExtraProviderStringData);

            m_pConn->put_CursorLocation(MSADO::adUseClient);
            m_pConn->put_ConnectionTimeout(lConnectTimeout);
            m_pConn->put_CommandTimeout(lCommandTimeout);
            
            if (SUCCEEDED(m_pConn->Open(_bstr_t(sbConnectionString.c_str()).copy(), _bstr_t("").copy(), _bstr_t("").copy(), MSADO::adModeUnknown)))
            {
                m_bIsOpen = true;
            }
            else
            {
                m_bIsOpen = false;
                RecordLastError("Open");
            }

            return m_bIsOpen;
        }

        bool Execute(const char* szCommand)
        {
            if (m_bIsOpen)
            {
                if(SUCCEEDED(m_pConn->Execute(_bstr_t(szCommand), &m_vRecordCount, 0, &m_pRec)))
                {
                    return true;
                }
                else
                {
                    RecordLastError("Execute");
                    return false;
                }
            }
            else
            {
                LastError.assign("\"CADOBinding::Execute\" method failed: Connection not Open.");
                return false;
            }
        }

        bool Execute(const char* szCommand, CADORecordBinding* cadoRecordBinding, CADORecordBindingEx* cadoRecordBindingEx = NULL)
        {
            VARIANT_BOOL vEOF;

            this->cadoRecordBinding = cadoRecordBinding;

            if (cadoRecordBindingEx != NULL)
            {
                this->cadoRecordBindingEx = cadoRecordBindingEx;
                cadoRecordBindingEx->_ClearValues();
                blnAdvancedBinding = true;
            }

            m_bIsBound = false;
            stringbuilder sbMsg;

            if (m_bIsOpen)
            {
                if (SUCCEEDED(m_pConn->Execute(_bstr_t(szCommand), &m_vRecordCount, 0, &m_pRec)))
                {
                    m_bIsBound = true;
                    // each recordset has the record binding interface. we just need to extract it here.
                    m_pRec->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&piAdoRecordBinding);
                    
                    // use the record binding interface from the recordset to bind to the class that has all the C datatypes for each record
                    piAdoRecordBinding->BindToRecordset(cadoRecordBinding);
                    m_pRec->get_adoEOF(&vEOF);

                    // check the EOF so we can more to the last record and get the totol count of records from the return.
                    // this is not required as you can check the value of EOF each time, but I wanted to get the total count
                    // of records just for clerity of this exercise. This will allow you to know how many items are returned 
                    // so you can create an array of items with this count.
                    if (vEOF == 0)
                    {
                        m_pRec->MoveLast();
                        m_pRec->get_RecordCount(&m_lngRecordCount);
                        m_pRec->MoveFirst();
                    }

                    // if we are using the additional interface to fixup new values then do so now.
                    if (blnAdvancedBinding) cadoRecordBindingEx->_Fixup();

                    return true;
                }
                else
                {
                    RecordLastError("Execute");
                    return false;
                }
            }
            else
            {
                LastError.assign("\"CADOBinding::Execute\" method failed: Connection not Open.");
                return false;
            }
        }

        bool EndOfRecord(void)
        {
            return m_lngCurrRecord >= m_lngRecordCount;
        }

        void Close(void)
        {
            if (m_bIsBound)
            {
                piAdoRecordBinding->Release();
                m_pRec->Close();
                m_pRec = NULL; 
            }

            if (m_bIsOpen)
            {
                m_pConn->Close();
                m_pConn = NULL;
            }
            else
            {
                LastError.assign("\"CADOBinding::Close\" method warning: Already Closed.");
            }

            m_bIsOpen = false;
            m_bIsBound = false;
            m_lngCurrRecord = 0;
            m_lngRecordCount = 0;
        }

        bool MoveNext(void)
        {
            if (!EndOfRecord())
            {
                m_pRec->MoveNext();
                if (blnAdvancedBinding) cadoRecordBindingEx->_Fixup();
                ++m_lngCurrRecord;
            }
            else
            {
                LastError.assign("\"CADOBinding::MoveNext\" method warning: Already at EndOfRecord.");
            }

            return !EndOfRecord();
        }

        int RecordCount(void)
        {
            return m_lngRecordCount;
        }

        int CurrRecord(void)
        {
            return m_lngCurrRecord;
        }

    private:
        void RecordLastError(stringbuilder strMethod)
        {
            m_bIsOpen = false;

            MSADO::ErrorPtr pErr;
            MSADO::ErrorsPtr pErrs;
            long lErrorCount = 0;
            long lCurrErrNumber = NULL;
            BSTR lCurrErrSource = NULL;
            BSTR lCurrErrDescription = NULL;
            
            m_pConn->get_Errors(&pErrs);
            pErrs->get_Count(&lErrorCount);

            LastError.assign("\"CADOBinding::").concat(strMethod.c_str()).concat("\" method failed with ").concat(lErrorCount).concat(" error(s). \r\n\r\n");
            
            for (long lErrIndex = 0; lErrIndex < lErrorCount; ++lErrIndex)
            {
                pErrs->get_Item(variant_t(lErrIndex), &pErr);

                pErr->get_Number(&lCurrErrNumber);
                pErr->get_Source(&lCurrErrSource);
                pErr->get_Description(&lCurrErrDescription);

                LastError.concat(lCurrErrNumber).concat(" (").concat((char*)bstr_t(lCurrErrSource)).concat("): ").concat((char*)bstr_t(lCurrErrDescription)).concat("\r\n");
            }

            pErrs->Clear();
        }
    };
//}

#endif