////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>

#define NORMAL_SPEED 1
#define TIME_STEP 64



// All the webots classes are defined in the "webots" namespace
using namespace webots;
using namespace std;

//motors and sensors names
char motorNames[4][15]={"left_fm","left_bm","right_fm","right_bm"};
char dsNames[3][10]={"left_ds","front_ds","right_ds"};
//char irNames[3][10]={"left_ir","mid_ir","right_ds"};

double kp=0.001;
double kd=0;
double ki=0;
double prevoious_error = 0.0;
double integral = 0.0;
float coefficients[2] = {-100,100};

Robot *robot = new Robot();
void delay(float t){
  float current = robot->getTime();
  float final  = current+(t/1000);
  while (final>robot->getTime()){
    robot->step(1);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////main function///////////////////////////////////////////

int main(int argc, char **argv) {
 
  
    
  //Motor Objects 
  Motor *wheels[4];

  //initializing motors 
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(motorNames[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
  }
  
  //Sensor Objects
  DistanceSensor *ds[3];
  
  //initializing sensors
  for (int j =0; j<3;j++)
  {
    ds[j] = robot->getDistanceSensor(dsNames[j]);
   // ds[3+j]= robot->getDistanceSensor(irNames[j]);
    ds[j]->enable(TIME_STEP);
   // ds[3+j]->enable(TIME_STEP);   
  }
  
  
  ///////////////////////////////////////////////////////////////////////////////
  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  //////////////////////////////////////////////////////////////////////////////
  
  
  while (robot->step(TIME_STEP) != -1) {

    // Read the sensors:  
    double left_value = ds[0] -> getValue();
    double right_value = ds[1] -> getValue();
    //  double front_value = ds[2] -> getValue();

    double ds_readings[2]={left_value,right_value};
    cout << "left_wall " << left_value <<"  right_wall " << right_value << endl;


    double error = 0.0;
    for (int j =0; j<2;j++)
    {
     error=error+ds_readings[j]*coefficients[j];
    }
    
    double P= kp*error;
    double I= (ki*error)+integral;
    double D= kd*(error-prevoious_error);
    
    
    double correction = NORMAL_SPEED*(P+I+D)/100;
    double left_speed =  NORMAL_SPEED + correction;
    double right_speed = NORMAL_SPEED - correction;
    
    if (left_speed <0.0) {left_speed=0.0;}
    if (left_speed >10.0) {left_speed=10.0;}
    if (right_speed <0.0) {right_speed=0.0;}
    if (right_speed >10.0) {right_speed=10.0;}

    wheels[0]->setVelocity(left_speed);
    wheels[1]->setVelocity(left_speed); 
    wheels[2]->setVelocity(right_speed);
    wheels[3]->setVelocity(right_speed);

    prevoious_error = error;
    integral = I;
    delay(100);

    
  }

  // Enter here exit cleanup code.
  delete robot;
  return 0;
 }
