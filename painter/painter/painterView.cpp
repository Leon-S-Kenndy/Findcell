
// painterView.cpp : CpainterView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "painter.h"
#endif

#include "painterDoc.h"
#include "painterView.h"
#define Random(x) (rand() % x)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include<iostream>
using namespace std;

// CpainterView

IMPLEMENT_DYNCREATE(CpainterView, CView)

BEGIN_MESSAGE_MAP(CpainterView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CpainterView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CpainterView::OnFileSave)
	ON_COMMAND(ID_SHOW_TWO, &CpainterView::OnShowTwo)
//	ON_COMMAND(ID_SHOW_HD, &CpainterView::OnShowHd)
	ON_COMMAND(ID_next, &CpainterView::OnNext)
	ON_COMMAND(ID_start, &CpainterView::OnStart)
END_MESSAGE_MAP()

// CpainterView 构造/析构

CpainterView::CpainterView()
{
	// TODO: 在此处添加构造代码

}
/*************************************************************/
/* numPicture变量显示图片数量
/* 0-提示错误或未打开图片 1-显示一张图片 2-显示两张图片和处理
/*************************************************************/
int numPicture = 0;

/*************************************************************/
/* level变量显示具体的处理操作，每个处理函数中赋值该变量
/* 0-显示2张图片 1-显示灰度图片 3-显示图片采样
/* 2 4 8 16 32 64-不同量化等级量化图片
/*************************************************************/
int level = 0;
void CpainterView::ShowBitmap(CDC * pDC, CString BmpName)
{
	//定义bitmap指针 调用函数LoadImage装载位图  
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

	/*************************************************************************/
	/* 1.要装载OEM图像，则设此参数值为0  OBM_ OEM位图 OIC_OEM图标 OCR_OEM光标
	/* 2.BmpName要装载图片的文件名
	/* 3.装载图像类型:
	/*   IMAGE_BITMAP-装载位图 IMAGE_CURSOR-装载光标 IMAGE_ICON-装载图标
	/* 4.指定图标或光标的像素宽度和长度 以像素为单位
	/* 5.加载选项:
	/*   IR_LOADFROMFILE-指明由lpszName指定文件中加载图像
	/*   IR_DEFAULTSIZE-指明使用图像默认大小
	/*   LR_CREATEDIBSECTION-当uType参数为IMAGE_BITMAP时,创建一个DIB项
	/**************************************************************************/

	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();           //切断CWnd和窗口联系  
	}
	m_bitmap.Attach(m_hBitmap);      //将句柄HBITMAP m_hBitmap与CBitmap m_bitmap关联  

										//边界  
	CRect rect;
	GetClientRect(&rect);

	//图片显示(x,y)起始坐标  
	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;   //计算客户区宽度  
	int m_nWindowHeight = rect.bottom - rect.top;  //计算客户区高度  

													//定义并创建一个内存设备环境DC  
	CDC dcBmp;
	if (!dcBmp.CreateCompatibleDC(pDC))   //创建兼容性的DC  
		return;
	BITMAP m_bmp;                          //临时bmp图片变量  
	m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中  
	CBitmap *pbmpOld = NULL;
	dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境  

											//图片显示调用函数stretchBlt  
	pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
		m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);

	/*******************************************************************************/
	/* BOOL StretchBlt(int x,int y,int nWidth,int nHeight,CDC* pSrcDC,
	/*                 int xSrc,int ySrc,int nSrcWidth,int nSrcHeight,DWORD dwRop );
	/* 1.参数x、y位图目标矩形左上角x、y的坐标值
	/* 2.nWidth、nHeigth位图目标矩形的逻辑宽度和高度
	/* 3.pSrcDC表示源设备CDC指针
	/* 4.xSrc、ySrc表示位图源矩形的左上角的x、y逻辑坐标值
	/* 5.dwRop表示显示位图的光栅操作方式 SRCCOPY用于直接将位图复制到目标环境中
	/*******************************************************************************/

	dcBmp.SelectObject(pbmpOld);           //恢复临时DC的位图  
	DeleteObject(&m_bitmap);               //删除内存中的位图  
	dcBmp.DeleteDC();                      //删除CreateCompatibleDC得到的图片DC  


											/**
											* 面代码为后面显示第二张图片
											*/
	if (numPicture == 2) {
		//显示图片函数LoadImage  
		HBITMAP m_hBitmapChange=NULL;
		if (level == 0) //显示2张图 BmpNameLin原图  
		{
			m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		}
		else
			if (level == 1) //灰度图片 BmpNameLin临时图片  
			{
				m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpNameLin, IMAGE_BITMAP, 0, 0,
					LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
			}
		if (m_bitmap.m_hObject) {
			m_bitmap.Detach();            //m_bitmap为创建的位图对象  
		}
		m_bitmap.Attach(m_hBitmapChange);
		//定义并创建一个内存设备环境  
		CDC dcBmp;
		if (!dcBmp.CreateCompatibleDC(pDC))   //创建兼容性的DC  
			return;
		BITMAP m_bmp;                          //临时bmp图片变量  
		m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中  
		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境  

											   //如果图片太大显示大小为固定640*640 否则显示原图大小  
		if (m_nDrawWidth<650 && m_nDrawHeight<650)
			pDC->StretchBlt(m_nWindowWidth - m_nDrawWidth, 0,
				m_nDrawWidth, m_nDrawHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
		else
			pDC->StretchBlt(m_nWindowWidth - 640, 0, 640, 640, &dcBmp, 0, 0,
				m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
		//恢复临时DC的位图  
		dcBmp.SelectObject(pbmpOld);
	}
	
}

bool CpainterView::ReadBmp()
{
	//图片读出存储其中的东西  
	FILE *fp = fopen(BmpName, "rb");
	if (fp == 0)
	{
		AfxMessageBox("无法打开文件!", MB_OK, 0);
		return 0;
	}
	//读取文件头 解决BMP格式倒置的方法  
	fread(&bfh.bfType, sizeof(WORD), 1, fp);
	fread(&bfh.bfSize, sizeof(DWORD), 1, fp);
	fread(&bfh.bfReserved1, sizeof(WORD), 1, fp);
	fread(&bfh.bfReserved2, sizeof(WORD), 1, fp);
	fread(&bfh.bfOffBits, sizeof(DWORD), 1, fp);
	//图像文件的总字节数  
	m_nSize = bfh.bfSize;
	//判断是否是bmp格式图片  
	if (bfh.bfType != 0x4d42)   //'BM'  
	{
		AfxMessageBox("不是BMP格式图片!", MB_OK, 0);
		return 0;
	}
	//读取信息头  
	fread(&bih.biSize, sizeof(DWORD), 1, fp);
	fread(&bih.biWidth, sizeof(LONG), 1, fp);
	fread(&bih.biHeight, sizeof(LONG), 1, fp);
	fread(&bih.biPlanes, sizeof(WORD), 1, fp);
	fread(&bih.biBitCount, sizeof(WORD), 1, fp);
	fread(&bih.biCompression, sizeof(DWORD), 1, fp);
	fread(&bih.biSizeImage, sizeof(DWORD), 1, fp);
	fread(&bih.biXPelsPerMeter, sizeof(LONG), 1, fp);
	fread(&bih.biYPelsPerMeter, sizeof(LONG), 1, fp);
	fread(&bih.biClrUsed, sizeof(DWORD), 1, fp);
	fread(&bih.biClrImportant, sizeof(DWORD), 1, fp);
	if (bih.biSize != sizeof(bih))
	{
		AfxMessageBox("本结构所占用字节数出现错误");
		return 0;
	}
	//位图压缩类型，必须是 0（不压缩） 1（BI_RLE8压缩类型）或2（BI_RLE压缩类型）之一  
	if (bih.biCompression == BI_RLE8 || bih.biCompression == BI_RLE4)
	{
		AfxMessageBox("位图被压缩!");
		return 0;
	}
	//获取图像高宽和每个像素所占位数  
	m_nHeight = bih.biHeight;
	m_nWidth = bih.biWidth;
	m_nDrawHeight = bih.biHeight;
	m_nDrawWidth = bih.biWidth;
	m_nBitCount = bih.biBitCount;   //每个像素所占位数  
									//计算图像每行像素所占的字节数（必须是32的倍数）  
	m_nLineByte = (m_nWidth*m_nBitCount + 31) / 32 * 4;
	//图片大小 调用系统自带的文件头 BITMAPFILEHEADER bfh; BITMAPINFOHEADER bih;   
	//否则用 BITMAPFILEHEADER_ bfh; BITMAPINFOHEADER_ bih;要 m_nImage = m_nLineByte * m_nHeight - 2;  
	m_nImage = m_nLineByte * m_nHeight;
	//位图实际使用的颜色表中的颜色数 biClrUsed  
	m_nPalette = 0;                       //初始化  
	if (bih.biClrUsed)
		m_nPalette = bih.biClrUsed;
	//申请位图空间 大小为位图大小 m_nImage  
	//malloc只能申请4字节的空间 （未知）  
	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fp);
	fclose(fp);
	return true;
}

bool CpainterView::SaveBmp(LPCSTR lpFileName)
{
	//保存bmp格式图片 写图片过程 只处理24像素的图片 该图片无调色板  
	FILE *fpo = fopen(BmpNameLin, "rb");
	FILE *fpw = fopen(lpFileName, "wb");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	//malloc只能申请4字节的空间 （未知）  
	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fpo);
	fwrite(m_pImage, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	return true;
}

CpainterView::~CpainterView()
{
}

BOOL CpainterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CpainterView 绘制

void CpainterView::OnDraw(CDC* pDC)
{
	CpainterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (EntName.Compare("bmp") == 0)      //bmp格式    
	{
		ReadBmp();
		ShowBitmap(pDC, BmpName);               //显示图片    
	}
	// TODO: 在此处为本机数据添加绘制代码
}


// CpainterView 打印

BOOL CpainterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CpainterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CpainterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CpainterView 诊断

#ifdef _DEBUG
void CpainterView::AssertValid() const
{
	CView::AssertValid();
}

void CpainterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CpainterDoc* CpainterView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CpainterDoc)));
	return (CpainterDoc*)m_pDocument;
}
#endif //_DEBUG


// CpainterView 消息处理程序


void CpainterView::OnFileOpen()
{
	//两种格式的文件：bmp gif  
	CString filter;
	filter = "所有文件(*.bmp)|*.bmp;| BMP(*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//按下确定按钮 dlg.DoModal() 函数显示对话框    
	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();     //获取文件路径名   如D:\pic\abc.bmp    
		BmpNameLin = "picture.bmp";      //临时变量名-----  
		numPicture = 1;                    //显示一张图片 ------ 
		EntName = dlg.GetFileExt();      //获取文件扩展名    
		EntName.MakeLower();             //将文件扩展名转换为一个小写字符    
		Invalidate();                    //调用该函数就会调用OnDraw重绘画图    
	}
}


void CpainterView::OnFileSave()
{
	// TODO: Add your command handler code here  
	CString filter;
	filter = "所有文件(*.bmp)|*.bmp;| BMP(*.bmp)|*.bmp||";
	//重点: 1-文件打开 0-文件保存  
	CFileDialog dlg(0, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	//按下确定按钮  
	if (dlg.DoModal() == IDOK) {
		CString str;
		CString strName;
		CString filename;
		str = dlg.GetPathName();           //获取文件的路径  
		filename = dlg.GetFileTitle();     //获取文件名  
		int nFilterIndex = dlg.m_ofn.nFilterIndex;
		if (nFilterIndex == 2)            //当用户选择文件过滤器为".BMP"时  
		{
			str = str + ".bmp";            //自动加扩展名.bmp  
			SaveBmp(str);                  //保存bmp图片 就是一个写出图片的过程   
			AfxMessageBox("图片保存成功", MB_OK, 0);
		}
	}
}


void CpainterView::OnShowTwo()
{
	//如果没有导入图片直接点击双显 提示错误  
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能显示2张图片!");
		return;
	}
	AfxMessageBox("显示两张图片!", MB_OK, 0);
	numPicture = 2;    //全局变量 显示两图  
	level = 0;          //level=0双显  
	Invalidate();      //调用Invalidate 每秒调用一次OnDraw画图  
}




void CpainterView::OnNext()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能下一步细胞上色!", MB_OK, 0);
		return;
	}
	AfxMessageBox("细胞上色!", MB_OK, 0);
	//打开临时的图片
	if (next == 0)
	{
		FILE *fpo = fopen(BmpNameLin, "rb");
		FILE *fpw = fopen(BmpName, "wb+");
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
		fread(m_pImage, m_nImage, 1, fpo);
		next++;
		unsigned char fred, fgreen, fblue;
		int number = 0;
		int first = 0;
		int RED = Random(255), GREEN = Random(255), BLUE = Random(255);
		for (int i = 0; i < m_nImage; i = i + 3)
		{
			fred = m_pImage[i];
			fgreen = m_pImage[i + 1];
			fblue = m_pImage[i + 2];
			if (fred == 255 && fgreen == 255 && fblue == 255 && first == 0)
			{

				m_pImage[i] = RED;
				m_pImage[i + 1] = GREEN;
				m_pImage[i + 2] = BLUE;
				first = 1;
				number++;
			}
			if (m_pImage[i] == RED && m_pImage[i + 1] == GREEN && m_pImage[i + 2] == BLUE)
			{
				int x1 = i + 3, x2 = i - 3, y1 = i + m_nLineByte, y2 = i - m_nLineByte;
				if (m_pImage[x1] == 255 && m_pImage[x1 + 1] == 255 && m_pImage[x1 + 2] == 255)
				{
					m_pImage[x1] = RED;
					m_pImage[x1 + 1] = GREEN;
					m_pImage[x1 + 2] = BLUE;
					number++;
				}
				if (m_pImage[x2] == 255 && m_pImage[x2 + 1] == 255 && m_pImage[x2 + 2] == 255)
				{
					m_pImage[x2] = RED;
					m_pImage[x2 + 1] = GREEN;
					m_pImage[x2 + 2] = BLUE;
					i = i - 6;
					number++;
				}
				if (m_pImage[y1] == 255 && m_pImage[y1 + 1] == 255 && m_pImage[y1 + 2] == 255)
				{
					m_pImage[y1] = RED;
					m_pImage[y1 + 1] = GREEN;
					m_pImage[y1 + 2] = BLUE;
					number++;
				}
				if (m_pImage[y2] == 255 && m_pImage[y2 + 1] == 255 && m_pImage[y2 + 2] == 255)
				{
					m_pImage[y2] = RED;
					m_pImage[y2 + 1] = GREEN;
					m_pImage[y2 + 2] = BLUE;
					number++;
				}

			}
		}
		if (number<300)
			for (int i = 0; i < m_nImage; i = i + 3)
			{
				fred = m_pImage[i];
				fgreen = m_pImage[i + 1];
				fblue = m_pImage[i + 2];
				if (fred == RED && fgreen == GREEN && fblue == BLUE)
				{

					m_pImage[i] = 0;
					m_pImage[i + 1] = 0;
					m_pImage[i + 2] = 0;
				}
			}
		
		fwrite(m_pImage, m_nImage, 1, fpw);
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level = 1;
		Invalidate();
	}
	else if(next==1)
	{
		FILE *fpo = fopen(BmpName, "rb");
		FILE *fpw = fopen(BmpNameLin, "wb+");
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
		fread(m_pImage, m_nImage, 1, fpo);
		next--;
		unsigned char fred, fgreen, fblue;

		int number = 0;	
		int first = 0;
		int RED = Random(255), GREEN = Random(255), BLUE = Random(255);
		for (int i = 0; i < m_nImage; i = i + 3)
		{
			fred = m_pImage[i];
			fgreen = m_pImage[i + 1];
			fblue = m_pImage[i + 2];

			if (fred == 255 && fgreen == 255 && fblue == 255 && first == 0)
			{

				m_pImage[i] = RED;
				m_pImage[i + 1] = GREEN;
				m_pImage[i + 2] = BLUE;
				first = 1;
				number++;
			}
			if (m_pImage[i] == RED && m_pImage[i + 1] == GREEN && m_pImage[i + 2] == BLUE)
			{
				int x1 = i + 3, x2 = i - 3, y1 = i + m_nLineByte, y2 = i - m_nLineByte;
				if (m_pImage[x1] == 255 && m_pImage[x1 + 1] == 255 && m_pImage[x1 + 2] == 255)
				{
					m_pImage[x1] = RED;
					m_pImage[x1 + 1] = GREEN;
					m_pImage[x1 + 2] = BLUE;
					number++;
				}
				if (m_pImage[x2] == 255 && m_pImage[x2 + 1] == 255 && m_pImage[x2 + 2] == 255)
				{
					m_pImage[x2] = RED;
					m_pImage[x2 + 1] = GREEN;
					m_pImage[x2 + 2] = BLUE;
					i = i - 6;
					number++;
				}
				if (m_pImage[y1] == 255 && m_pImage[y1 + 1] == 255 && m_pImage[y1 + 2] == 255)
				{
					m_pImage[y1] = RED;
					m_pImage[y1 + 1] = GREEN;
					m_pImage[y1 + 2] = BLUE;
					number++;
				}
				if (m_pImage[y2] == 255 && m_pImage[y2 + 1] == 255 && m_pImage[y2 + 2] == 255)
				{
					m_pImage[y2] = RED;
					m_pImage[y2 + 1] = GREEN;
					m_pImage[y2 + 2] = BLUE;
					number++;
				}

			}
		}
		if (number<300)
			for (int i = 0; i < m_nImage; i = i + 3)
			{
				fred = m_pImage[i];
				fgreen = m_pImage[i + 1];
				fblue = m_pImage[i + 2];
				if (fred == RED && fgreen == GREEN && fblue == BLUE)
				{

					m_pImage[i] = 0;
					m_pImage[i + 1] = 0;
					m_pImage[i + 2] = 0;
				}
			}
		fwrite(m_pImage, m_nImage, 1, fpw);
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level = 1;
		Invalidate();
	}
	
	
	

}


void CpainterView::OnStart()
{
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能细胞上色!", MB_OK, 0);
		return;
	}
	AfxMessageBox("细胞上色!", MB_OK, 0);
	//打开临时的图片  
	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	//读取文件  
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	unsigned char fred, fgreen, fblue;
	fread(m_pImage, m_nImage, 1, fpo);
	int white=1;
	int count=0;
	for(white;white>0;white--)
	{
		int first = 0;
		int number = 0;
		srand((int)time(0));
		int RED = (rand() % 255), GREEN = (rand() % 255), BLUE = (rand() % 255);
		for(int k=count;k>0;k--)
			RED = (rand() % 255), GREEN = (rand() % 255), BLUE = (rand() % 255);
		for (int i = 0; i < m_nImage; i = i + 3)
		{
			fred = m_pImage[i];
			fgreen = m_pImage[i + 1];
			fblue = m_pImage[i + 2];
			
			if (fred == 255 && fgreen == 255 && fblue == 255 && first == 0)
			{
				count++;
				white++;
				m_pImage[i] = RED;
				m_pImage[i + 1] = GREEN;
				m_pImage[i + 2] = BLUE;
				first = 1;
			}
			if (m_pImage[i] == RED && m_pImage[i + 1] == GREEN && m_pImage[i + 2] == BLUE)
			{
				int x1 = i + 3, x2 = i - 3, y1 = i + m_nLineByte, y2 = i - m_nLineByte;
				if (m_pImage[x1] == 255 && m_pImage[x1 + 1] == 255 && m_pImage[x1 + 2] == 255)
				{
					m_pImage[x1] = RED;
					m_pImage[x1 + 1] = GREEN;
					m_pImage[x1 + 2] = BLUE;
					number++;
				}
				if (m_pImage[x2] == 255 && m_pImage[x2 + 1] == 255 && m_pImage[x2 + 2] == 255)
				{
					m_pImage[x2] = RED;
					m_pImage[x2 + 1] = GREEN;
					m_pImage[x2 + 2] = BLUE;
					i = i - 6;
					number++;
				}
				if (m_pImage[y1] == 255 && m_pImage[y1 + 1] == 255 && m_pImage[y1 + 2] == 255)
				{
					m_pImage[y1] = RED;
					m_pImage[y1 + 1] = GREEN;
					m_pImage[y1 + 2] = BLUE;
					number++;
				}
				if (m_pImage[y2] == 255 && m_pImage[y2 + 1] == 255 && m_pImage[y2 + 2] == 255)
				{
					m_pImage[y2] = RED;
					m_pImage[y2 + 1] = GREEN;
					m_pImage[y2 + 2] = BLUE;
					number++;
				}
			}
		}
		if (number<300)
			for (int i = 0; i < m_nImage; i = i + 3)
			{
				fred = m_pImage[i];
				fgreen = m_pImage[i + 1];
				fblue = m_pImage[i + 2];
				if (fred == RED && fgreen == GREEN && fblue == BLUE)
				{

					m_pImage[i] = 0;
					m_pImage[i + 1] = 0;
					m_pImage[i + 2] = 0;
				}
			}
	}
	
	fwrite(m_pImage, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level = 1;
	Invalidate();

}
