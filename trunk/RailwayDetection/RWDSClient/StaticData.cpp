#include "StdAfx.h"
#include "StaticData.h"


CStaticData::CStaticData(void)
{
}


CStaticData::~CStaticData(void)
{
}

void CStaticData::CopyLineInfoWithoutTime( LineInfo* aLineSrc, LineInfo* aLineDes )
{
	aLineDes->iLineID = aLineSrc->iLineID;
	aLineDes->iOrgID = aLineSrc->iOrgID;
	aLineDes->iLineName = aLineSrc->iLineName;
	aLineDes->iStartKm = aLineSrc->iStartKm;
	for (size_t i=0; i<aLineSrc->iLineKmLonLat.size(); i++)
	{
		aLineDes->iLineKmLonLat.push_back(aLineDes->iLineKmLonLat[i]);
		aLineDes->iLineKmTime.push_back(0);
	}
}
