#include "StdAfx.h"
#include "AnalyzePic.h"
#include <math.h>
#include <map>

using namespace std;

static CCompareRGB g_gridLineClr(RGB(149,149,148),60);
static CCompareRGB g_redClr(RGB(255,0,0),100);
static CCompareRGB g_greenClr(RGB(0,255,0),100);
static CCompareRGB g_blueClr(RGB(0,0,255),100);
static char g_cBlue='b';
static char g_cBlueGreen = '1';
static char g_cRed='r';
static char g_cRedGreen='0';
void CAnalyzePic::TestClr()
{
	COLORREF clrBlue1=RGB(10,10,255);
	COLORREF clrBlue2=RGB(37,37,228);
	COLORREF clrBlue3=RGB(49,49,255);
	COLORREF clrBlue4=RGB(81,81,255);
	COLORREF clrBlue5=RGB(46,46,255);
	COLORREF clrBlue6=RGB(98,98,255);
	COLORREF clrBlue7=RGB(150,150,255);

	double b1 = GetSeCha(clrBlue1,RGB(149,149,148));
	double b2 = GetSeCha(clrBlue2,RGB(149,149,148));
	double b3 = GetSeCha(clrBlue3,RGB(149,149,148));
	double b4 = GetSeCha(clrBlue4,RGB(149,149,148));
	double b5 = GetSeCha(clrBlue5,RGB(149,149,148));
	double b6 = GetSeCha(clrBlue6,RGB(149,149,148));
	double b7 = GetSeCha(clrBlue7,RGB(149,149,148));

	double b8 = GetSeCha(RGB(149,149,148),RGB(134,134,133));
}

CAnalyzePic::CAnalyzePic(void)
{
	//TestClr();
}

CAnalyzePic::~CAnalyzePic(void)
{
}

CString CAnalyzePic::AnalyzePic(const CString& strPic)
{
	CImage img;
	HRESULT hr = img.Load(strPic);
	if (FAILED(hr))
	{
		return _T("");
	}
	std::vector<int> HPosVec;
	std::vector<int> VPosVec;
	//识别出格子线的像素位置
	GetGridLinesPos(img,HPosVec,VPosVec);
	if (HPosVec.size()<3 ||VPosVec.size()<3)
	{
		return _T("");
	}
	//计算出最有可能的格子宽度和长度
	CSize wh = GetGridCellWH(HPosVec,VPosVec);
	//看格子有没截取完整，没有就填补第一个像素位置和最后一个像素位置
	if (HPosVec[0]>wh.cy/2)
	{
		HPosVec.insert(HPosVec.begin(),0);
	}
	if (VPosVec[0]>wh.cx/2)
	{
		VPosVec.insert(VPosVec.begin(),0);
	}
	if ((img.GetHeight()-HPosVec.back())>wh.cy/2)
	{
		HPosVec.push_back(img.GetHeight()-1);
	}
	if ((img.GetWidth()-VPosVec.back())>wh.cx/2)
	{
		VPosVec.push_back(img.GetWidth()-1);
	}

	//识别每个格子的状态（蓝色，红色，或者带绿斜杠的）
	//竖着识别每个格子，每个格子横着识别每个像素，且只识别格子中间横着的一条直线上的像素
	CString strParse;
	for (size_t i=1; i<VPosVec.size(); ++i)
	{
		//竖着的一列格子
		bool bFinish = false;
		bool bDifferClr = false;
		bool bBlueCell = false;
		int gridMid=0;
		size_t j=1;
		for (; j<HPosVec.size(); ++j)
		{
			//识别每个格子的中间横着的一条直线上的像素
			gridMid = (HPosVec[j-1]+HPosVec[j])/2;
			CString strCellClr = GetCellClr(img,VPosVec,(int)i,gridMid);
			if (strCellClr.IsEmpty())
			{//出现下面是空白格，则结束
				if (j==1 && i!=1)
				{//第一个格子就是空白的，那后面的就不识别了,但是如果第一列就是空白，那还是继续识别
					bFinish = true;
				}
				break;
			}
			bool bBlue = IsBlueClr(strCellClr);
			if (j==1)
			{//记录竖着的第一个格子的颜色
				bBlueCell = bBlue;
			}
			else if (bBlueCell^bBlue)
			{//格子颜色出现不同，则结束
				bDifferClr = true;
				gridMid = (HPosVec[j-2]+HPosVec[j-1])/2;
				break;
			}
			strParse+=strCellClr;
		}

		if (bFinish)
		{
			break;
		}

		if (j==HPosVec.size() || bDifferClr)
		{//最后一个格子的右边格子的颜色,也就是识别L形的 或者 前一排是L形后一排也是L形
			size_t nextIndex=i;
			++nextIndex;
			for (; nextIndex<VPosVec.size(); ++nextIndex)
			{
				CString strCellClr = GetCellClr(img,VPosVec,(int)nextIndex,gridMid);
				if (strCellClr.IsEmpty())
				{
					break;
				}
				bool bBlue = IsBlueClr(strCellClr);
				if (bBlueCell^bBlue)
				{//格子颜色出现不同，则结束
					break;
				}
				strParse+=strCellClr;
			}
		}

		strParse+=_T("/");
	}
	if (!strParse.IsEmpty() && (strParse[0]=='/'))
	{
		strParse.Delete(0);
	}
	return strParse;
}

CString	CAnalyzePic::GetCellClr(CImage& img,const std::vector<int>& VPosVec,int nCol,int nGridMid)
{
	CString strClr;
	//识别每个格子的中间横着的一条直线上的像素
	int nRed=0,nGreen=0,nBlue=0;
	for (int x=VPosVec[nCol-1]+1; x<VPosVec[nCol]; ++x)
	{
		COLORREF clr = img.GetPixel(x,nGridMid);
		if (IsRedClr(clr))
		{
			++nRed;
		}
		else if (IsBlueClr(clr))
		{
			++nBlue;
		}
		else if (IsGreenClr(clr))
		{
			++nGreen;
		}
	}
	//如果是空白，则下面的格子就不识别了
	if (nRed==0 && nGreen==0 && nBlue==0)
	{
		return strClr;
	}

	if (nRed>nBlue)
	{
// 		if (nGreen>0)
// 		{
// 			//0代表红色中有绿色的
// 			strClr+=g_cRedGreen;
// 		}
// 		else
// 		{
			strClr+=g_cRed;
//		}
	}
	else
	{
// 		if (nGreen)
// 		{
// 			//1代表蓝色中有绿色的
// 			strClr+=g_cBlueGreen;
// 		}
// 		else
// 		{
			strClr+=g_cBlue;
//		}
	}
	return strClr;
}

double CAnalyzePic::GetSeCha(COLORREF clr1, COLORREF clr2)
{
	int nR1,nG1,nB1,nR2,nG2,nB2;
	nR1 = GetRValue(clr1);
	nG1 = GetGValue(clr1);
	nB1 = GetBValue(clr1);

	nR2 = GetRValue(clr2);
	nG2 = GetGValue(clr2);
	nB2 = GetBValue(clr2);
	
	int nRC = abs(nR2-nR1);
	int nGC = abs(nG2-nG1);
	int nBC = abs(nB2-nB1);
	double f=nRC*nRC+nGC*nGC+nBC*nBC;
	return sqrt(f);
}

bool CAnalyzePic::IsGridLineClr(COLORREF clr)
{
	double sc = GetSeCha(clr,g_gridLineClr.m_clr);
	return sc<g_gridLineClr.m_thresholdVal;
}

bool CAnalyzePic::IsRedClr(COLORREF clr)
{
	double sc = GetSeCha(clr,g_redClr.m_clr);
	return sc<g_redClr.m_thresholdVal;
}

bool CAnalyzePic::IsGreenClr(COLORREF clr)
{
	double sc = GetSeCha(clr,g_greenClr.m_clr);
	return sc<g_greenClr.m_thresholdVal;
}

bool CAnalyzePic::IsBlueClr(COLORREF clr)
{
	double sc = GetSeCha(clr,g_blueClr.m_clr);
	return sc<g_blueClr.m_thresholdVal;
}

bool CAnalyzePic::IsBlueClr(const CString& strClr)
{
	return strClr==g_cBlue || strClr==g_cBlueGreen;
}

bool CAnalyzePic::IsRedClr(const CString& strClr)
{
	return strClr==g_cRed || strClr==g_cRedGreen;
}

void CAnalyzePic::GetGridLinesPos(CImage& img, std::vector<int>& HPosVec, std::vector<int>& VPosVec)
{

	int nWidth = img.GetWidth();
	int nHeight = img.GetHeight();
	
	//认为当颜色数达到一定比例就认为找到gridline了
	double ratio=0.4;
	int ratioV = int(nHeight*ratio);
	int ratioH = int(nWidth*ratio);

	//查找竖向线的像素位置
	for (int i=0; i<nWidth; ++i)
	{
		int curRatioV=0;
		for (int j=0; j<nHeight; ++j)
		{
			COLORREF clr = img.GetPixel(i,j);
			IsGridLineClr(clr)?++curRatioV:0;
		}
		if (curRatioV>=ratioV)
		{
			//把每次找到的竖直的gridline的像素位置保存
			VPosVec.push_back(i);
		}
	}

	//查找水平线的像素位置
	for (int i=0; i<nHeight; ++i)
	{
		int curRatioH=0;
		for (int j=0; j<nWidth; ++j)
		{
			COLORREF clr = img.GetPixel(j,i);
			IsGridLineClr(clr)?++curRatioH:0;
		}
		if (curRatioH>=ratioH)
		{
			//把每次找到的水平的gridline的像素位置保存
			HPosVec.push_back(i);
		}
	}
}

CSize CAnalyzePic::GetGridCellWH(const std::vector<int>& HPosVec, const std::vector<int>& VPosVec)
{
	CSize sz;
	sz.cx = GetMostChaJi(HPosVec);
	sz.cy = GetMostChaJi(VPosVec);
	return sz;
}

int CAnalyzePic::GetMostChaJi(const std::vector<int>& HPosVec)
{
	map<int, int> numToHitMap;
	for (size_t i=1; i<HPosVec.size(); ++i)
	{
		int num = HPosVec[i]-HPosVec[i-1];
		map<int, int>::iterator itFind = numToHitMap.find(num);
		if (itFind!=numToHitMap.end())
		{
			numToHitMap[num]++;
		}
		else
		{
			numToHitMap[num]=1;
		}
	}

	map<int, int>::iterator maxIt = numToHitMap.begin();
	map<int, int>::iterator it = ++numToHitMap.begin();
	
	for (; it!=numToHitMap.end(); ++it)
	{
		if (maxIt->second<it->second)
		{
			maxIt = it;
		}
	}
	return maxIt->first;
}
