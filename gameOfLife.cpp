#include <iostream>
#include <unistd.h>
#include <vector>

void display(char *output, const int width, const int height) {
    printf("\033[2J\033[1;1H");
    printf("\x1b[H");
    for (int i = 0; i <height; i++) {
        for (int n = 0; n<width ; n++){
            putchar(output[i*width+n]);
        }
        putchar(10);
    }
    //std::cout << "************" << "\n";
}
void update (short *config,const int size, const int width, const int height){
    std::cout << std::endl;
    short liveNeighbour = 0;
    int onCell;
    std::vector<short> toAlive;
    std::vector<short> toKill;
    for (int i = 0; i<height; i++){
        for (int n=0; n<width; n++){
            onCell = (i*width)+n;
            liveNeighbour = 0;
            //calculation of live cells
            if (onCell>=width && onCell%width!=0){               //top left corner check
                liveNeighbour += config[(onCell-width)-1];
            }
            if (onCell>=width && (onCell+1)%width!=0){           //top right corner check
                liveNeighbour += config[(onCell-width)+1];
            }
            if (onCell>=width){                                  //top edge check
                liveNeighbour += config[onCell-width];
            }
            if (onCell%width!=0){                               //left edge check
                liveNeighbour += config[onCell-1];
            }
            if ((onCell+1)%width!=0){                            //right edge check
                liveNeighbour += config[onCell+1];
            }
            if (onCell<size-width && onCell%width!=0){          //bottum left corner check
                liveNeighbour += config[(onCell+width)-1];
            }
            if (onCell<size-width && (onCell+1)%width!=0){          //bottum right corner check
                liveNeighbour += config[onCell+width+1];
            }
            if (onCell<size-width){                             //bottum edge check
                liveNeighbour += config[onCell+width];
            }
            
            /*Any live cell with <2 live neighbours dies -- underpopulation.
            Any live cell with 2 or 3 live neighbours lives -- survive
            Any live cell with >3 live neighbours dies -- by overpopulation.
            Any dead cell with exactly 3 live neighbours becomes a live cell -- reproduction.*/
            
            if (config[onCell]==1 && (liveNeighbour < 2 || liveNeighbour>3)){   //over and underpopulation
                toKill.push_back(onCell);
            }
            if (config[onCell]==0 && liveNeighbour==3){                         //reproduction
                toAlive.push_back(onCell);
            }
            
            /*
            std::cout << liveNeighbour;
            if ((onCell+1)%width==0){
                std::cout << std::endl;
            }
            */
        }
    }
    for (int element:toKill){                      //updating at end cuz itrating in between loop causes issues
        config[element] = 0;
    }
    for (int element:toAlive){                      //updating at end cuz itrating in between loop causes issues
        config[element] = 1;
    }
}
int main() {
    const int width = 100;
    const int height = 20;
    const int size = (width * height);
    char output[size];
    short config[size]{};
    srand(time(nullptr));
    
    for (int r= 0; r<500; r++){
        int ranNum = rand()%size;
        config[ranNum] = 1;
    }
    
    std::string garbage;
    config[(size-width)] = 1;
    for(int i=0; i>-1; i++){
        
        for (int n=0; n<size;n++){
            if (config[n] == 1){
                output[n] = '#';
            }
            else{
                output[n] = '`';
            }
        }
        //std::getline(std::cin,garbage);           //wait for user input to render next frame
        usleep(0.1*1000000);                        //wait 0.5 sec
        display(output,width,height);
        std::cout <<"gen - " << i << "\n";
        update(config,size,width,height);
    }
    std::cout << "finished";
    return 0;
}