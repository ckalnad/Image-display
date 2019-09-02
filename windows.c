// iii.cpp : Defines the entry point for the application.
//

#include "stdafx.h"


// uuu.cpp : Defines the entry point for the console application.
//




#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>



LRESULT CALLBACK EventProcessor (HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{ return(DefWindowProc(hWnd,uMsg,wParam,lParam)); }

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,int nCmdShow)
{
WNDCLASS		wc;
HWND			WindowHandle;
int			ROWS,COLS;
unsigned char	*displaydata;
BITMAPINFO		*bm_info;

HDC			hDC;

FILE *fpt;
    unsigned char *image;
    char header[80];
    int BYTES;
    int i,j,b;
	char fname[30];
	
			


    


wc.style=CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc=(WNDPROC)EventProcessor;
wc.cbClsExtra=wc.cbWndExtra=0;
wc.hInstance=hInstance;
wc.hIcon=NULL;
wc.lpszMenuName=NULL;
wc.hCursor=LoadCursor(NULL,IDC_ARROW);
wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
wc.lpszClassName=L"Image Window Class";

if (RegisterClass(&wc) == 0)
  exit(0);

if (MessageBox(NULL,L"greyscale image?",L"Greyscale", MB_YESNO) == IDYES)
		strcpy(fname,"bridge.ppm");
	else
strcpy(fname,"retina.ppm");

fpt=fopen(fname,"rb");

    /*if(fpt==NULL)
    {
        printf("Unable to open file");
        exit(0);
    }*/

    i=fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);
	header[79]=fgetc(fpt);


	if (strcmp(header,"P5")==0)
	{
		
		image=(unsigned char *)calloc(ROWS*COLS*2,sizeof(unsigned char));

	
		if(image==NULL)
		{
			printf("Memory not allocated");
			exit(0);
		}

		fread(image,1,ROWS*COLS,fpt);
		fclose(fpt);

		displaydata=(unsigned char *)calloc(ROWS*COLS*2,sizeof(unsigned char));

		for(i=0;i<ROWS*COLS;i++)
		{
			displaydata[i]=image[i];
		}
	
	}
	
	else if(strcmp(header,"P6") == 0)
		{
			
			
			image=(unsigned char *)calloc(3*ROWS*COLS,sizeof(unsigned char));
			displaydata=(unsigned char *)calloc(2*ROWS*COLS,sizeof(unsigned char));
			fread(image,1,ROWS*COLS*3,fpt);
			
			int ww=0,uu=0,xx=0,LSB=0,MSB=0,aa=0,vv=0,bb=0,cc=0,dd=0,ee=0,jj=0;

			for(i=0;i<ROWS*COLS;i++)
			{
				
				ww=(image[i*3]>>3) & 31;
				aa=ww<<2;

				uu=(image[(i*3)+1]>>3) & 31;
				vv=uu & 24;
				ww=vv>>3;
				bb=aa+ww;
				cc=uu & 7;
				dd=cc<< 5;

				xx=(image[(i*3)+2]>>3) & 31;

				ee=dd+xx;
		
				LSB=ee;		
				MSB=bb;
					
				
				displaydata[jj+1]=MSB;
				displaydata[jj]=LSB;

				jj=jj+2;
		
			
			}
			fclose(fpt);
			
	}
	else
	{
  		printf("%s file is not PPM image\n");
  		fclose(fpt);
  		exit(0);
  	}
	








WindowHandle=CreateWindow(L"Image Window Class",L"ECE468 Lab1",
						  WS_OVERLAPPEDWINDOW,
						  10,10,COLS,ROWS,
						  NULL,NULL,hInstance,NULL);
if (WindowHandle == NULL)
  {
  MessageBox(NULL,L"No window",L"Try again",MB_OK | MB_APPLMODAL);
  exit(0);
  }
ShowWindow (WindowHandle, SW_SHOWNORMAL);



bm_info=(BITMAPINFO *)calloc(1,sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD));
hDC=GetDC(WindowHandle);



/*typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;*/

if(strcmp(header,"P5")==0)
{
bm_info->bmiHeader.biBitCount=8;
}
else 
{
bm_info->bmiHeader.biBitCount=16;
}

bm_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bm_info->bmiHeader.biWidth = COLS;
	bm_info->bmiHeader.biHeight = -ROWS;
	bm_info->bmiHeader.biPlanes = 1;
	bm_info->bmiHeader.biCompression = BI_RGB;
	bm_info->bmiHeader.biSizeImage = 0;
	bm_info->bmiHeader.biClrUsed = 0;
bm_info->bmiHeader.biClrImportant = 0;

/* ... set up bmiHeader field of bm_info ... */

for (i=0; i<256; i++)	/* colormap */
  {
  bm_info->bmiColors[i].rgbBlue=bm_info->bmiColors[i].rgbGreen=bm_info->bmiColors[i].rgbRed=i;
  bm_info->bmiColors[i].rgbReserved=0;
  } 
SetDIBitsToDevice(hDC,0,0,COLS,ROWS,0,0,
			  0, /* first scan line */
			  ROWS, /* number of scan lines */
			  displaydata,bm_info,DIB_RGB_COLORS);
ReleaseDC(WindowHandle,hDC);
free(bm_info);
free(image);
free(displaydata);
MessageBox(NULL,L"Press OK to continue",L"",MB_OK | MB_APPLMODAL);
}



