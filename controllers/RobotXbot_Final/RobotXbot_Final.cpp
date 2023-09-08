//Including Header files
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <string.h>
#include <webots/PositionSensor.hpp>
#include <cmath>
#include <iomanip>


#define TIME_STEP 16
#define normal_speed 3


using namespace webots;  
using namespace std;

//line following arrays
float line_follow_error_array[2]={0,0};   //previous_error , Integral
float line_follow_PID[3]={0.8,0,0};  //kp,ki,kd

//line following array
float dotted_line_follow_PID[3]={10,0,0};  //kp,ki,kd


//wall following arrays
float wall_follow_error_array[2]={0,0};   //previous_error , Integral
float wall_follow_PID[3]={0.0019,0,0.00001};   //kp,ki,kd
string color="blue";

//laser sensor position
double linear_distance=0;

//robot functions
void line_following();
void line_following_dotted(string color);
void wall_following();
void turnMotors(float correction);
void delay(float t);
void turnRobot(float angle);
void findDottedLine();
void sensorMove(float distance);
void goForward(double time);
void liftRook();
void turnArm(float angle);
void slide(bool reverse, bool box);
void poleRotate(float angle);


double PID(double error,double kp,double ki,double kd);
double angle=0;
double speed=6;

//object initializing
Robot *robot = new Robot();
DistanceSensor *ir[8];
Motor *motors[4];
Motor *linear_m;
Motor *armMotor;
Motor *base1Mot;
Motor *base2Mot;
Motor *p_motor;
DistanceSensor *dis[4];
PositionSensor *psensor;
PositionSensor *linear_p;
PositionSensor *armPos;
PositionSensor *base1Pos;
PositionSensor *base2Pos;
PositionSensor *p_sensor;


//////////////////////////////////////////////////////////////////////////////
/////////////////////////////MAIN FUNCTION////////////////////////////////////

int main(int argc, char **argv) 
{    
      char irNames[8][5] = {"ir1","ir2","ir3","ir4","ir5","ir6","ir7","ir8"};
      for (int i = 0; i < 8; i++) {
        ir[i] = robot->getDistanceSensor(irNames[i]);
        ir[i]->enable(TIME_STEP);
      }
            
      char motor_names[4][9] = {"left_fm", "left_bm","right_fm","right_bm"};
      for (int i = 0; i < 4; i++) {
        motors[i] = robot->getMotor(motor_names[i]);
        motors[i]->setPosition(INFINITY);
        motors[i]->setVelocity(0.0);
      }

      
      char disNames[4][7] = {"us_lb", "us_lf", "us_rf", "us_rb"};
      for (int i = 0; i < 4; i++) {
      dis[i] = robot->getDistanceSensor(disNames[i]);
      dis[i]->enable(TIME_STEP);
      }
      
       linear_m =robot->getMotor("linear_m");
       linear_m->setPosition(INFINITY);
       linear_m->setVelocity(0);

      
       psensor = robot->getPositionSensor("right_fp");
       psensor->enable(TIME_STEP);
       
       linear_p =robot->getPositionSensor("linear_p");
       linear_p->enable(TIME_STEP);

        armMotor=robot->getMotor("Arm_mot");
        armMotor->setPosition(INFINITY);
        armMotor->setVelocity(0.0);

        base1Mot=robot->getMotor("base1Mot");
        base1Mot->setPosition(INFINITY);
        base1Mot->setVelocity(0.0);

        base2Mot=robot->getMotor("base2Mot");
        base2Mot->setPosition(INFINITY);
        base2Mot->setVelocity(0.0);  

        p_motor=robot->getMotor("p_motor");
        p_motor->setPosition(INFINITY);
        p_motor->setVelocity(0);
        

        p_sensor=robot->getPositionSensor("p_sensor");
        p_sensor->enable(TIME_STEP);
        
        armPos=robot->getPositionSensor("Arm_pos");
        armPos->enable(TIME_STEP);
        
        
        base1Pos=robot->getPositionSensor("base1pos");
        base1Pos->enable(TIME_STEP);
        
        
        base2Pos=robot->getPositionSensor("base2pos");
        base2Pos->enable(TIME_STEP);
        
//////////////////////////////////////////////////////////////////////////////////  
////////////////////////////MAIN LOOP////////////////////////////////////////////
      while (robot->step(TIME_STEP) != -1){
      
        cout<<"line following started."<<endl;
        line_following();
        
        cout <<"Wall following started."<<endl;
        wall_following();
        
        cout<<"Find dotted line started."<<endl;
        findDottedLine();       
        
        cout<<"Dotted line following started."<<endl;
        line_following_dotted(color);
        
   
        
      }
      
      delete robot;
      return 0;  // EXIT_SUCCESS   
 }





//////////////////////////////////////////////////////////////////////////////
///////////////////////////           FUNCTIONS            ///////////////////

   
//line following 

void line_following()

{  
   
   int ir_readings[8];
   double ir_values[8];
   int coefficient[8]={-4000,-3000,-2000,-1000,1000,2000,3000,4000};
   bool isLinefollowing=true;
   int total_t_count=0;
   while ((robot->step(TIME_STEP) != -1) && isLinefollowing) 
   {
      
      for (int i = 0; i < 8; i++) { 
       cout <<"IR "<< ir[i]->getValue() << endl; 
            ir_values =  ir[i]->getValue();           
            if (ir[i]->getValue() < 800 ){
              ir_readings[i]=0;              //black 1  color 0
              }  
            else
            {
              ir_readings[i]=1;  
            }  
        }
      
        double error = 0.0;
        for (int j =0; j<8;j++){
          error=error+ir_readings[j]*coefficient[j];
        }
        
        double correction= PID(error,line_follow_PID[0],line_follow_PID[1],line_follow_PID[2]);
        turnMotors(correction);
        
        int t_count=0;
        for (int k:ir_readings)
        {
            if (k!=1)  
            {
              break;
            }
            else t_count+=1;          
        }
        
      //  cout <<"t_count  :"<<t_count<<endl;
        
        if (t_count==8) 
          {
            total_t_count+=1;
          }
        if (total_t_count>=5 )
        {
           isLinefollowing=false;             
        }

        int carpet=0;
         for (int k:ir_values)
        {
            if ((300<k)&&(k<400) ) 
            {
              carpet+=1;
            }
            else break;          
        }

        if (carpet==8)
        {
          liftRook();break;
        }

    }   
            
    line_follow_error_array[0]=0;
    line_follow_error_array[1]=0;
    cout <<"Line following END."<<endl;
}     


/////////////////////////////////////////////////////////////////////////////////////
//dotted line following 

void line_following_dotted(string color)

{  
   float c_value;
   if (color=="red")
   {
     c_value=210;
   }
   
   else if (color=="blue") 
   {
     c_value =500;
   }
     
   int ir_readings[8];
   float ir_values[8];
   int coefficient[8]={-4000,-3000,-2000,-1000,1000,2000,3000,4000};
   bool isLinefollowing=true;
   while ((robot->step(TIME_STEP) != -1) && isLinefollowing) 
   {
      
      for (int i = 0; i < 8; i++) {
            cout<<"ir "<<i<<" "<<ir[i]->getValue()<<endl;
            ir_values[i]=ir[i]->getValue();
            if (ir[i]->getValue() < c_value ){   //black 1  color 0
              ir_readings[i]=0;
              }
            else
            {
              ir_readings[i]=1;  
            }  
        }
      
        double error = 0.0;
        for (int j =0; j<8;j++){
          error=error+ir_readings[j]*coefficient[j];
        }
        
        double correction= PID(error,dotted_line_follow_PID[0],dotted_line_follow_PID[1],dotted_line_follow_PID[2]);
        turnMotors(correction);

        int right_count=0;
        int left_count=0;  
             
        for (int n=0;n<=3;n++)  
       {   
           if ((ir_readings[n]==0)&& ir_values[n]<210)
           {       
              right_count+=1; 
           }
           else if ((ir_readings[7-n]==0)&& ir_values[n]<210)
           {
              left_count+=1;
           }
           else break;
             
       }
       
       if ((left_count>right_count) && left_count>=2 )
       {  
          goForward(1500);
          cout<<"turn left"<<endl;
          turnRobot(-80);break;
       }
       else if ((left_count<right_count) &&  right_count>=2)
       {
          goForward(1500);
          cout<<"turn right"<<endl;
          turnRobot(80);break;
       }
      
        
    }
    line_following();    
    line_follow_error_array[0]=0;
    line_follow_error_array[1]=0;
    cout <<"Dotted Line following END."<<endl;
               
 }  
    


//////////////////////////////////////////////////////////////////////////////////////////
//wall following
void wall_following()
{ 
  int ir_readings[8];
  double error = 0.0;
  int dis_readings[4]={0,0,0,0};
  int coefficient[4] = {-2000, -1000, 1000, 2000};
  bool isWallfollowing=true;
  while ((robot->step(TIME_STEP) != -1) && isWallfollowing) {

    // Read distance sensor values
    
    if ((dis[1]->getValue() > dis[2]->getValue()) && dis[0]->getValue() > 320.0) //turn left
    {
      dis_readings[0] = 1;
      dis_readings[1] = 1;
      dis_readings[2] = 0;
      dis_readings[3] = 0;
    } 
    else if ( (dis[2]->getValue() > dis[1]->getValue()) && dis[3]->getValue() > 320.0) //turn right 
    {
      dis_readings[0] = 0;
      dis_readings[1] = 0;
      dis_readings[2] = 1;
      dis_readings[3] = 1;
    }
    
    else
    {
      dis_readings[0] = 0; //go forward
      dis_readings[1] = 0;
      dis_readings[2] = 0;
      dis_readings[3] = 0;
    }   
    
    
    for (int i = 0; i < 4; i++) {
      error += coefficient[i] * dis_readings[i];
      //cout<<"ds_sensor "<< i <<" : "<< dis[i]->getValue() <<endl; //ds[0]=left_bk,ds[1]=left_ft,ds[2]=right_bk,ds[3]=right_ft
    }
    
    double correction=PID(error,wall_follow_PID[0],wall_follow_PID[1],wall_follow_PID[2]);
    turnMotors(correction);
    
    for (int j=0;j<=7;j++)
    {
        ir_readings[j]=ir[j]->getValue();
    }
    
     int t_count=0;
      for (float k:ir_readings)
      {
          if (k==1000)  
          { 
            break;
          }
          else t_count+=1;          
      }
      
      if (t_count==8)
      {
          isWallfollowing=false;
      }
 }
  line_follow_error_array[0]=0;
  line_follow_error_array[1]=0;
  cout<<"Wall following Ended."<<endl;

}

///////////////////////////////////////////////////////////////////////////////////////

void goForward(double time)
{
  for (int i=0;i<=3;i++)
  {
    motors[i]->setVelocity(normal_speed);
  }
  delay(time);

}

////////////////////////////////////////////////////////////////////////////////////////
//motor turning
void turnMotors(float correction)
{
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
              
}

//////////////////////////////////////////////////////////////////////////////////////////
//PID Function
double PID(double error,double kp,double ki,double kd)
{   
    float integral=line_follow_error_array[1];
    float previous_error=line_follow_error_array[0];
    double P=kp*error;
    double I= (ki*error)+integral;
    double D= kd*(error-previous_error);    
    double correction = (P+I+D)/1000;
    line_follow_error_array[0]=error;
    line_follow_error_array[1]= I;
    return correction;
}



////////////////////////////////////////////////////////////////////////////////////////
//DELAY Function

void delay(float t)
  {
    float current = robot->getTime();
    float final  = current+(t/1000);
    while (final>robot->getTime())
    {
      robot->step(1);
    }
}

///////////////////////////////////////////////////////////////////////////////////
// //FIND DOTTED LINE

void findDottedLine()
{
   int ir_readings[8];
   int ir_values[8]; 
   int coefficient[8]={-4000,-3000,-2000,-1000,1000,2000,3000,4000};
   
   while (robot->step(TIME_STEP) != -1)
   {
      
      for (int i = 0; i < 8; i++) {
            cout<<"ir "<<i<<" "<<ir[i]->getValue()<<endl;
            if (ir[i]->getValue() < 800 ){   //black 1  color 0
              ir_readings[i]=0;
              }
            else
            {
              ir_readings[i]=1;  
            }  
        }
       
               
        double error = 0.0;
        for (int j =0; j<8;j++){
          error=error+ir_readings[j]*coefficient[j];
        }
        
        double correction= PID(error,dotted_line_follow_PID[0],dotted_line_follow_PID[1],dotted_line_follow_PID[2]);
        turnMotors(correction);
        
           int left_count=0;
           int right_count=0;  


               for (int n=0;n<=3;n++)  
         {   
           if (((500<ir_values[7-n])&&(ir_values[n]!=1000)) && ((100<ir_values[n])&&(ir_values[n]<300)))
           {       
              right_count+=1; 
              cout<<"right_count"<<right_count<<endl;

           }
           else if (((500<ir_values[n])&&(ir_values[n]!=1000)) && ((100<ir_values[7-n])&&(ir_values[7-n]<300)))
           {
              left_count+=1;
              cout<<"left_count"<<left_count<<endl;
           }
           
           else {break;}  
             
       }
       cout << "left_count "<< left_count<<endl; 
       cout << "right_count "<< right_count<<endl;          
       
       if ((left_count==4) && color=="blue")
       {  
          cout<<"left BLUE"<<endl;
          turnRobot(-90);break;
       }
       else if ((left_count==4) && color=="red")
       {
          cout<<"left RED"<<endl;
          turnRobot(90);break;
       }
       else if ((right_count==4) && color=="blue")
       {  
          cout<<"right BLUE"<<endl;
          turnRobot(90);break;
       }
       else if ((right_count==4) && color=="red")
       {  
          cout<<"right RED"<<endl;
          turnRobot(-90);break;
       }
   }
   cout<<"Find dotted line END."<<endl;
   
 }  
   
 ///////////////////////////////////////////////////////////////////////////////////
 //turn Robot
 
 void turnRobot(float angle)
{
  double m = 8.65;


  double start_location = psensor -> getValue();
  
  int direction = int(angle) / abs(int(angle));
  double delta_pos = abs(angle / m);  

  motors[0]->setVelocity(direction * 0.6 * normal_speed);
  motors[1]->setVelocity(direction * 0.6 * normal_speed);
  motors[2]->setVelocity(direction * -0.6 * normal_speed);
  motors[3]->setVelocity(direction * -0.6 * normal_speed);


  while (robot->step(TIME_STEP) != -1){
    if ( abs(start_location - psensor -> getValue()) > delta_pos ){
      turnMotors(0);
      break;
    }
  }

} 

 
//sensor Move
void sensorMove(float distance)
 {  
   
   float start_location=linear_p->getValue();
   int direction = distance /abs(distance);
   while (robot->step(TIME_STEP) != -1) 
    {
      linear_m->setVelocity(direction*0.3);
      linear_distance=linear_p->getValue();
      if (abs(start_location - linear_distance)>abs(distance))
      {  
         linear_m->setVelocity(0.0);
         break;
      }      
    }
    cout<<"Sensor move END."<<endl;
 }  
 
 ///////////////////////////////////////////////////////////////////////////////////////////// 
 void liftRook()
 {  
    sensorMove();
    delay(3000)
     
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
}
 
/////////////////////////////////////////////////////////////////////////////////////////////// 
void slide(bool reverse, bool box)
{

  float END_POS;
  if  (box) END_POS = 0.025;
  else END_POS = 0.019;

  base1Mot->setVelocity(0.04); //done
  base2Mot->setVelocity(0.04);

  if(reverse){

    if (box){
      base1Mot -> setPosition(-0.015); //done
      base2Mot -> setPosition(0.015);
    }else{
      base1Mot -> setPosition(0.01); //done
      base2Mot -> setPosition(-0.01);
    }

  }else{
    base1Mot -> setPosition(-END_POS); //done
    base2Mot -> setPosition(END_POS);
  }
  
  int count = 0;
  double pos1 = base1Pos->getValue();
  double pos2 = base2Pos->getValue();
  double prevoiusPos1;
  double prevoiusPos2;

  while (robot->step(TIME_STEP) != -1 && !reverse)
  {
    prevoiusPos1 = pos1;
    prevoiusPos2 = pos2;
    pos1 = base1Pos->getValue();
    pos2 = base2Pos->getValue();

    if ( abs(prevoiusPos1 - pos1) < 0.001 && abs(prevoiusPos2-pos2) < 0.001 ) {
      count += 1;
    }
    else{
      count = 0;
    }

    if (count > 100) {
      break;
    }
  }
}
 
///////////////////////////////////////////////////////////////////////////////////

void poleRotate(float angle)

{
  float velocity = 2 * (angle / abs(angle));
  float startPos = p_sensor->getValue();
  while (robot->step(TIME_STEP) != -1)
  {
    float pos = p_sensor->getValue();
    p_motor->setVelocity(velocity);
    if ((abs(startPos - pos) / 3.142) * 180 > abs(angle))
    {
      p_motor->setVelocity(0);
      break;
    } 
  }
}
//////////////////////////////////////////////////////////////////////////////////////////


// void chessBoard()
// {

 // dSensor=robot->getDistanceSensor("laser_s");
 // dSensor->enable(TIME_STEP);
 
 // double speed=0.5;
 // double angle=0;
 // double distance=1000;
 // int stage=0;
 // double King_angle=0.0;
 // double black_ang=0.0;
 // int count=0;
 // int i=0; 
 double linear=0;
 
 
 // while (robot->step(TIME_STEP) != -1)
 // {  
    // //bool is=false;
    // if(stage==0)
    // {
       // if (distance >425)
        // {
        // m->setVelocity(speed);
        // angle=pSensor->getValue();   
        // cout<<"angle "<<angle<<endl; 
        // distance=dSensor->getValue();
        // cout<<"  Distance "<<distance<<endl;
        // }
          
           
        // if(distance<425)       
        // {  
           // cout<< "nkvfsnvkbdhs hd"<<distance<<endl;
           // m->setVelocity(speed);
           // stage=1;
           // while(robot->step(TIME_STEP) != -1)
           
            
            // { 
            
              // if(angle!=1.57)
              // {
              // cout<<"IN WHILE"<<endl;
              // m->setVelocity(speed);
              // angle=pSensor->getValue();
              // cout<<" angle "<<angle<<endl;
              // }
              // break ;
                
                              
            // }
           // m->setVelocity(0);
           // King_angle=angle;
           // angle=pSensor->getValue();
           // cout<<" angle "<<angle<<endl; 
           // cout<<"King angle "<<King_angle<<endl; 
           // distance=dSensor->getValue();
           // cout<<"King Distance "<<distance<<endl;
         // }
       
       // } 
       
       // else{
       // cout<< "nkvfsnvkbdhs hd";
       
       
       
       // }
     
       
 
 // } 
// }
 
