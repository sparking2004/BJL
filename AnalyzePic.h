#pragma once
#include "atlimage.h"
#include <vector>

class CAnalyzePic
{
public:
	/// ����ͼƬ����һ��������
	CString AnalyzePic(const CString& strPic);
	CAnalyzePic(void);
	~CAnalyzePic(void);
private:
	double	GetSeCha(COLORREF clr1, COLORREF clr2);
	bool	IsGridLineClr(COLORREF clr);
	bool	IsRedClr(COLORREF clr);
	bool	IsGreenClr(COLORREF clr);
	bool	IsBlueClr(COLORREF clr);
	bool	IsBlueClr(const CString& strClr);
	bool	IsRedClr(const CString& strClr);

	void	GetGridLinesPos(CImage& img, std::vector<int>& HPosVec, std::vector<int>& VPosVec);
	/// ��ȡ���ӵĴ�С����������ߣ������ش�С����
	CSize	GetGridCellWH(const std::vector<int>& HPosVec, const std::vector<int>& VPosVec);
	/// ��һ��ֵ��ȥǰһ��ֵ�ó���һ��ֵ���ֵĴ��������Ǹ�
	int		GetMostChaJi(const std::vector<int>& HPosVec);
	/// ��ɫr ��ɫ������0 ��ɫb ��ɫ������1���������ؿ�
	CString	GetCellClr(CImage& img,const std::vector<int>& VPosVec,int nCol,int nGridMid);

private:
	void	TestClr();
};

class CCompareRGB
{
public:
	CCompareRGB(COLORREF clr, int thresholdVal):m_clr(clr),m_thresholdVal(thresholdVal){}
	COLORREF m_clr;		///< �Աȵ���ɫ
	int	m_thresholdVal; ///< ��ֵ
};
