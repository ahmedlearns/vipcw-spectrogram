/*
	davinci_display.c

	Previously called appMain.c
					  =========
*/
/*  Draw a vertical bar of a color */
        for (x = 0; x<dim.lineLength; x++) { //dim.width
        
	  for (y = 0; y < dim.height; y++) { //dim.height

		if(((y >= 112)&&(y<368)) && x<273  )
		{			

			memset(Buffer_getUserPtr(hDispBuf) + (x+20)+(y*dim.lineLength) , getY( datapoints[ (y-112) + ((x%273)*256) ] ), scale);
		      if (x % 2 ==0)
		      {

			memset(Buffer_getUserPtr(hDispBuf)+ 720*480 + (x-396)+(y+9)*dim.lineLength , getU( datapoints[ (y-112) + ((x%273)*256) ] ), scale);

		      }   
		      else
		      {
		  
			memset(Buffer_getUserPtr(hDispBuf)+ 720*480 + (x-396)+(y+9)*dim.lineLength , getV( datapoints[ (y-112) + ((x%273)*256) ] ), scale);

		      }
		} else
		{
			memset(Buffer_getUserPtr(hDispBuf) + x+(y)*dim.lineLength , 0, scale);
		}
 		 
	  }
	    
	}

/* Retreive YUV values based on the FFT point*/
int getY(float fft_point)
{
//	printf("Entered getY()\n");
  int index, Y;	
  index = (int)(fft_point*63);
   Y = yuv_colors[index][0];
    return Y;
} 

int getU(float fft_point)
{
//	printf("Entered getU()\n");
  int index,U;	
  index = (int)(fft_point*63);
    U = yuv_colors[index][1];
    return U;
} 

int getV(float fft_point)
{
//	printf("Entered getV()\n");
  int index,V;	
  index = (int)(fft_point*63);
 
  V = yuv_colors[index][2];
  //printf("%d,%d\n", index,V);
    return V;
}