#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>


#define TIME_STEP 16

#define normal_speed 6

int ir_readings[8];

double kp=1;
double kd=0.2;
double ki=0.02;
double prevoious_error = 0.0;
double integral = 0.0;
int coefficient[8]={-4000,-3000,-2000,-1000,1000,2000,3000,4000};


using namespace webots;

Robot *robot = new Robot();
Camera *camera = robot->getCamera("cam");

void delay(float t){
  float current = robot->getTime();
  float final  = current+(t/1000);
  while (final>robot->getTime()){
    robot->step(1);
  }
}


int main(int argc, char **argv) {

  camera->disable();
  DistanceSensor *ir[8];
  char irNames[8][5] = {"ir1","ir2","ir3","ir4","ir5","ir6","ir7","ir8"};
  for (int i = 0; i < 8; i++) {
    ir[i] = robot->getDistanceSensor(irNames[i]);
    ir[i]->enable(TIME_STEP);
  }
  
  
  Motor *motors[2];
  char motor_names[2][12] = {"left_motor", "right_motor"};
  for (int i = 0; i < 2; i++) {
    motors[i] = robot->getMotor(motor_names[i]);
    motors[i]->setPosition(INFINITY);
    motors[i]->setVelocity(0.0);
  }
  
  while (robot->step(TIME_STEP) != -1) {
  
    for (int i = 0; i < 8; i++) {
        if (ir[i]->getValue() <800 ){
          ir_readings[i]=0;
          }
        else{ir_readings[i]=1;  
        }  
    } 
   
    double error = 0.0;
    for (int j =0; j<8;j++){
      error=error+ir_readings[j]*coefficient[j];
    }
    
    double P=kp*error;
    double I= (ki*error)+integral;
    double D= kd*(error-prevoious_error);
    
    
    double correction = (P+I+D)/1000;
    double left_motor = normal_speed + correction;
    double right_motor = normal_speed - correction;
    
    if (left_motor <0.0) {left_motor=0.0;}
    if (left_motor >10.0) {left_motor=10.0;}
    if (right_motor<0.0) {right_motor=0.0;}
    if (right_motor>10.0) {right_motor=10.0;}
    
    
    motors[0]->setVelocity(left_motor);
    motors[1]->setVelocity(right_motor);
    
    prevoious_error = error;
    integral = I;
    
  delay(100);  
    
  }
    
    
  delete robot;
  return 0;  // EXIT_SUCCESS   
}