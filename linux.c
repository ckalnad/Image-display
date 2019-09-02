#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
Display			*Monitor;
Window			ImageWindow;
GC				ImageGC;
XWindowAttributes		Atts;
XImage			*Picture;
int				ROWS,COLS;
unsigned char		*displaydata;
FILE            *fpt;
unsigned char   *image;
char            header[80];
int             BYTES;
int             i,j=0;
unsigned char	uu=0,vv=0,ww=0,xx=0,aa=0,bb=0,cc=0,dd=0,yy=0;

int as,qw,q,t;
unsigned char LSB,MSB;


 
fpt=fopen(argv[1],"r");
if (fpt == NULL)
  {
  printf("File could not be opened\n");
  exit(0);
  }
        
i=fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);

fgetc(fpt);


if (strcmp(header,"P5") == 0)
	{
		image=(unsigned char *)calloc(2*ROWS*COLS,sizeof(unsigned char));
		if (image == NULL)
  		{
  		printf("Memory not allocated");
  		exit(0);
  		}

		displaydata=(unsigned char *)calloc(2*ROWS*COLS,sizeof(unsigned char));
		fread(image,1,ROWS*COLS,fpt);
		fclose(fpt);


		printf("Rows is equal to %d\n",ROWS);
		printf("Columns is equal to %d\n",COLS);

		for(i=0;i<ROWS*COLS;i++)
  			{
  				aa=(image[i]>>3);
				bb=(image[i]>>2);

				uu=aa & 31;
				vv=bb & 63;

				cc=(uu<<3);
				dd=(vv>>3);
				LSB=cc+dd;

				MSB=(vv<<5) + (uu);

				displaydata[j]=MSB;
				displaydata[j+1]=LSB;
			
				//printf("Image[%d]=%d\n",i,image[i]);
				//printf("displaydata[%d]=%d\n",j,displaydata[j]);
                                //printf("displaydata[%d]=%d\n",j+1,displaydata[j+1]);

				j=j+2;

  			}

		}

else if(strcmp(header,"P6") == 0)
	{
		image=(unsigned char *)calloc(3*ROWS*COLS,sizeof(unsigned char));
		if (image == NULL)
  		{
  		printf("Unable to allocate %d x %d memory\n",COLS,ROWS);
  		exit(0);
  		}
		displaydata=(unsigned char *)calloc(2*ROWS*COLS,sizeof(unsigned char));
		fread(image,1,ROWS*COLS*3,fpt);
		fclose(fpt);


		for(i=0;i<ROWS*COLS;i++)
			{
				aa=(image[i*3]>>3);
				ww=aa & 31;
				bb=(image[(i*3)+1]>>2); 
				uu=bb & 63;
				yy=(image[(i*3)+2]>>3);
				xx=yy & 31;
		
				LSB=(uu<<5) + (xx);		
				MSB=(ww<<3) + (uu>>3);
					
				displaydata[j+1]=MSB;
				displaydata[j]=LSB;
				
				//printf("Image[%d]=%d\n",i,image[i]);
				//printf("displaydata[%d]=%d\n",j,displaydata[j]);
                                //printf("displaydata[%d]=%d\n",j+1,displaydata[j+1]);
		
				j=j+2;
			}
	}
	else
	{
  		printf("%s file is not PPM image\n",argv[1]);
  		fclose(fpt);
  		exit(0);
  	}




//printf("1\n");


/* ... */

Monitor=XOpenDisplay(NULL);
if (Monitor == NULL)
  {
  printf("Unable to open graphics display\n");
  exit(0);
  }

//printf("2\n");

ImageWindow=XCreateSimpleWindow(Monitor,RootWindow(Monitor,0),
	500,100,		/* x,y on screen */
	COLS,ROWS,	/* width, height */
	2, 		/* border width */
	BlackPixel(Monitor,0),
	WhitePixel(Monitor,0));

ImageGC=XCreateGC(Monitor,ImageWindow,0,NULL);

XMapWindow(Monitor,ImageWindow);
XFlush(Monitor);


//printf("3\n");


while(1)
  {
  XGetWindowAttributes(Monitor,ImageWindow,&Atts);
  if (Atts.map_state == IsViewable /* 2 */)
    break;
  }


//printf("4\n");
//printf("%ld\n",sizeof(unsigned char)*sizeof(displaydata));
//printf("%d\n",ROWS*COLS*2);
//printf("%ld\n",sizeof(displaydata));
Picture=XCreateImage(Monitor,DefaultVisual(Monitor,0),
		DefaultDepth(Monitor,0),
		ZPixmap,	/* format */
		0,		/* offset */
		displaydata,       /* the data */
		COLS,ROWS,	/* size of the image data */
		16,		/* pixel quantum (8, 16 or 32) */
		0);	
	/* bytes per line (0 causes compute) */


//printf("5\n");


XPutImage(Monitor,ImageWindow,ImageGC,Picture,
		0,0,0,0,	/* src x,y and dest x,y offsets */
		COLS,ROWS);	/* size of the image data */


//printf("6\n");


XFlush(Monitor);
sleep(2);
XCloseDisplay(Monitor);
}

