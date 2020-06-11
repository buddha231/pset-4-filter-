#include "helpers.h"
#include <unistd.h>
#include<stdio.h>
#include <math.h>
#include <stdbool.h>

typedef struct{
    int rgbtRed;
    int rgbtGreen;
    int rgbtBlue;
} tempd;          //when finding the average the BYTE type restarts counting from zero when the value exceeds 255. So i have used int here;

int sobelx[3][3] =  { 
                    {1, 0, -1},
                    {2, 0, -2},
                    {1, 0, -1}
                    };
int sobely[3][3] =  { 
                    {1, 2, 1},
                    {0, 0, 0},
                    {-1,-2,-1}
                    };

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int a = 0; a<height ; a++){
        for(int b=0 ; b<width; b++){
            int temp = round((image[a][b].rgbtRed + image[a][b].rgbtGreen +image[a][b].rgbtBlue)/3);
            image[a][b].rgbtRed = temp;
            image[a][b].rgbtGreen = temp;
            image[a][b].rgbtBlue= temp;
        
        
        }

    }
    
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i<height ; i++){
        for(int j=0 ; j<width/2; j++){
            temp = image[i][j];                         
            image[i][j]= image [i] [width-j];
            image[i][width-j]=temp;
        }

    }
    return;
}


// Blur image
bool is_pixel( int x, int y, int height, int width ){     //checks if a pixel is valid
    if ((x<0 || x>=width) || (y<0 || y>=height) ) return false;
    else return true;
}
void blur(int height, int width, RGBTRIPLE image[height][width])
{   
    
    for(int l=0;l<3;l++){  //just a little tweak to adjust the level of blur. increase the constant value to increase blur level
    
        tempd tempo[height][width];           
        for (int i = 0 ; i < height ; i=i+1){
            
            for (int j = 0 ; j < width  ; j=j+1){
                
                tempo[i][j].rgbtRed=0;
                tempo[i][j].rgbtGreen=0;
                tempo[i][j].rgbtBlue=0;
                int pixelcount=0;
                    
                for(int dy=-1; dy<=1;dy++){
                    for (int dx=-1; dx<=1;dx++){
                        int pixelx = j+dx; int pixely = i+dy;
                        if (is_pixel(pixelx,pixely,height,width) ){
                            tempo[i][j].rgbtBlue = tempo[i][j].rgbtBlue +  image[pixely][pixelx].rgbtBlue; 
                            tempo[i][j].rgbtRed = tempo[i][j].rgbtRed +  image[pixely][pixelx].rgbtRed;
                            tempo[i][j].rgbtGreen = tempo[i][j].rgbtGreen +  image[pixely][pixelx].rgbtGreen; 
                            pixelcount++;

                        }
                        
                        
                        
                    }
                }    
              
                tempo[i][j].rgbtRed =   round((float)tempo[i][j].rgbtRed/pixelcount);
                tempo[i][j].rgbtGreen = round((float)tempo[i][j].rgbtGreen/pixelcount);
                tempo[i][j].rgbtBlue=   round((float)tempo[i][j].rgbtBlue/pixelcount);
            
            }
        } 
        for (int i = 0; i < height; i=i+1)
        {
            for (int j = 0; j < width; j=j+1)
            {
                        // CHECKING VALUES OF A PIXEL
                //if (i==height/2 && j== width/2)  printf(" \n%d %u\t ",tempo[i][j].rgbtGreen,image[i][j].rgbtGreen);
                //if (i==height/2 && j== width/2)  printf(" \n%d %u\t ",tempo[i][j].rgbtRed,image[i][j].rgbtRed);
                //if (i==height/2 && j== width/2)  printf(" \n%d %u\t ",tempo[i][j].rgbtBlue,image[i][j].rgbtBlue);
                
                image[i][j].rgbtBlue = tempo[i][j].rgbtBlue;
                image[i][j].rgbtGreen = tempo[i][j].rgbtGreen;
                image[i][j].rgbtRed = tempo[i][j].rgbtRed;
                            
            }
            
        }
    }
    return;

}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    tempd tempo[height][width];  
    tempd tempox, tempoy;         
        for (int i = 0 ; i < height ; i=i+1){
            
            for (int j = 0 ; j < width  ; j=j+1){
                tempox.rgbtRed=0;
                tempoy.rgbtRed=0;
                tempox.rgbtGreen=0;
                tempoy.rgbtGreen=0;
                tempox.rgbtBlue=0;
                tempoy.rgbtBlue=0;
              
                tempo[i][j].rgbtRed=0;
                tempo[i][j].rgbtGreen=0;
                tempo[i][j].rgbtBlue=0;
                int pixelcount=0;
                    
                for(int dy=-1; dy<=1;dy++){
                    for (int dx=-1; dx<=1;dx++){
                        int pixelx = j+dx; int pixely = i+dy;
                        if (is_pixel(pixelx,pixely,height,width) ){
                            tempox.rgbtBlue += sobelx[dy+1][dx+1] *  image[pixely][pixelx].rgbtBlue; 
                            tempoy.rgbtBlue += sobely[dy+1][dx+1] *  image[pixely][pixelx].rgbtBlue; 
                            
                            tempox.rgbtGreen += sobelx[dy+1][dx+1] *  image[pixely][pixelx].rgbtGreen; 
                            tempoy.rgbtGreen += sobely[dy+1][dx+1] *  image[pixely][pixelx].rgbtGreen; 
                            
                            tempox.rgbtRed += sobelx[dy+1][dx+1] *  image[pixely][pixelx].rgbtBlue; 
                            tempoy.rgbtRed += sobely[dy+1][dx+1] *  image[pixely][pixelx].rgbtRed; 
                            
                        }
                       
                        
                        
                    }
                }    
              
                tempo[i][j].rgbtBlue= sqrt(tempox.rgbtBlue * tempox.rgbtBlue + tempoy.rgbtBlue * tempoy.rgbtBlue);
                tempo[i][j].rgbtGreen= sqrt(tempox.rgbtGreen * tempox.rgbtGreen + tempoy.rgbtGreen * tempoy.rgbtGreen);
                tempo[i][j].rgbtRed= sqrt(tempox.rgbtRed * tempox.rgbtRed + tempoy.rgbtRed * tempoy.rgbtRed);
    
            }
        } 
        for (int i = 0; i < height; i=i+1)
        {
            for (int j = 0; j < width; j=j+1)
            {
                        // CHECKING VALUES OF A PIXEL
                //if (i==height/2 && j== width/2)  printf(" \n%d %u\t ",tempo[i][j].rgbtGreen,image[i][j].rgbtGreen);
                //if (i==height/2 && j== width/2)  printf(" \n%d %u\t ",tempo[i][j].rgbtRed,image[i][j].rgbtRed);
                //if (i==height/2 && j== width/2)  printf(" \n%d %u\t ",tempo[i][j].rgbtBlue,image[i][j].rgbtBlue);
                
                image[i][j].rgbtBlue =  (tempo[i][j].rgbtBlue<255)?   tempo[i][j].rgbtBlue  : 255;
                image[i][j].rgbtGreen = (tempo[i][j].rgbtGreen<=255)? tempo[i][j].rgbtGreen : 255;
                image[i][j].rgbtRed =   (tempo[i][j].rgbtRed<=255)?   tempo[i][j].rgbtRed   : 255;
                            
            }
            
        }
    return;
}
