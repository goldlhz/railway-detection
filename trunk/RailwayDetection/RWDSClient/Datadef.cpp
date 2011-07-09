
#include "StdAfx.h"

#include "Datadef.h"

vector<CString> strRailLineName;
int strRailLineNameCount = 0;

const CString strDirectName[] = {
							  _T("上下行"),
							  _T("上行"),
							  _T("下行")
							 };
const int strDirectNameCount = 3;

const CString StrKm = _T("公里");

const CString strStartNo[] = {
                              _T("未指定"),
							  _T("第一天"),
							  _T("第二天"),
							  _T("第三天"),
							  _T("第四天"),
							  _T("第五天"),
							  _T("第六天"),
							  _T("第七天")
							 };
const int strStartNoCount = 8;
const CString strClientCaption = _T("铁路巡查系统");

const CString strPictureErrorType[] = {
                                       _T("未知故障"),
                                       _T("损伤发展"),
                                       _T("钢轨折断"),
                                       _T("联配件松动失效"),
                                       _T("轨枕失效"),
                                       _T("道床翻浆"),
                                       _T("边坡溜坍"),
                                      };
const int strPictureErrorTypeCount = 7;

const CString strPointState[] = {
                                _T("未知错误"),
                                _T("正常"),
                                _T("未按时到达"),
                                _T("未到")
                                };
const int strPointStateCount = 4;