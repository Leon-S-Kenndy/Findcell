
// painterView.cpp : CpainterView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CpainterView ����/����

CpainterView::CpainterView()
{
	// TODO: �ڴ˴���ӹ������

}
/*************************************************************/
/* numPicture������ʾͼƬ����
/* 0-��ʾ�����δ��ͼƬ 1-��ʾһ��ͼƬ 2-��ʾ����ͼƬ�ʹ���
/*************************************************************/
int numPicture = 0;

/*************************************************************/
/* level������ʾ����Ĵ��������ÿ���������и�ֵ�ñ���
/* 0-��ʾ2��ͼƬ 1-��ʾ�Ҷ�ͼƬ 3-��ʾͼƬ����
/* 2 4 8 16 32 64-��ͬ�����ȼ�����ͼƬ
/*************************************************************/
int level = 0;
void CpainterView::ShowBitmap(CDC * pDC, CString BmpName)
{
	//����bitmapָ�� ���ú���LoadImageװ��λͼ  
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

	/*************************************************************************/
	/* 1.Ҫװ��OEMͼ������˲���ֵΪ0  OBM_ OEMλͼ OIC_OEMͼ�� OCR_OEM���
	/* 2.BmpNameҪװ��ͼƬ���ļ���
	/* 3.װ��ͼ������:
	/*   IMAGE_BITMAP-װ��λͼ IMAGE_CURSOR-װ�ع�� IMAGE_ICON-װ��ͼ��
	/* 4.ָ��ͼ���������ؿ�Ⱥͳ��� ������Ϊ��λ
	/* 5.����ѡ��:
	/*   IR_LOADFROMFILE-ָ����lpszNameָ���ļ��м���ͼ��
	/*   IR_DEFAULTSIZE-ָ��ʹ��ͼ��Ĭ�ϴ�С
	/*   LR_CREATEDIBSECTION-��uType����ΪIMAGE_BITMAPʱ,����һ��DIB��
	/**************************************************************************/

	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();           //�ж�CWnd�ʹ�����ϵ  
	}
	m_bitmap.Attach(m_hBitmap);      //�����HBITMAP m_hBitmap��CBitmap m_bitmap����  

										//�߽�  
	CRect rect;
	GetClientRect(&rect);

	//ͼƬ��ʾ(x,y)��ʼ����  
	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;   //����ͻ������  
	int m_nWindowHeight = rect.bottom - rect.top;  //����ͻ����߶�  

													//���岢����һ���ڴ��豸����DC  
	CDC dcBmp;
	if (!dcBmp.CreateCompatibleDC(pDC))   //���������Ե�DC  
		return;
	BITMAP m_bmp;                          //��ʱbmpͼƬ����  
	m_bitmap.GetBitmap(&m_bmp);            //��ͼƬ����λͼ��  
	CBitmap *pbmpOld = NULL;
	dcBmp.SelectObject(&m_bitmap);         //��λͼѡ����ʱ�ڴ��豸����  

											//ͼƬ��ʾ���ú���stretchBlt  
	pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
		m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);

	/*******************************************************************************/
	/* BOOL StretchBlt(int x,int y,int nWidth,int nHeight,CDC* pSrcDC,
	/*                 int xSrc,int ySrc,int nSrcWidth,int nSrcHeight,DWORD dwRop );
	/* 1.����x��yλͼĿ��������Ͻ�x��y������ֵ
	/* 2.nWidth��nHeigthλͼĿ����ε��߼���Ⱥ͸߶�
	/* 3.pSrcDC��ʾԴ�豸CDCָ��
	/* 4.xSrc��ySrc��ʾλͼԴ���ε����Ͻǵ�x��y�߼�����ֵ
	/* 5.dwRop��ʾ��ʾλͼ�Ĺ�դ������ʽ SRCCOPY����ֱ�ӽ�λͼ���Ƶ�Ŀ�껷����
	/*******************************************************************************/

	dcBmp.SelectObject(pbmpOld);           //�ָ���ʱDC��λͼ  
	DeleteObject(&m_bitmap);               //ɾ���ڴ��е�λͼ  
	dcBmp.DeleteDC();                      //ɾ��CreateCompatibleDC�õ���ͼƬDC  


											/**
											* �����Ϊ������ʾ�ڶ���ͼƬ
											*/
	if (numPicture == 2) {
		//��ʾͼƬ����LoadImage  
		HBITMAP m_hBitmapChange=NULL;
		if (level == 0) //��ʾ2��ͼ BmpNameLinԭͼ  
		{
			m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		}
		else
			if (level == 1) //�Ҷ�ͼƬ BmpNameLin��ʱͼƬ  
			{
				m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpNameLin, IMAGE_BITMAP, 0, 0,
					LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
			}
		if (m_bitmap.m_hObject) {
			m_bitmap.Detach();            //m_bitmapΪ������λͼ����  
		}
		m_bitmap.Attach(m_hBitmapChange);
		//���岢����һ���ڴ��豸����  
		CDC dcBmp;
		if (!dcBmp.CreateCompatibleDC(pDC))   //���������Ե�DC  
			return;
		BITMAP m_bmp;                          //��ʱbmpͼƬ����  
		m_bitmap.GetBitmap(&m_bmp);            //��ͼƬ����λͼ��  
		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);         //��λͼѡ����ʱ�ڴ��豸����  

											   //���ͼƬ̫����ʾ��СΪ�̶�640*640 ������ʾԭͼ��С  
		if (m_nDrawWidth<650 && m_nDrawHeight<650)
			pDC->StretchBlt(m_nWindowWidth - m_nDrawWidth, 0,
				m_nDrawWidth, m_nDrawHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
		else
			pDC->StretchBlt(m_nWindowWidth - 640, 0, 640, 640, &dcBmp, 0, 0,
				m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
		//�ָ���ʱDC��λͼ  
		dcBmp.SelectObject(pbmpOld);
	}
	
}

bool CpainterView::ReadBmp()
{
	//ͼƬ�����洢���еĶ���  
	FILE *fp = fopen(BmpName, "rb");
	if (fp == 0)
	{
		AfxMessageBox("�޷����ļ�!", MB_OK, 0);
		return 0;
	}
	//��ȡ�ļ�ͷ ���BMP��ʽ���õķ���  
	fread(&bfh.bfType, sizeof(WORD), 1, fp);
	fread(&bfh.bfSize, sizeof(DWORD), 1, fp);
	fread(&bfh.bfReserved1, sizeof(WORD), 1, fp);
	fread(&bfh.bfReserved2, sizeof(WORD), 1, fp);
	fread(&bfh.bfOffBits, sizeof(DWORD), 1, fp);
	//ͼ���ļ������ֽ���  
	m_nSize = bfh.bfSize;
	//�ж��Ƿ���bmp��ʽͼƬ  
	if (bfh.bfType != 0x4d42)   //'BM'  
	{
		AfxMessageBox("����BMP��ʽͼƬ!", MB_OK, 0);
		return 0;
	}
	//��ȡ��Ϣͷ  
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
		AfxMessageBox("���ṹ��ռ���ֽ������ִ���");
		return 0;
	}
	//λͼѹ�����ͣ������� 0����ѹ���� 1��BI_RLE8ѹ�����ͣ���2��BI_RLEѹ�����ͣ�֮һ  
	if (bih.biCompression == BI_RLE8 || bih.biCompression == BI_RLE4)
	{
		AfxMessageBox("λͼ��ѹ��!");
		return 0;
	}
	//��ȡͼ��߿��ÿ��������ռλ��  
	m_nHeight = bih.biHeight;
	m_nWidth = bih.biWidth;
	m_nDrawHeight = bih.biHeight;
	m_nDrawWidth = bih.biWidth;
	m_nBitCount = bih.biBitCount;   //ÿ��������ռλ��  
									//����ͼ��ÿ��������ռ���ֽ�����������32�ı�����  
	m_nLineByte = (m_nWidth*m_nBitCount + 31) / 32 * 4;
	//ͼƬ��С ����ϵͳ�Դ����ļ�ͷ BITMAPFILEHEADER bfh; BITMAPINFOHEADER bih;   
	//������ BITMAPFILEHEADER_ bfh; BITMAPINFOHEADER_ bih;Ҫ m_nImage = m_nLineByte * m_nHeight - 2;  
	m_nImage = m_nLineByte * m_nHeight;
	//λͼʵ��ʹ�õ���ɫ���е���ɫ�� biClrUsed  
	m_nPalette = 0;                       //��ʼ��  
	if (bih.biClrUsed)
		m_nPalette = bih.biClrUsed;
	//����λͼ�ռ� ��СΪλͼ��С m_nImage  
	//mallocֻ������4�ֽڵĿռ� ��δ֪��  
	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fp);
	fclose(fp);
	return true;
}

bool CpainterView::SaveBmp(LPCSTR lpFileName)
{
	//����bmp��ʽͼƬ дͼƬ���� ֻ����24���ص�ͼƬ ��ͼƬ�޵�ɫ��  
	FILE *fpo = fopen(BmpNameLin, "rb");
	FILE *fpw = fopen(lpFileName, "wb");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	//mallocֻ������4�ֽڵĿռ� ��δ֪��  
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CpainterView ����

void CpainterView::OnDraw(CDC* pDC)
{
	CpainterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (EntName.Compare("bmp") == 0)      //bmp��ʽ    
	{
		ReadBmp();
		ShowBitmap(pDC, BmpName);               //��ʾͼƬ    
	}
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CpainterView ��ӡ

BOOL CpainterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CpainterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CpainterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CpainterView ���

#ifdef _DEBUG
void CpainterView::AssertValid() const
{
	CView::AssertValid();
}

void CpainterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CpainterDoc* CpainterView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CpainterDoc)));
	return (CpainterDoc*)m_pDocument;
}
#endif //_DEBUG


// CpainterView ��Ϣ�������


void CpainterView::OnFileOpen()
{
	//���ָ�ʽ���ļ���bmp gif  
	CString filter;
	filter = "�����ļ�(*.bmp)|*.bmp;| BMP(*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//����ȷ����ť dlg.DoModal() ������ʾ�Ի���    
	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();     //��ȡ�ļ�·����   ��D:\pic\abc.bmp    
		BmpNameLin = "picture.bmp";      //��ʱ������-----  
		numPicture = 1;                    //��ʾһ��ͼƬ ------ 
		EntName = dlg.GetFileExt();      //��ȡ�ļ���չ��    
		EntName.MakeLower();             //���ļ���չ��ת��Ϊһ��Сд�ַ�    
		Invalidate();                    //���øú����ͻ����OnDraw�ػ滭ͼ    
	}
}


void CpainterView::OnFileSave()
{
	// TODO: Add your command handler code here  
	CString filter;
	filter = "�����ļ�(*.bmp)|*.bmp;| BMP(*.bmp)|*.bmp||";
	//�ص�: 1-�ļ��� 0-�ļ�����  
	CFileDialog dlg(0, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	//����ȷ����ť  
	if (dlg.DoModal() == IDOK) {
		CString str;
		CString strName;
		CString filename;
		str = dlg.GetPathName();           //��ȡ�ļ���·��  
		filename = dlg.GetFileTitle();     //��ȡ�ļ���  
		int nFilterIndex = dlg.m_ofn.nFilterIndex;
		if (nFilterIndex == 2)            //���û�ѡ���ļ�������Ϊ".BMP"ʱ  
		{
			str = str + ".bmp";            //�Զ�����չ��.bmp  
			SaveBmp(str);                  //����bmpͼƬ ����һ��д��ͼƬ�Ĺ���   
			AfxMessageBox("ͼƬ����ɹ�", MB_OK, 0);
		}
	}
}


void CpainterView::OnShowTwo()
{
	//���û�е���ͼƬֱ�ӵ��˫�� ��ʾ����  
	if (numPicture == 0)
	{
		AfxMessageBox("����ͼƬ�������ʾ2��ͼƬ!");
		return;
	}
	AfxMessageBox("��ʾ����ͼƬ!", MB_OK, 0);
	numPicture = 2;    //ȫ�ֱ��� ��ʾ��ͼ  
	level = 0;          //level=0˫��  
	Invalidate();      //����Invalidate ÿ�����һ��OnDraw��ͼ  
}




void CpainterView::OnNext()
{
	if (numPicture == 0)
	{
		AfxMessageBox("����ͼƬ�������һ��ϸ����ɫ!", MB_OK, 0);
		return;
	}
	AfxMessageBox("ϸ����ɫ!", MB_OK, 0);
	//����ʱ��ͼƬ
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
		AfxMessageBox("����ͼƬ�����ϸ����ɫ!", MB_OK, 0);
		return;
	}
	AfxMessageBox("ϸ����ɫ!", MB_OK, 0);
	//����ʱ��ͼƬ  
	FILE *fpo = fopen(BmpName, "rb");
	FILE *fpw = fopen(BmpNameLin, "wb+");
	//��ȡ�ļ�  
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
