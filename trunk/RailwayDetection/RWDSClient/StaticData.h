#pragma once
#include "Datadef.h"
class CStaticData
{
public:
	CStaticData(void);
	~CStaticData(void);
public:
	static void CopyLineInfoWithoutTime(LineInfo* aLineSrc, LineInfo* aLineDes);
};

