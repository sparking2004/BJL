#pragma once
#include "atlimage.h"
#include <vector>

class CAnalyzePic
{
public:
	/// 分析图片返回一个特征码
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
	/// 获取格子的大小不包含外边线，以像素大小返回
	CSize	GetGridCellWH(const std::vector<int>& HPosVec, const std::vector<int>& VPosVec);
	/// 后一个值减去前一个值得出的一个值出现的次数最多的那个
	int		GetMostChaJi(const std::vector<int>& HPosVec);
	/// 红色r 红色带绿条0 蓝色b 蓝色带绿条1，其他返回空
	CString	GetCellClr(CImage& img,const std::vector<int>& VPosVec,int nCol,int nGridMid);

private:
	void	TestClr();
};

class CCompareRGB
{
public:
	CCompareRGB(COLORREF clr, int thresholdVal):m_clr(clr),m_thresholdVal(thresholdVal){}
	COLORREF m_clr;		///< 对比的颜色
	int	m_thresholdVal; ///< 阀值
};
