/* version 2.0 */

#include <stdio.h>
#include "windows.h"

#ifndef PERFTIMER_CLASS_098F2470_11CD_BAE0_B579_08002B30BBEF
#define PERFTIMER_CLASS_098F2470_11CD_BAE0_B579_08002B30BBEF

class PerfTimer
{
private:
	LARGE_INTEGER Frequency;
	LARGE_INTEGER StartTime;
	LARGE_INTEGER EndTime;

private:
	char szDuration[30];

public:
	PerfTimer::PerfTimer(void)
	{
		QueryPerformanceFrequency(&Frequency);
		ZeroMemory(szDuration, sizeof(szDuration) / sizeof(char));
	}

	void Start(void)
	{
		QueryPerformanceCounter(&StartTime);
	}

	void Stop(void)
	{
		LONGLONG liDifference1;
		LONGLONG liDifference2;
		LARGE_INTEGER LagStartTime;
		LARGE_INTEGER LagEndTime;

		QueryPerformanceCounter(&EndTime);
		ZeroMemory(szDuration, sizeof(szDuration) / sizeof(char));

		QueryPerformanceCounter(&LagStartTime);
		QueryPerformanceCounter(&LagEndTime);

		EndTime.QuadPart = EndTime.QuadPart - (LagEndTime.QuadPart - LagStartTime.QuadPart);

		liDifference1 = (EndTime.QuadPart - StartTime.QuadPart) / Frequency.QuadPart;
		liDifference2 = (EndTime.QuadPart - StartTime.QuadPart);

		sprintf(szDuration, "%I64d-%2I64d:%2I64d:%2I64d.%3I64d.%3I64d.%3I64d", 
			((liDifference1 / 60) / 60) / 24,                            // days
			((liDifference1 / 60) / 60) % 24,                            // hours
			(liDifference1 / 60) % 60,                                   // minutes
			liDifference1 % 60,                                          // seconds
			((liDifference2 * 1000) / Frequency.QuadPart) % 1000,        // milliseconds
			((liDifference2 * 1000000) / Frequency.QuadPart) % 1000,     // microseconds
			((liDifference2 * 1000000000) / Frequency.QuadPart) % 1000); // nanoseconds
		
		for (size_t iCharIndex = 0; iCharIndex < strlen(szDuration); ++iCharIndex)
		{
			if (szDuration[iCharIndex] == ' ')
			{
				szDuration[iCharIndex] = '0';
			}
		}
	}

	LONGLONG Nanoseconds(void)
	{
		return (1000000000 * (EndTime.QuadPart - StartTime.QuadPart)) / Frequency.QuadPart;
	}

	LONGLONG Microseconds(void)
	{
		return (1000000 * (EndTime.QuadPart - StartTime.QuadPart)) / Frequency.QuadPart;
	}

	LONGLONG Milliseconds(void)
	{
		return (1000 * (EndTime.QuadPart - StartTime.QuadPart)) / Frequency.QuadPart;
	}

	LONGLONG Seconds(void)
	{
		return (EndTime.QuadPart - StartTime.QuadPart) / Frequency.QuadPart;
	}

	LONGLONG Minutes(void)
	{
		return ((EndTime.QuadPart - StartTime.QuadPart) / Frequency.QuadPart) / 60;
	}

	LONGLONG Hours(void)
	{
		return (((EndTime.QuadPart - StartTime.QuadPart) / Frequency.QuadPart) / 60) / 60;
	}

	LONGLONG Days(void)
	{
		return ((((EndTime.QuadPart - StartTime.QuadPart) / Frequency.QuadPart) / 60) / 60) / 24;
	}

	char* GetDuration(void)
	{
        return szDuration;
	}
};

#endif
