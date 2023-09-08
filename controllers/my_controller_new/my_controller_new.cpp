#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/PositionSensor.hpp>
#include <cmath>
//#include <webots/Keyboard.hpp>
// #include <webots/GPS.hpp>
// #include <webots/InertialUnit.hpp>
#include <webots/DistanceSensor.hpp>
#include <iomanip>

#define TIME_STEP 16

#define normal_speed 6

using namespace webots;
using namespace std;

int main(int argc, char **argv){

 Robot *robot = new Robot();
 Motor *m;
 // Motor *linear_m;
 PositionSensor *pSensor;
 DistanceSensor *dSensor;
 // Keyboard *kb;
 
 m=robot->getMotor("p_motor");
 m->setPosition(INFINITY);
 m->setVelocity(0.0);
 
 // linear_m=robot->getMotor("linear_m");
 // linear_m->setPosition(INFINITY);
 // linear_m->setVelocity(0.0);
 
 // kb->enable(TIME_STEP);

 
 pSensor=robot->getPositionSensor("p_sensor");
 pSensor->enable(TIME_STEP);
 
 dSensor=robot->getDistanceSensor("laser_s");
 dSensor->enable(TIME_STEP);
 
 double speed=0.5;
 double angle=0;
 double distance=1000;
 int stage=0;
 double King_angle=0.0;
 double black_ang=0.0;
 int count=0;
 int i=0; 
 // double linear=0;
 
 
 while (robot->step(TIME_STEP) != -1)
 {  
    //bool is=false;
    if(stage==0)
    {
       if (distance >425)
        {
        m->setVelocity(speed);
        angle=pSensor->getValue();   
        cout<<"angle "<<angle<<endl; 
        distance=dSensor->getValue();
        cout<<"  Distance "<<distance<<endl;
        }
          
           
        if(distance<425)       
        {  
           cout<< "nkvfsnvkbdhs hd"<<distance<<endl;
           m->setVelocity(speed);
           stage=1;
           while(robot->step(TIME_STEP) != -1)
           
            
            { 
            
              if(angle!=1.57)
              {
              cout<<"IN WHILE"<<endl;
              m->setVelocity(speed);
              angle=pSensor->getValue();
              cout<<" angle "<<angle<<endl;
              }
              break ;
                
                              
            }
           m->setVelocity(0);
           King_angle=angle;
           angle=pSensor->getValue();
           cout<<" angle "<<angle<<endl; 
           cout<<"King angle "<<King_angle<<endl; 
           distance=dSensor->getValue();
           cout<<"King Distance "<<distance<<endl;
         }
       
       } 
       
       else{
       cout<< "nkvfsnvkbdhs hd";
       
       
       
       }
     
       
 
 } 
 
 delete robot;
 return 0;  // EXIT_SUCCESS          
    
 }
