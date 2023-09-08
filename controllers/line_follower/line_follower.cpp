#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>



#define TIME_STEP 16

#define normal_speed 6

int ir_readings[8];

double kp=10;
double kd=0;
double ki=0;
double prevoious_error = 0.0;
double integral = 0.0;
int coefficient[8]={-4000,-3000,-2000,-1000,1000,2000,3000,4000};



using namespace webots;
using namespace std;

//turn_left
//turn_right
    
//dot_pid function


int main(int argc, char **argv) {
  Robot *line_follow = new Robot();
  
  //get_colour_function

  
  DistanceSensor *ir[8];
  char irNames[8][5] = {"ir1","ir2","ir3","ir4","ir5","ir6","ir7","ir8"};
  for (int i = 0; i < 8; i++) {
    ir[i] = line_follow->getDistanceSensor(irNames[i]);
    ir[i]->enable(TIME_STEP);
    std::cout<< 123 ;
  }
  
  
  Motor *motors[4];
  char motor_names[4][9] = {"left_fm", "left_bm","right_fm","right_bm"};
  for (int i = 0; i < 4; i++) {
    motors[i] = line_follow->getMotor(motor_names[i]);
    motors[i]->setPosition(INFINITY);
    motors[i]->setVelocity(0.0);
  }
  
  while (line_follow->step(TIME_STEP) != -1) {
   
   for (int i = 0; i < 8; i++) {
   cout<<"sensor "<<i<<"  "<< ir[i]->getValue()<<endl;
        if (ir[i]->getValue() <670 ){
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
    motors[1]->setVelocity(left_motor);
    motors[2]->setVelocity(right_motor);
    motors[3]->setVelocity(right_motor);
    
    prevoious_error = error;
    integral = I;
    
   
    
}
    
    
  delete line_follow;
  return 0;  // EXIT_SUCCESS   
}