#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/PositionSensor.hpp>

#define TIME_STEP 16

using namespace webots;
using namespace std;


Robot *robot = new Robot();
Motor *armMotor;
PositionSensor *armPos;


double angle=0;
double speed=6;


void turnArm(float angle);
void delay(float t);

int main(int argc, char **argv){

 armMotor=robot->getMotor("Arm_mot");
 armMotor->setPosition(INFINITY);
 armMotor->setVelocity(0.0);

 armPos=robot->getPositionSensor("Arm_pos");
 armPos->enable(TIME_STEP);
 

   while (robot->step(TIME_STEP) != -1)
   {  
     
     turnArm(-45);delay(1000);
     turnArm(45);delay(1000);
     break;
                  
   } 
   
 delete robot;
 return 0;  // EXIT_SUCCESS          
    
 }
 
 
 
 
 //////////////////////////////////////////////////////////////////////////////////////////////
 
 
 void turnArm(float angle)
{
  float velocity = 2 * (angle / abs(angle));
  float startPos = armPos->getValue();
  while (robot->step(TIME_STEP) != -1)
  {
    float pos = armPos->getValue();
    armMotor->setVelocity(velocity);
    if ((abs(startPos - pos) / 3.142) * 180 > abs(angle))
    {
      armMotor->setVelocity(0);
      break;
    } 
  }
}// end of arm tuurning


  void delay(float t)
  {
    float current = robot->getTime();
    float final  = current+(t/1000);
    while (final>robot->getTime())
    {
      robot->step(1);
    }
}  

// void slide(bool reverse, bool ball)
// {

  // float END_POS;
  // if (ball) END_POS = 0.025;
  // else END_POS = 0.019;

  // base1Mot->setVelocity(0.04); //done
  // base2Mot->setVelocity(0.04);

  // if(reverse){

    // if(ball){
      // base1Mot -> setPosition(-0.02); //done
      // base2Mot -> setPosition(0.02);
    // }else{
      // base1Mot -> setPosition(0.01); //done
      // base2Mot -> setPosition(-0.01);
    // }

  // }else{
    // base1Mot -> setPosition(-END_POS); //done
    // base2Mot -> setPosition(END_POS);
  // }
  
  

  // int count = 0;
  // double pos1 = basepos1->getValue();
  // double pos2 = basepos2->getValue();
  // double prevoiusPos1;
  // double prevoiusPos2;

  // while (robot->step(timeStep) != -1 && !reverse)
  // {
    // prevoiusPos1 = pos1;
    // prevoiusPos2 = pos2;
    // pos1 = basepos1->getValue();
    // pos2 = basepos2->getValue();

    // if ( abs(prevoiusPos1 - pos1) < 0.001 && abs(prevoiusPos2-pos2) < 0.001 ) {
      // count += 1;
    // }
    // else{
      // count = 0;
    // }

    // if (count > 100) {
      // break;
    // }
  // }
// }
 
 
