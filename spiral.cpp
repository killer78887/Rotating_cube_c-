#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cmath>
#include <algorithm>

void display(char *output, const int size, const int width,float& a) {
    printf("\x1b[H");    //clears console and sets the cursor at top left
    for (int i = 0; i < size; i++) {
        if (i%width==0){ //prints newline char(10)
            putchar(10);
        }
        putchar(output[i]);
    }
    a += 0.09;    //updating rotation angle
}

void spiral(char*output,const int width,const int height,float a,const int size){
    std::fill_n(output,size,' ');
    float pi = 3.14159265;
    float b = 8; //size
    int offsetX = width/2 -1;
    int offsetY = height/2-1;
    float alpha = 1*pi;
    float radian = 180/pi;
    for(float theta=0; theta<alpha; theta+=0.0001){
        float r = pow(theta,b);                 //gives the distance from the centre to a point in spiral hence r
        float angle = theta*(radian)-a;         //converting theta(radian) in angle (subtracting a rotates the spiral)
        int x = cos(angle)*r +offsetX;          //calculating x and y of that point
        int y = sin(angle)*r +offsetY;
        int pixel = y*width+x;
        if(pixel<size &&  pixel>=0 && x<width && x>=0){   //to prevent printing outside screen ans preventing artifacts and segmentation fault
            output[pixel] = '.';
        }
    }
}
int main() {
    int screenWidth = 180;
    int screenHeight = 40;
    int totalPixel = screenWidth * screenHeight;
    float a = 0;
    char output[totalPixel];
    std::fill_n(output,totalPixel,' ');
    printf("\x1b[H");
    while(true){
        spiral(output,screenWidth,screenHeight,a,totalPixel);
        display(output,totalPixel,screenWidth,a);
        usleep(10000);
    }
    return 0;
}