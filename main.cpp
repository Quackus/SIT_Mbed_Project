#include "mbed.h"
#include "ultrasonic.h"
#include "C12832.h"
//#include "LinkedList.h"
//#include "buzzer.h"

//Serial pc(USBTX,USBRX);
/*Instantiate MBED onboard LED1, App board LCD,
    lists for sensor objects and distance holder
*/
// chosen pin names correspond to connections found on Application Boards
DigitalOut led1(LED1);
C12832 lcd(p5,p7,p6,p8,p11);
//Beep buzzer(p26);
//LinkedList<node>listSens;
//LinkedList<node>listDist;

// empty method, exists for if some reason its needed
void dist(int distance)
{
    
}

// avoiding having to indivdually name them would be nice, but this language is
// bad at dynamically handling variables sigh.
// at least using the linked list will modularize the calculations

//ADDING MORE SENSORS, REQUIRES INSTANSTIATING ANOTHER ULTRASONIC, INT
//THEN ADDING TO THE LIST FOR EACH, AND INTO DISPLAY(?)
// P26 IS USED BY BUZZER
ultrasonic s1(p12,p29,.2,.5,&dist);
ultrasonic s2(p13,p28,.2,.5,&dist);
ultrasonic s3(p14,p27,.2,.5,&dist);
ultrasonic s4(p16,p25,.2,.5,&dist);
//ultrasonic s5(p16,p25,.1,1,&dist);
int d1,d2,d3,d4,d5;
double stdDev = 0;

void setup()
{
    led1 = false;
    
    s1.startUpdates();
    s2.startUpdates();
    s3.startUpdates();
    s4.startUpdates();
}

// grab values from ultrasonic value and place them into distance list
void updateDist()
{
    d1 = s1.getCurrentDistance();
    d2 = s2.getCurrentDistance();
    d3 = s3.getCurrentDistance();
    d4 = s4.getCurrentDistance();
}

// argument instead of using global variable is incase i want to include pre-adjustment
// of values in number set, such as horizontal slope correction
double calcStdDev()
{
    double mean = 0, mean2 = 0, temp = 0;
    double sum = d1 + d2 + d3 + d4;
//    int size = numSet.length();
    
    // calculate mean
    mean = sum / 4;
    
    // for each, subtract by mean, square, then calculate mean
    
    mean2 = pow(mean - d1,2) + 
            pow(mean - d2,2) + 
            pow(mean - d3,2) + 
            pow(mean - d4,2);
    mean2 = mean2 / 4;
    
    // square root 2nd mean to get standard deviation
    return sqrt(mean2);
}

// code for displaying values goes here, and or buzzer control.
void outputResult()
{
    lcd.cls();
    lcd.locate(0,3);
    lcd.printf("%d, %d, %d, %d",d1,d2,d3,d4);
    lcd.locate(0,15);
    lcd.printf("SD: %03.4f",stdDev);
    //buzzer(120 + (int)(stdDev * 300),0.1);
}

int main()
{  
    
    setup();
    while(1)
    {
        led1 = true;
        updateDist();
        stdDev = calcStdDev();
        outputResult();
        wait(0.1);
        led1 = false;
        wait(0.1);
    }
}