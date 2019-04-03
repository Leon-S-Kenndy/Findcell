
// painterView.h : CpainterView ��Ľӿ�
//

#pragma once


class CpainterView : public CView
{
protected: // �������л�����
	CpainterView();
	DECLARE_DYNCREATE(CpainterView)

// ����
public:
	CpainterDoc* GetDocument() const;
 
public:
	//��ӳ�Ա����  
	void ShowBitmap(CDC* pDC, CString BmpName); //��ʾλͼ����  
	bool ReadBmp();                            //������ȡbmp���ֻ�ͼƬ  
	bool SaveBmp(LPCSTR lpFileName);           //��������bmp��ʽͼƬ  

											   //��ӳ�Ա����  
	CString EntName;     //ͼ���ļ���չ��  
	CString BmpName;     //ͼ���ļ�����  
	CBitmap m_bitmap;    //����λͼ����  

	CBitmap m_bitmaplin;   //������ʱλͼ������д���---  
	CString BmpNameLin;    //����ͼ�񸱱��ļ�---  
	CBitmap m_bitmaplin1;   //������ʱλͼ������д���---  
	CString BmpNameLin1;    //����ͼ�񸱱��ļ�---  
	int next = 1;

	int m_nWidth;       //ͼ��ʵ�ʿ��  
	int m_nHeight;      //ͼ��ʵ�ʸ߶�  
	int m_nDrawWidth;   //ͼ����ʾ���  
	int m_nDrawHeight;  //ͼ����ʾ�߶�  
	DWORD m_nImage;     //ͼ�����ݵ��ֽ��� ֻ��λͼ  
	DWORD m_nSize;      //ͼ���ļ���С  
	int m_nLineByte;    //ͼ��һ����ռ�ֽ���  
	int m_nBitCount;    //ͼ��ÿ��������ռλ��  
	int m_nPalette;     //λͼʵ��ʹ�õ���ɫ���е���ɫ��  

	BYTE *m_pImage;         //����ͼƬ���ݺ��ָ��  
	BITMAPFILEHEADER bfh;   //ȫ�ֱ����ļ�ͷ  
	BITMAPINFOHEADER bih;   //ȫ�ֱ�����Ϣͷ  
	RGBQUAD m_pPal;         //��ɫ��ָ��  
// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CpainterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();  //����򿪰�ť
	afx_msg void OnFileSave();	//������水ť
	afx_msg void OnShowTwo();   //�����ʾ����ͼƬ
	afx_msg void OnNext();      //����ֶ���ɫ
	afx_msg void OnStart();     //����Զ���ɫ
};

#ifndef _DEBUG  // painterView.cpp �еĵ��԰汾
inline CpainterDoc* CpainterView::GetDocument() const
   { return reinterpret_cast<CpainterDoc*>(m_pDocument); }
#endif

