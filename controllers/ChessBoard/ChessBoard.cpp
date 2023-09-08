#include <webots/Keyboard.hpp>
#include <webots/GPS.hpp>
#include <webots/InertialUnit.hpp>
//#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>

#define TIME_STEP 16

using namespace webots;
using namespace std;

void sensorUp();
void sensorDown();
void delay(float t);

Robot *robot =new Robot();    
Motor *linear_m;

double linear=0;   


int main(int argc,char **argv)
{   

    linear_m =robot->getMotor("linear_m");
       
    while (robot->step(TIME_STEP) != -1) 
    {
 
      sensorUp();delay(2000);
      sensorDown();delay(2000);
      break;
      
     }
    
 delete robot;
 return 0;  // EXIT_SUCCESS          
 }     
  
////////////////////////////////////////////////////////////////  
  
 void sensorUp()
 {  
   
   while (robot->step(TIME_STEP) != -1) 
    {

      linear+=0.005;
      cout<<"Laser Sensor :"<<linear<<endl;
      
      if (linear>=0.53)
      {
        linear_m->setVelocity(0.0);
        break;
      }  
      linear_m->setPosition(linear);
      
     }
 } 
 
  void sensorDown()
 {  
   
   while (robot->step(TIME_STEP) != -1) 
    {

      linear+= -0.005;
      cout<<"Laser Sensor :"<<linear<<endl;
      if (linear<=0.0)
      {
        linear_m->setVelocity(0.0);
        break;
      }  
      linear_m->setPosition(linear);
      
     }
 }    
 
  
  void delay(float t)
  {
    float current = robot->getTime();
    float final  = current+(t/1000);
    while (final>robot->getTime())
    {
      robot->step(1);
    }
}  

