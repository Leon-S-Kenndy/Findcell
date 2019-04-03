
// painterView.h : CpainterView 类的接口
//

#pragma once


class CpainterView : public CView
{
protected: // 仅从序列化创建
	CpainterView();
	DECLARE_DYNCREATE(CpainterView)

// 特性
public:
	CpainterDoc* GetDocument() const;
 
public:
	//添加成员函数  
	void ShowBitmap(CDC* pDC, CString BmpName); //显示位图函数  
	bool ReadBmp();                            //用来读取bmp个手机图片  
	bool SaveBmp(LPCSTR lpFileName);           //用来保存bmp格式图片  

											   //添加成员变量  
	CString EntName;     //图像文件扩展名  
	CString BmpName;     //图像文件名称  
	CBitmap m_bitmap;    //创建位图对象  

	CBitmap m_bitmaplin;   //创建临时位图对象进行处理---  
	CString BmpNameLin;    //保存图像副本文件---  
	CBitmap m_bitmaplin1;   //创建临时位图对象进行处理---  
	CString BmpNameLin1;    //保存图像副本文件---  
	int next = 1;

	int m_nWidth;       //图像实际宽度  
	int m_nHeight;      //图像实际高度  
	int m_nDrawWidth;   //图像显示宽度  
	int m_nDrawHeight;  //图像显示高度  
	DWORD m_nImage;     //图像数据的字节数 只含位图  
	DWORD m_nSize;      //图像文件大小  
	int m_nLineByte;    //图像一行所占字节数  
	int m_nBitCount;    //图像每个像素所占位数  
	int m_nPalette;     //位图实际使用的颜色表中的颜色数  

	BYTE *m_pImage;         //读入图片数据后的指针  
	BITMAPFILEHEADER bfh;   //全局变量文件头  
	BITMAPINFOHEADER bih;   //全局变量信息头  
	RGBQUAD m_pPal;         //颜色表指针  
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CpainterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();  //点击打开按钮
	afx_msg void OnFileSave();	//点击保存按钮
	afx_msg void OnShowTwo();   //点击显示两张图片
	afx_msg void OnNext();      //点击手动上色
	afx_msg void OnStart();     //点击自动上色
};

#ifndef _DEBUG  // painterView.cpp 中的调试版本
inline CpainterDoc* CpainterView::GetDocument() const
   { return reinterpret_cast<CpainterDoc*>(m_pDocument); }
#endif

