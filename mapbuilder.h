#include "stdafx.h"

#define valloc(size) VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#define vfree(address) VirtualFree(address, NULL, MEM_RELEASE);

// return values
#define SEARCH_VALUE_NOT_FOUND  -1
#define SEARCH_VALUE_TOO_LOW    -2
#define SEARCH_VALUE_TOO_HIGH   -3

// return for the comparison function
#define COMPARE_VALUE_IS_GREATER 1
#define COMPARE_VALUE_IS_LESSER -1
#define COMPARE_VALUE_EQUALS     0

// algor hard upper limit
#define SEARCH_LIMIT 1000

struct FOUNDRANGE
{
    long start;
    long finis;
};

template <typename TPattern> class mapbuilder
{
public:
    mapbuilder(void)
    {
        tArray = NULL;
        ulArraySize = 0;
    }

public:
    TPattern* tArray;
    long ulArraySize;

public:
    virtual int _compareV1V2(TPattern v1, TPattern v2)
    {
        if (v1 == v2)
            return COMPARE_VALUE_EQUALS;
        else if (v1 < v2)
            return COMPARE_VALUE_IS_LESSER;
        else
            return COMPARE_VALUE_IS_GREATER;
    }

    virtual void MakeArray(unsigned long ulSize)
    {
        ulArraySize = ulSize;
        tArray = (TPattern*)valloc(sizeof(TPattern) * ulSize);
    }

    virtual void FreeArray(void)
    {
        vfree(tArray);
    }

    virtual void SetItem(unsigned long ulIndex, TPattern tValue)
    {
        tArray[ulIndex] = tValue;
    }

    virtual TPattern operator[](const unsigned long ulIndex)
    {
        return tArray[ulIndex];
    }

public:
    FOUNDRANGE FindAll(TPattern tPattern)
    {
        bool bContinue = true;
        FOUNDRANGE fr = Find(tPattern);

        if (fr.start >= 0)
        {
            while (bContinue == true)
            {
                if (fr.start == 0)
                    bContinue = false;
                else if (_compareV1V2(tArray[fr.start - 1], tPattern) != COMPARE_VALUE_EQUALS)
                    bContinue = false;
                else
                    fr.start--;
            }

            bContinue = true;

            while (bContinue == true)
            {
                if (fr.finis == (ulArraySize - 1))
                    bContinue = false;
                else if (_compareV1V2(tArray[fr.finis + 1], tPattern) != COMPARE_VALUE_EQUALS)
                    bContinue = false;
                else
                    fr.finis++;
            }
        }

        return fr;
    }

    FOUNDRANGE Find(TPattern tPattern)
    {
        FOUNDRANGE fr;
        fr.start = 0;
        fr.finis = (ulArraySize - 1);
        long lCurrIndex = (fr.finis - fr.start) / 2;
        int iAttempts = 0;
		int compReturn = 0;

        // check to see if we are outside the lowest value
		
        compReturn = _compareV1V2(tPattern, tArray[fr.start]);
        
        
        if (compReturn == COMPARE_VALUE_IS_LESSER)
        {
            
            fr.start = SEARCH_VALUE_TOO_LOW;
            fr.finis = SEARCH_VALUE_TOO_LOW;
            return fr;
        }
		else if (compReturn == COMPARE_VALUE_EQUALS)
		{
            
			fr.finis = 0;
			return fr;
		}

		compReturn = _compareV1V2(tPattern, tArray[fr.finis]);

        // check to see if we are outside the highest value
        
        if (compReturn == COMPARE_VALUE_IS_GREATER)
        {   
            fr.start = SEARCH_VALUE_TOO_HIGH;
            fr.finis = SEARCH_VALUE_TOO_HIGH;
            return fr;
        }
		else if (compReturn == COMPARE_VALUE_EQUALS)
		{
			fr.start = fr.finis;
			return fr;
		}

        // printf("low\tmid\thigh\t\tvalue\r\n");
        // printf("------- ------- -------         --------\r\n");

        while (iAttempts <= SEARCH_LIMIT) // this is a built in save mechanism that should NEVER be reached
        {
            // printf("%i\t%i\t%i\t\t%i\r\n", fr.start, lCurrIndex, fr.finis, tArray[lCurrIndex]);

			if (fr.start == fr.finis)
            {
                if (_compareV1V2(tArray[fr.start], tPattern) == COMPARE_VALUE_EQUALS)
                {
                    return fr;
                }
                else
                {
                    fr.start = SEARCH_VALUE_NOT_FOUND;
                    fr.finis = SEARCH_VALUE_NOT_FOUND;
                    return fr;
                }
            }

			compReturn = _compareV1V2(tArray[lCurrIndex], tPattern);
            
			if (compReturn == COMPARE_VALUE_EQUALS)
            {
                fr.start = lCurrIndex;
                fr.finis = lCurrIndex;
                return fr;
            }
            else
            {
                if (compReturn == COMPARE_VALUE_IS_GREATER)
                {
                    fr.finis = ((lCurrIndex - 1) > fr.start ? lCurrIndex - 1 : fr.start);
                    lCurrIndex = ((fr.finis - fr.start) / 2) + fr.start;
                }
                else
                {
                    fr.start = ((lCurrIndex + 1) < fr.finis ? lCurrIndex + 1 : fr.finis);
                    lCurrIndex = ((fr.finis - fr.start) / 2) + fr.start;
                }
            }

            iAttempts++;
        }

        fr.start = SEARCH_VALUE_NOT_FOUND;
        fr.finis = SEARCH_VALUE_NOT_FOUND;
   
        return fr;
    }

    FOUNDRANGE FindBetween(TPattern tPatternStart, TPattern tPatternFinis)
    {
        FOUNDRANGE frReturn;

        frReturn.start = 0;
        frReturn.finis = (ulArraySize - 1);

        if (_compareV1V2(tPatternStart, tPatternFinis) == COMPARE_VALUE_IS_GREATER)
        {
            TPattern tTemp;

            tTemp = tPatternFinis;
            tPatternFinis = tPatternStart;
            tPatternStart = tTemp;
        }

        {
            FOUNDRANGE fr;
            fr.start = 0;
            fr.finis = (ulArraySize - 1);
            long lCurrIndex = (fr.finis - fr.start) / 2;
            int iAttempts = 0;
			int compReturn = 0;

			// check to see if we are outside the lowest value
            if (_compareV1V2(tPatternStart, tArray[fr.start]) == COMPARE_VALUE_IS_LESSER)
            {
                goto _FindFinis;
            }

            // if the start pattern is outside the bounds
            if (_compareV1V2(tPatternStart, tArray[fr.finis]) == COMPARE_VALUE_IS_GREATER)
            {
                frReturn.start = SEARCH_VALUE_TOO_HIGH;
                frReturn.finis = SEARCH_VALUE_TOO_HIGH;
                goto _FoundAll;
            }

            while (iAttempts <= SEARCH_LIMIT) // this is a built in save mechanism that should NEVER be reached
            {
                if (fr.start == fr.finis)
                {
					compReturn = _compareV1V2(tArray[fr.start], tPatternStart);

                    if (compReturn == COMPARE_VALUE_EQUALS)
                    {
                        frReturn.start = fr.start;
                        goto _FindFinis;
                    }
                    else
                    {
                        if (compReturn == COMPARE_VALUE_IS_GREATER)
                        {
                            frReturn.start = fr.start;
                        }
                        else
                        {
                            frReturn.start = fr.start + 1;
                        }

                        goto _FindFinis;
                    }
                }

                compReturn = _compareV1V2(tArray[lCurrIndex], tPatternStart);
				
				if (compReturn == COMPARE_VALUE_EQUALS)
                {
                    frReturn.start = lCurrIndex;
                    goto _FindFinis;
                }
                else
                {
                    if (compReturn == COMPARE_VALUE_IS_GREATER)
                    {
                        fr.finis = ((lCurrIndex - 1) > fr.start ? lCurrIndex - 1 : fr.start);
                        lCurrIndex = ((fr.finis - fr.start) / 2) + fr.start;
                    }
                    else
                    {
                        fr.start = ((lCurrIndex + 1) < fr.finis ? lCurrIndex + 1 : fr.finis);
                        lCurrIndex = ((fr.finis - fr.start) / 2) + fr.start;
                    }
                }

                iAttempts++;
            }
        }

        _FindFinis:
        {
            FOUNDRANGE fr;
            fr.start = 0;
            fr.finis = (ulArraySize - 1);
            long lCurrIndex = (fr.finis - fr.start) / 2;
            int iAttempts = 0;
			int compReturn = 0;

            // check to see if we are outside the lowest value
            if (_compareV1V2(tPatternFinis, tArray[fr.start]) == COMPARE_VALUE_IS_LESSER)
            {
                frReturn.start = SEARCH_VALUE_TOO_LOW;
                frReturn.finis = SEARCH_VALUE_TOO_LOW;
                goto _FoundAll;
            }

            // if the start pattern is outside the bounds
            if (_compareV1V2(tPatternFinis, tArray[fr.finis]) == COMPARE_VALUE_IS_GREATER)
            {
                goto _FoundAll;
            }

            while (iAttempts <= SEARCH_LIMIT) // this is a built in save mechanism that should NEVER be reached
            {
                if (fr.start == fr.finis)
                {
					compReturn = _compareV1V2(tArray[fr.start], tPatternFinis);

                    if (compReturn == COMPARE_VALUE_EQUALS)
                    {
                        frReturn.finis = fr.start;
                        goto _FoundAll;
                    }
                    else
                    {
                        if (compReturn == COMPARE_VALUE_IS_LESSER)
                        {
                            frReturn.finis = fr.start;
                        }
                        else
                        {
                            frReturn.finis = fr.start - 1;
                        }

                        goto _FoundAll;
                    }
                }

				compReturn = _compareV1V2(tArray[lCurrIndex], tPatternFinis);

                if (compReturn == COMPARE_VALUE_EQUALS)
                {
                    frReturn.finis = lCurrIndex;
                    goto _FoundAll;
                }
                else
                {
                    if (compReturn == COMPARE_VALUE_IS_GREATER)
                    {
                        fr.finis = ((lCurrIndex - 1) > fr.start ? lCurrIndex - 1 : fr.start);
                        lCurrIndex = ((fr.finis - fr.start) / 2) + fr.start;
                    }
                    else
                    {
                        fr.start = ((lCurrIndex + 1) < fr.finis ? lCurrIndex + 1 : fr.finis);
                        lCurrIndex = ((fr.finis - fr.start) / 2) + fr.start;
                    }
                }

                iAttempts++;
            }
        }

        _FoundAll:

        bool bContinue = true;

        if (frReturn.start >= 0)
        {
            while (bContinue == true)
            {
                if (frReturn.start == 0)
                    bContinue = false;
                else if (_compareV1V2(tArray[frReturn.start - 1], tPatternStart) != COMPARE_VALUE_EQUALS)
                    bContinue = false;
                else
                    frReturn.start--;
            }

            bContinue = true;

            while (bContinue == true)
            {
                if (frReturn.finis == (ulArraySize - 1))
                    bContinue = false;
                else if (_compareV1V2(tArray[frReturn.finis + 1], tPatternFinis) != COMPARE_VALUE_EQUALS)
                    bContinue = false;
                else
                    frReturn.finis++;
            }
        }

        return frReturn;
    }
};

template <typename TPattern> void BuildArray(mapbuilder<TPattern>& mb)
{
    // allocate the space for the array
    mb.MakeArray(ARRAY_SIZE);

    // populate the array
    for (long i = 0; i < ARRAY_SIZE; i++)
    {
        if (i == 0)
        {
            // start the initial values at 5
            mb.tArray[i] = 2;
        }
        else
        {
            // increase each array value 1 unless the index is "% 13" then increase by 2
            // this will create skips in the array
            if (i == 4)
                mb.tArray[i] = mb.tArray[3];
            else
                mb.tArray[i] = (i % 13) ? (mb.tArray[i - 1] + 2) : (mb.tArray[i - 1] + 1);
        }
    }
}

/*
template <typename TPattern> void Find1(mapbuilder<TPattern>& mb, TPattern tPattern)
{
    // find the value ... and clock the speed
    DWORD dwTick = GetTickCount();
    FOUNDRANGE fr = mb.Find(tPattern);
    printf("FIND FOR [%i] TOOK %i MILLISECONDS\r\n", tPattern, GetTickCount() - dwTick);

    // output the results
    if (fr.start >= 0)
        printf("found at [%i]\r\n\r\n", fr.start);
    else if (fr.start == SEARCH_VALUE_NOT_FOUND)
        printf("pattern NOT found\r\n\r\n");
    else if (fr.start == SEARCH_VALUE_TOO_LOW)
        printf("pattern too low\r\n\r\n");
    else if (fr.start == SEARCH_VALUE_TOO_HIGH)
        printf("pattern too high\r\n\r\n");
    else
        printf("UnKnOwN IsSuEs !!\r\n\r\n");
}

template <typename TPattern> void FindN(mapbuilder<TPattern>& mb, TPattern tPattern)
{
    // find the value ... and clock the speed
    DWORD dwTick = GetTickCount();
    FOUNDRANGE fr = mb.FindAll(tPattern);
    printf("FIND FOR [%i] TOOK %i MILLISECONDS\r\n", tPattern, GetTickCount() - dwTick);

    // output the results
    if (fr.start >= 0)
        printf("found at [%i,%i]\r\n\r\n", fr.start, fr.finis);
    else if (fr.start == SEARCH_VALUE_NOT_FOUND)
        printf("pattern NOT found\r\n\r\n");
    else if (fr.start == SEARCH_VALUE_TOO_LOW)
        printf("pattern too low\r\n\r\n");
    else if (fr.start == SEARCH_VALUE_TOO_HIGH)
        printf("pattern too high\r\n\r\n");
    else
        printf("UnKnOwN IsSuEs !!\r\n\r\n");
}

template <typename TPattern> void FindB(mapbuilder<TPattern>& mb, TPattern tPatternStart, TPattern tPatternFinis)
{
    // find the value ... and clock the speed
    DWORD dwTick = GetTickCount();
    FOUNDRANGE fr = mb.FindBetween(tPatternStart, tPatternFinis);
    printf("FIND FOR [%i,%i] TOOK %i MILLISECONDS\r\n", tPatternStart, tPatternFinis, GetTickCount() - dwTick);

    // output the results
    if (fr.start >= 0)
        printf("found at [%i,%i] values (%i,%i)\r\n\r\n", fr.start, fr.finis, mb[fr.start], mb[fr.finis]);
    else if (fr.start == SEARCH_VALUE_NOT_FOUND)
        printf("pattern NOT found\r\n\r\n");
    else if (fr.start == SEARCH_VALUE_TOO_LOW)
        printf("pattern too low\r\n\r\n");
    else if (fr.start == SEARCH_VALUE_TOO_HIGH)
        printf("pattern too high\r\n\r\n");
    else
        printf("UnKnOwN IsSuEs !!\r\n\r\n");
}
*/

class stringsmap : public mapbuilder<char*>
{
public:
    virtual int _compareV1V2(char* v1, char* v2)
    {
		return strcmp(v1, v2);
    }

    virtual void MakeArray(unsigned long ulSize)
    {
        ulArraySize = ulSize;
        tArray = (char**)valloc(sizeof(char*) * ulSize);

        for (unsigned long ul = 0; ul < ulSize; ul++)
        {
            tArray[ul] = NULL;
        }
    }

    virtual void FreeArray(void)
    {
        if (ulArraySize > 0)
        {
            for (long ul = 0; ul < ulArraySize; ul++)
            {
                if (tArray[ul] != NULL)
                {
                    vfree(tArray[ul]);
                }
            }

            vfree(tArray);
        }

        ulArraySize = 0;
        tArray = NULL;
    }

    virtual void SetItem(unsigned long ulIndex, char* tValue)
    {
        if (tArray[ulIndex] != NULL)
        {
            vfree((char*)tArray[ulIndex]);
        }

        tArray[ulIndex] = (char*)valloc(strlen(tValue) + 1);
        lstrcpy(tArray[ulIndex], tValue);
    }
};
