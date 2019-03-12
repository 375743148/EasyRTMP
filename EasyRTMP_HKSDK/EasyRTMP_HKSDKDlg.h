// DecCallBack_DemoDlg.h : header file
//

#if !defined(AFX_DECCALLBACK_DEMODLG_H__ECDDE6AD_247D_4895_B55A_F6BD7B32CB1D__INCLUDED_)
#define AFX_DECCALLBACK_DEMODLG_H__ECDDE6AD_247D_4895_B55A_F6BD7B32CB1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HCNetSDK.h"
#include "PlayM4.h"

#include <map>
#include "process.h"
#include <queue>
#include "afxcmn.h"
using namespace std;

#include "EasyRTMPAPI.h"
//#include "trace.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#define KEY "79397037795969576B5A754144474A636F35337A4A65354659584E35556C524E55463947535578464C6D56345A56634D5671442F70654E4659584E355247467964326C755647566862556C7A5647686C516D567A644541794D4445345A57467A65513D3D"
#else //linux
#include "unistd.h"
#include <signal.h>
#define KEY "79397037795A4F576B596F41753242636F35394570664A6C59584E35636E52746346396D6157786C567778576F502B6C3430566863336C4559584A33615735555A57467453584E55614756435A584E30514449774D54686C59584E35"
#endif


#define MAX_FRAME_LENTH 64
#define MAX_PACK_SIZE 5120
#define MAX_DEV 1
#define MAX_QUEUE_FRAMECOUNT 1000

typedef struct tagCacheBuffder
{
	BYTE* pCacheBuffer;
	int	  nCacheBufLenth;//��������Ϊ5120
	long  lTimeStamp;
	DWORD  dwFrameLenth;

}CacheBuffder;

typedef struct tagFrameInfo
{
	tagFrameInfo() : pDataBuffer(NULL)
		, nBufSize(0), nTimestamp(0),bIsVideo(FALSE),nID(-1)
	{
	}
	tagFrameInfo& operator=(const tagFrameInfo& src)
	{
		pDataBuffer	 = src.pDataBuffer;
		nBufSize     = src.nBufSize;
		nTimestamp   = src.nTimestamp;
		bIsVideo	=	src.bIsVideo;
		nID			=	src.nID;
	}
	//�ṹ������Ϣ����
	unsigned char* pDataBuffer;
	int nBufSize;
	long nTimestamp;//ʱ���
	bool bIsVideo;//��ʶ�Ƿ�Ϊ��Ƶ
	bool bKeyFrame;//��ʶ�Ƿ�Ϊ�ؼ�֡(����Ƶ��Ч)
	int nID;//�ļ�ID
}FrameInfo;

typedef std::queue<FrameInfo*> QueueFrame;


/////////////////////////////////////////////////////////////////////////////
// CDecCallBack_DemoDlg dialog

class CDecCallBack_DemoDlg : public CDialog
{
// Construction
public:
	CDecCallBack_DemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDecCallBack_DemoDlg)
	enum { IDD = IDD_DECCALLBACK_DEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDecCallBack_DemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDecCallBack_DemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONLogin();
	virtual void OnCancel();
	afx_msg void OnButtonCapture();
	afx_msg void OnBnClickedBtnLink();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	LONG m_lUserID[MAX_DEV];
	LONG m_llRealHandle[MAX_DEV];
	CComboBox n_Channel;
	NET_DVR_DEVICECFG m_struCfg;
	CStatic m_picplay;
	CBitmap m_cBackGround;
	CBitmap	m_cOverlay;
	short iPChannel;
	CIPAddressCtrl m_ctrlDeviceIP;
	CString m_csUserName;
	CString m_csPassword;
	short   m_nLoginPort;
	HWND m_hPlayWnd[MAX_DEV];

	// д��ʱ�ļ� [5/4/2014-15:44:44 Dingshuai]
	CFile m_file;
	int m_nfIndex;
	int m_nCIndex;
	BOOL m_sendrtmp;
	CString m_strUrl;

	CacheBuffder m_pFrameCache[MAX_DEV][MAX_FRAME_LENTH];//MAX_FRAME_LENTH
	int m_mFrameCacheLenth[MAX_DEV];

	bool m_rtmpresize;
	//��������
	bool m_brtmpcapdata;
	CRITICAL_SECTION m_rtmpcs;
	HANDLE m_hHandleRtmpThread;
	QueueFrame m_queuertmpframe;
	HANDLE m_WriteFileAndEncThrdHandle;

	BOOL m_bwritevideoinfo[MAX_DEV+1];
//	BOOL m_bwriteAudioinfo;
	BOOL m_bUseGPAC;
	BOOL m_bRtmpRunning;
	BOOL m_bInitAudio;
	BOOL m_bWriting;
	BOOL m_bChange;

	QueueFrame m_queueframe;
	CRITICAL_SECTION m_cs;
	
	int m_nKeyFCount;
	long m_lCurTimeStamp;

	//¼�Ʊ�־����
	int m_nRecordDevFlag;
	//��һ�ε�¼�Ʊ�־
	int m_nPreRecordDevFlag;
	//�л���Ƶ���ڱ�־
	BOOL m_bChangeWtKeyf;
	//������Ƶ֡��������
	int m_nCountQueueVideoFrames;
	int m_nMaxQueueVideoFrames;
	//��Ƶ·��
	int m_nChangeId;
	//int MAX_DEV;
	Easy_RTMP_Handle m_RtmpHandle;

public:
	//��PS����ȡH264����
	//����ֵ���Ƿ�Ϊ���ݰ� 
	BOOL GetH246FromPS(IN BYTE* pBuffer, IN int nBufLenth, BYTE** pH264, int& nH264Lenth, BOOL& bVideo);
	static int CALLBACK RealDataCallbackFuncEx(int lRealHandle, BYTE *pBuffer,DWORD dwBufSize, DWORD dwDataType, void* lParam, void* dwUser);
	void ReceiveRealData(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pDataParam);
	bool LinkRtmp();
	void CloseRtmp();
	//H264����д��д�ļ��������
	int WriteH264DataToChace(int nDevId, BYTE* pBuffer, int nBufSize, BOOL bIsKeyFrame, long lTimeStamp);
	//д�����ݵ�����
	int WriteFilePushData(BYTE* pBuffer, int nBufSize, int nId, BOOL bVideo, BOOL bKeyFrame, long lTimeStamp);

public:
	afx_msg void OnBnClickedCheckWriting();
	afx_msg void OnBnClickedBtnChange();
	afx_msg void OnBnClickedCheckDecord();
	BOOL m_bDecord;
	afx_msg void OnBnClickedCheckFilmmode();
	BOOL m_bFilmMode;
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECCALLBACK_DEMODLG_H__ECDDE6AD_247D_4895_B55A_F6BD7B32CB1D__INCLUDED_)
