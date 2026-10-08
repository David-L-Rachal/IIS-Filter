#if !defined(F440AC82_A939_4F1E_8BFF_187268D26B73)
#define F440AC82_A939_4F1E_8BFF_187268D26B73



#include "stdafx.h"


class CDataReturn: public CADORecordBinding, public CADORecordBindingEx
{
BEGIN_ADO_BINDING(CDataReturn)
    
    ADO_VARBL_LENGTH_ENTRY_EX(1, UserAndPath)
    ADO_FIXED_LENGTH_ENTRY_EX(2, Number)
   // ADO_VARBL_LENGTH_ENTRY_EX(2, TblDt)
    
END_ADO_BINDING()

public:
    CDataReturn(void)
    {
        this->_ClearValues();
    }

    virtual void _ClearValues(void)
    {        
        UserAndPath._ClearValues();
        Number._ClearValues();
        // TblDt._ClearValues();
    }

    virtual void _Fixup(void)
    {
        UserAndPath._Fixup();
        Number._Fixup();
        //TblDt._Fixup();
    }

public:
    
    CADODataTypeChars<1000> UserAndPath;
    CADODataTypeInt Number;
    //CADODataTypeChars<100> TblDt;

public:
    stringbuilder SQL(char* szSite, char* szUpdated)
    {
        stringbuilder sbSQL;
        //char* szSiteId = "2 \r\n";
        
        sbSQL.assign("exec sp_UpdateUserMap '");
        sbSQL.concat(szSite);
        sbSQL.concat("', '");
        sbSQL.concat(szUpdated);
        sbSQL.concat("'");
#ifdef _DEBUG        
        OutputDebugString(sbSQL.c_str());
#endif

        //sbSQL.assign("set nocount on \r\n");
        //sbSQL.concat("select  userandpath, updatedt from \r\n");
        //sbSQL.concat("( \r\n");
        //sbSQL.concat("    select \r\n");
        //sbSQL.concat("    ('Basic ' + Encrypted + URLFUllPath) as UserAndPath , ('placeholder') as UpdateDt\r\n");
        //sbSQL.concat("from \r\n");
        //sbSQL.concat("    <DB_NAME>.dbo.users u \r\n");
        //sbSQL.concat("inner join \r\n");
        //sbSQL.concat("    <DB_NAME>.dbo.Resources r \r\n");
        //sbSQL.concat("on u.Siteid = r.SiteId \r\n");
        //sbSQL.concat("    where Encrypted <> '' \r\n");
        //sbSQL.concat(") as x \r\n");
        //sbSQL.concat("order by convert(binary(1100), userandpath) \r\n");
        //sbSQL.concat("set nocount off \r\n");
        //sbSQL.concat("select UserAndPath, UpdateDt from #t order by convert(binary (100), UserAndPath) \r\n");
        //sbSQL.concat("set nocount on \r\n");
        //sbSQL.concat("drop table #t \r\n");
        //sbSQL.concat("set nocount off \r\n");
        return sbSQL;
    }
/*
public:
    stringbuilder SqlGet(void)
    {
        stringbuilder sbGet;
        sbGet.assign("select UserAndPath, UpdateDt from #t order by convert(binary (100), UserAndPath)");
        return sbGet;    
    }
    stringbuilder SqlKill(void)
    {
        stringbuilder sbKill;
        sbKill.assign("drop table #t");
        return sbKill;
    }
*/
};


#endif
