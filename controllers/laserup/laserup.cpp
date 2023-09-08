#include <webots/Keyboard.hpp>
#include <webots/GPS.hpp>
#include <webots/InertialUnit.hpp>
//#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/PositionSensor.hpp>


#define TIME_STEP 16
using namespace webots;

Robot *robot =new Robot();

Motor *linear_m;
PositionSensor *linear_p;

// double linear=0;
void sensorUp(float distance);

int main(int argc,char **argv)
{  
    // DistanceSensor *laser;
    // laser =robot->getDistanceSensor("laser_s");
    // laser->enable(TIME_STEP);
    
   
   // Keyboard kb;
   //Motor *p_motor;
    linear_m =robot->getMotor("linear_m");
    linear_m->setPosition(INFINITY);
    linear_m->setVelocity(0.0);
    
    linear_p =robot->getPositionSensor("linear_p");
    linear_p->enable(TIME_STEP);
    
    //p_motor =robot->getMotor("p_motor");
   // kb.enable(TIME_STEP);
    
    
    while (robot->step(TIME_STEP) != -1) 
    {
       // int key=kb.getKey();
       sensorUp(-0.53);robot->step(100);
       break;
       
       // if (key==87)
       // {
         // linear+=0.005;
       // }    
       // else if (key==83)
       // {
         // linear+=-0.005;
       // }
       // else
       // {
         // linear+=0.0;
       // }

       // linear_m->setPosition(linear);
       // int key=kb.getKey();
       
       // if (key==87)
       // { 
         // while (robot->step(TIME_STEP) != -1)
         // {
           // linear_m->setPosition(0.53);
         // }
       // }    
       // else if (key==83)
       // {
          // while (robot->step(TIME_STEP) != -1)
         // {
           // linear_m->setPosition(0.0);
         // }
       // }
       // else continue;
        


       
       
        // if (key2==65 && rotate<3.142)
    // {
        // rotate+=0.005;
    // }    
    // else if (key2==68 && rotate>-3.142)
    // {
        // rotate+=-0.005;
    // }
    // else
    // {
        // rotate+=0.0;
    // }

    // p_motor->setPosition(rotate);
    //
     }
    
 delete robot;
 return 0;  // EXIT_SUCCESS          
      
    
}

 void sensorUp(float distance)
{
  float velocity = 2 * (distance / abs(distance));
  float startPos = linear_p->getValue();
  while (robot->step(TIME_STEP) != -1)
  {
    float pos = linear_p->getValue();
    linear_m->setVelocity(velocity);
    if ((abs(startPos - pos) ) > abs(distance))
    {
      linear_m->setVelocity(0);
      break;
    } 
  }
}