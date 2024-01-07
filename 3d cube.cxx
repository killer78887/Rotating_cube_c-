#include <iostream>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <algorithm>
using namespace std;

void display(char *output, const int size, const int width) {
    printf("\x1b[H");
    for (int i = 0; i < size; i++) {
        if (i%width==0){ //prints newline char(10)
            putchar(10);
        }
        //output[i] = 'o';
        putchar(output[i]);
    }
}

void drawLine(const float x1,const float y1,const float x2,const float y2, char *output, int width){
    //bressanham's line alg
    float deltax = x2-x1;
    float deltay = y2-y1;
    float x = x1;
    float y = y1;
    float slope = deltay/deltax;
    float inverseslope = deltax/deltay;
    float stepx;
    float stepy;
    
    if(abs(slope)<=1){
        if(deltax>0){
            stepx = 1;
        }
        else{
            stepx= (-1);
        }
        if(deltay>=0){
            stepy=std::abs(slope);
        }
        else{
            stepy = std::abs(slope)*-1;
        }
        while((x<x2 && deltax>=0)||(x>x2 && deltax<0)){
            int pixel = ((round(y))*width+x);
            output[pixel] = '-';             //char to print on slope<1
            x += stepx;
            y += stepy;
        }
    }
    else{
        if(deltay>0){
            stepy = 1;
        }
        else{
            stepy= (-1);
        }
        if(deltax>=0){
            stepx=std::abs(inverseslope);
        }
        else{
            stepx = std::abs(inverseslope)*-1;
        }
        while((y<y2 && deltay>=0)||(y>y2 && deltay<0)){
            int pixel = ((round(y))*width+x);
            output[pixel] = '|';                //char to print on slope>1
            x += stepx;
            y += stepy;
        }
    }
}
void render(int buffer_size, float *zBuffer, char *output, int height, int width, float vertices3[8][3]) {
    std::fill_n(output,height*width,' ');

    float k = 45;          //lower = more perspectective/distortion 
    int x=0;            //for readability
    int y=1;
    int z=2;
    float thetaX = -.9 * (3.14/180);                 // rotation angles
    float thetaY = -.8 * (3.14/180);
    float thetaZ = .2 * (3.14/180);
    
    float centerX = 20;                          //cube position offsets
    float centerY = 8;
    float centerZ = 20;
    
    float originalX ;
    float originalY ;
    float originalZ ;
    for(int v=0; v<8; v++){                 //rotation calculation probably very inefficent
        for (int r=0 ; r<3 ; r++){
            originalX = vertices3[v][x];
            originalY = vertices3[v][y];
            originalZ = vertices3[v][z];
            if(r==0) {
                vertices3[v][x]=originalX;
                vertices3[v][y]=originalY*cos(thetaX)-(originalZ*sin(thetaX));
                vertices3[v][z]=originalY*sin(thetaX)+(originalZ*cos(thetaX));
            }
            else if(r==1) {
                vertices3[v][x]=originalX*cos(thetaY)+(originalZ*sin(thetaY));
                vertices3[v][y]=originalY;
                vertices3[v][z]=-(originalX*sin(thetaY))+(originalZ*cos(thetaY));
            }
            else{
                vertices3[v][x]=originalX*cos(thetaZ)-(originalY*sin(thetaZ));
                vertices3[v][y]=originalX*sin(thetaZ)+(originalY*cos(thetaZ));
                vertices3[v][z]=originalZ;
            }
            
            //cout << vertices3[v][x] << " , " << vertices3[v][y] << " , " << vertices3[v][z] << endl;
        }
    }
    
    float vertices2[8][2];
    for (int i = 0 ; i<8 ;i++){                         //projection to 2d
        vertices2[i][0] = ((vertices3[i][x]+centerX)*k)/(vertices3[i][z]+centerZ);
        vertices2[i][1] = ((vertices3[i][y]+centerY)*k)/(vertices3[i][z]+centerZ);
        output[static_cast<int>((static_cast<int>(vertices2[i][1]))*width+vertices2[i][0])] = '.';
    }
    
    //god this is a mess (drawing lines)
    drawLine(vertices2[0][0],vertices2[0][1],vertices2[1][0],vertices2[1][1],output,width);
    drawLine(vertices2[0][0],vertices2[0][1],vertices2[2][0],vertices2[2][1],output,width);
    drawLine(vertices2[0][0],vertices2[0][1],vertices2[4][0],vertices2[4][1],output,width);
    drawLine(vertices2[3][0],vertices2[3][1],vertices2[2][0],vertices2[2][1],output,width);
    drawLine(vertices2[3][0],vertices2[3][1],vertices2[1][0],vertices2[1][1],output,width);
    drawLine(vertices2[3][0],vertices2[3][1],vertices2[7][0],vertices2[7][1],output,width);
    drawLine(vertices2[6][0],vertices2[6][1],vertices2[4][0],vertices2[4][1],output,width);
    drawLine(vertices2[6][0],vertices2[6][1],vertices2[2][0],vertices2[2][1],output,width);
    drawLine(vertices2[6][0],vertices2[6][1],vertices2[7][0],vertices2[7][1],output,width);
    drawLine(vertices2[5][0],vertices2[5][1],vertices2[4][0],vertices2[4][1],output,width);
    drawLine(vertices2[5][0],vertices2[5][1],vertices2[1][0],vertices2[1][1],output,width);
    drawLine(vertices2[5][0],vertices2[5][1],vertices2[7][0],vertices2[7][1],output,width);
    
}
int main() {
    int height = 40;
    int width = 100;
    int size = height * width;
    float zBuffer[size];
    char output[size];
    float (side) = 4.5;
    float vertices3[8][3] = 
    {
        {-(side),(side),(side)}, //top1
        {(side),(side),(side)}, //top2
        {-(side),(side),-(side)}, //top3
        {(side),(side),-(side)}, //top4
        {-(side),-(side),(side)}, //bottom1
        {(side),-(side),(side)}, //bottom2
        {-(side),-(side),-(side)}, //bottom3
        {(side),-(side),-(side)} //bottom4
    };
    std::fill_n(output,size,' ');
    printf("\x1b[2J");
    for (int i ; i >-1; i++) {
        render( size, zBuffer, output, height, width, vertices3);
        display(output, size, width);
        usleep(10000); 
    }
    return 0;
}
