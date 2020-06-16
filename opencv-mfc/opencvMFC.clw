; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=COpencvMFCDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "opencvMFC.h"

ClassCount=3
Class1=COpencvMFCApp
Class2=COpencvMFCDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_OPENCVMFC_DIALOG

[CLS:COpencvMFCApp]
Type=0
HeaderFile=opencvMFC.h
ImplementationFile=opencvMFC.cpp
Filter=N

[CLS:COpencvMFCDlg]
Type=0
HeaderFile=opencvMFCDlg.h
ImplementationFile=opencvMFCDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_DISPLAY

[CLS:CAboutDlg]
Type=0
HeaderFile=opencvMFCDlg.h
ImplementationFile=opencvMFCDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_OPENCVMFC_DIALOG]
Type=1
Class=COpencvMFCDlg
ControlCount=2
Control1=IDC_DISPLAY,button,1342242816
Control2=IDC_SCREEN,static,1342177287

