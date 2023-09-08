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
 Motor *left_fm;
 Motor *left_bm;
 Motor *right_bm;
 Motor *right_fm;
 
 // Motor *linear_m;
 PositionSensor *pSensor;
 DistanceSensor *dSensor;
 // Keyboard *kb;
 
 m=robot->getMotor("p_motor");
 m->setPosition(INFINITY);
 m->setVelocity(0.0);
 
 left_fm=robot->getMotor("left_fm");
 left_fm->setPosition(INFINITY);
 left_fm->setVelocity(0.0);
 
 right_fm=robot->getMotor("right_fm");
 right_fm->setPosition(INFINITY);
 right_fm->setVelocity(0.0);
 
 left_bm=robot->getMotor("left_bm");
 left_bm->setPosition(INFINITY);
 m->setVelocity(0.0);
 
 right_bm=robot->getMotor("right_bm");
 right_bm->setPosition(INFINITY);
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
 double King_angle=0;
 int vertical_dis=0;
 // double linear=0;
 
 cout << " Shanilka ";
 while (robot->step(TIME_STEP) != -1)
 {  
   
    if(stage==0)
    {
       
        //distance=dSensor->getValue();
        if (distance >500)
        {
          left_fm->setVelocity(0);
             right_fm->setVelocity(0);
             left_bm->setVelocity(0);
             right_bm->setVelocity(0);
          
          m->setVelocity(speed);
          angle=pSensor->getValue();
          cout<<"angle "<<angle; 
          King_angle=angle;            
          distance=dSensor->getValue();
          cout<<"  Distance "<<distance<<endl;
          cout<<"  King_angle "<<King_angle<<endl;  
          
        } 
         
      else 
         { 
             int King_dis=distance;
             cout<<"King Distance "<<distance<<endl;
             int i=0;
             stage=1;
             //angle=pSensor->getValue();
             cout<<"New angle "<<angle;
             while(robot->step(TIME_STEP) != -1)
              
              {
                  
                  if(angle>=1.5708)
                  { 
                    break;            
                  } 
                  else
                  {               
                    cout<<"IN WHILE"<<endl;
                    m->setVelocity(speed);
                    angle=pSensor->getValue();
                    cout<<" angle "<<angle;
                  }
                  i=i+1;   
                                
              }
             m->setVelocity(0.0);
             //King_angle=angle;
             cout<<"King angle "<<King_angle<<endl; 
             distance=dSensor->getValue();
             cout<<"King Distance "<<King_dis<<endl;
             cout<<"distance"<<distance<<endl;
             vertical_dis=King_dis*0.665;
             cout<<"vertical_dis "<<vertical_dis<<endl;
             cout<<"stage "<<stage<<endl;       
        }
           
       
       
       
      } 
  else if(stage==1)
      {
         cout<<"stage "<<endl;
         if(distance >= vertical_dis)
         {
          stage=2;
          cout<<"stage "<<stage<<endl;
          
          //linear_actuator_up()
         }
         else
         {
          stage=3;
          //turn_ninty()
          }
      
      
    }
      
      else if(stage==2)
      {
          distance=1000;
          while(robot->step(TIME_STEP) != -1)
          {
            if(angle<=0.0)
            {
            break;
            }
            else
            {
            m->setVelocity(-speed);
            angle=pSensor->getValue();
            cout<<"Angle in stage 2 "<<angle <<endl;
            }
            
          }
         m->setVelocity(0.0); 
         cout<<"End break" <<endl;
         while(robot->step(TIME_STEP) != -1)
         { 
           distance=dSensor->getValue();
           if(distance<400)
             {
             left_fm->setVelocity(0);
             right_fm->setVelocity(0);
             left_bm->setVelocity(0);
             right_bm->setVelocity(0);
             }
             
           else
           {
             left_fm->setVelocity(5);
             right_fm->setVelocity(5);
             left_bm->setVelocity(5);
             right_bm->setVelocity(5);
             }
             
          }
       }  
             
            
          
        
          
      
      
      
 
       
     // else if (stage==1)
     // {
         
         // cout<<"King angle "<<King_angle<<endl; 
         // //distance=dSensor->getValue();
         // left_fm.setVelocity(5);
         // right_fm.setVelocity(5);
         // left_bm.setVelocity(5);
         // right_bm.setVelocity(5);
     // }
              
 }
 delete robot;
 return 0;  // EXIT_SUCCESS          
    
 }
 
 
 