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
	//ʶ��������ߵ�����λ��
	GetGridLinesPos(img,HPosVec,VPosVec);
	if (HPosVec.size()<3 ||VPosVec.size()<3)
	{
		return _T("");
	}
	//��������п��ܵĸ��ӿ�Ⱥͳ���
	CSize wh = GetGridCellWH(HPosVec,VPosVec);
	//��������û��ȡ������û�о����һ������λ�ú����һ������λ��
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

	//ʶ��ÿ�����ӵ�״̬����ɫ����ɫ�����ߴ���б�ܵģ�
	//����ʶ��ÿ�����ӣ�ÿ�����Ӻ���ʶ��ÿ�����أ���ֻʶ������м���ŵ�һ��ֱ���ϵ�����
	CString strParse;
	for (size_t i=1; i<VPosVec.size(); ++i)
	{
		//���ŵ�һ�и���
		bool bFinish = false;
		bool bDifferClr = false;
		bool bBlueCell = false;
		int gridMid=0;
		size_t j=1;
		for (; j<HPosVec.size(); ++j)
		{
			//ʶ��ÿ�����ӵ��м���ŵ�һ��ֱ���ϵ�����
			gridMid = (HPosVec[j-1]+HPosVec[j])/2;
			CString strCellClr = GetCellClr(img,VPosVec,(int)i,gridMid);
			if (strCellClr.IsEmpty())
			{//���������ǿհ׸������
				if (j==1 && i!=1)
				{//��һ�����Ӿ��ǿհ׵ģ��Ǻ���ľͲ�ʶ����,���������һ�о��ǿհף��ǻ��Ǽ���ʶ��
					bFinish = true;
				}
				break;
			}
			bool bBlue = IsBlueClr(strCellClr);
			if (j==1)
			{//��¼���ŵĵ�һ�����ӵ���ɫ
				bBlueCell = bBlue;
			}
			else if (bBlueCell^bBlue)
			{//������ɫ���ֲ�ͬ�������
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
		{//���һ�����ӵ��ұ߸��ӵ���ɫ,Ҳ����ʶ��L�ε� ���� ǰһ����L�κ�һ��Ҳ��L��
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
				{//������ɫ���ֲ�ͬ�������
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
	//ʶ��ÿ�����ӵ��м���ŵ�һ��ֱ���ϵ�����
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
	//����ǿհף�������ĸ��ӾͲ�ʶ����
	if (nRed==0 && nGreen==0 && nBlue==0)
	{
		return strClr;
	}

	if (nRed>nBlue)
	{
// 		if (nGreen>0)
// 		{
// 			//0�����ɫ������ɫ��
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
// 			//1������ɫ������ɫ��
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
	
	//��Ϊ����ɫ���ﵽһ����������Ϊ�ҵ�gridline��
	double ratio=0.4;
	int ratioV = int(nHeight*ratio);
	int ratioH = int(nWidth*ratio);

	//���������ߵ�����λ��
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
			//��ÿ���ҵ�����ֱ��gridline������λ�ñ���
			VPosVec.push_back(i);
		}
	}

	//����ˮƽ�ߵ�����λ��
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
			//��ÿ���ҵ���ˮƽ��gridline������λ�ñ���
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
