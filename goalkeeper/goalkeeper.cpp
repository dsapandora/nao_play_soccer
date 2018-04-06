#include "goalkeeper.h"

#include <alvalue/alvalue.h>
#include <alcommon/alproxy.h>

#include <iostream>
#include <alcommon/albroker.h>
#include <althread/alcriticalsection.h>

#include <qi/log.hpp>

Goalkeeper::Goalkeeper(boost::shared_ptr<AL::ALBroker> broker,
                   const std::string& name)
  : AL::ALModule(broker, name),
  fCallbackMutex(AL::ALMutex::createALMutex())
{
  AL::ALMemoryProxy *memory = new AL::ALMemoryProxy(broker);
  memory->insertData("GameCtrl/teamNumber", 1); //must be non-zero
  memory->insertData("GameCtrl/teamColour", 0);//omitted, black
  memory->insertData("GameCtrl/playerNumber", 1); //goalkeeper

  setModuleDescription("FollowBall module");

  functionName("ballDetected", getName(), "Guarda en variable");
  BIND_METHOD(Goalkeeper::ballDetected);

  functionName("leftBumperPressed", getName(), "Hace penal");
  BIND_METHOD(Goalkeeper::leftBumperPressed);
}
void Goalkeeper::caidaizq()
{
  std::string behaviorID;
  behaviorID = fFrameManagerProxy.newBehaviorFromFile("/home/dsapandora/Public/nao_workspace/choreographs/caida1.xar", "");
  fFrameManagerProxy.completeBehavior(behaviorID);
}

void Goalkeeper::caidader()
{
  std::string behaviorID;
  behaviorID = fFrameManagerProxy.newBehaviorFromFile("/home/dsapandora/Public/nao_workspace/choreographs/caida2.xar", "");
  fFrameManagerProxy.completeBehavior(behaviorID);
}

Goalkeeper::~Goalkeeper()
{
  //fMemoryProxy.unsubscribeToMicroEvent("EKBallDetected", "Goalkeeper");
  //fMemoryProxy.unsubscribeToEvent("LeftBumperPressed", "Goalkeeper");
  fPostureProxy.goToPosture("Crouch", 0.5f);
}

void Goalkeeper::init()
{
  try {
    fMemoryProxy = AL::ALMemoryProxy(getParentBroker());
    fMotionProxy = AL::ALMotionProxy(getParentBroker());
    fFrameManagerProxy = AL::ALFrameManagerProxy(getParentBroker());
    //fMemoryProxy.subscribeToMicroEvent("EKBallDetected", "Goalkeeper", "EKBallDetected", "ballDetected");
    fMemoryProxy.subscribeToEvent("LeftBumperPressed", "Goalkeeper", "LefttBumperPressed", "leftBumperPressed");
    fMotionProxy.setStiffnesses("Head", 0.7);
    fMotionProxy.setStiffnesses("Body", 0.7);
    fPostureProxy.goToPosture("StandInit", 0.5f);
  }
  catch (const AL::ALError& e) {
    qiLogError("goalkeeper.init") << e.what() << std::endl;
  }
}

void Goalkeeper::leftBumperPressed()
{
  fMotionProxy.setStiffnesses("Head", 0.7);
  fMotionProxy.setStiffnesses("Body", 0.7);
  fPostureProxy.goToPosture("StandInit", 0.5f);
  fMemoryProxy.subscribeToMicroEvent("EKBallDetected", "Goalkeeper", "EKBallDetected", "ballDetected");
}

void Goalkeeper::centerBall()
{
    if(fState.getSize() > 0)
    {
      fMotionProxy.setAngles("Head", fState[1], 0.4);
    }
}

void Goalkeeper::walkToBall()
{
  //fMotionProxy.setWalkTargetVelocity((float)fState[3][0],(float)fState[3][1],(float)fState[3][2],0.8);
  //fMotionProxy.moveTo((float)fState[2][0]*0.01-0.15, (float)fState[2][1]*0.01+0.05, (float)fState[2][2]);
}

void Goalkeeper::shoot()
{
  std::string behaviorID;
  behaviorID = fFrameManagerProxy.newBehaviorFromFile("/home/dsapandora/Public/nao_workspace/choreographs/kick.xar", "");
  fFrameManagerProxy.completeBehavior(behaviorID);
}

void Goalkeeper::ballDetected()
{
  AL::ALCriticalSection section(fCallbackMutex);
  fState =  fMemoryProxy.getData("EKBallDetected");


  fMotionProxy.setAngles("Head", fState[1], 0.4);
  //fMotionProxy.setWalkTargetVelocity(0, fState[2][2], 0, 0.1);

  if((float)fState[0] < 65)
  {
    std::cout << "Theta: " << (float)fState[2][2] << std::endl;
    if ((float)fState[2][2] > 0.35)
    {
      caidader();
    }
    else if((float)fState[2][2] < -0.35)
    {
      caidaizq();
    }
  }
/**
  if((float)fState[0] > 17)
  {
    fMotionProxy.setAngles("Head", fState[1], 0.4);
    //fMotionProxy.setWalkTargetVelocity(0.0,(float)fState[3][1],(float)fState[3][2],0.1);
    //std::cout << "Seconds: " << (float)fState[4][0] << std::endl;
    //std::cout << "Miccros: " << (float)fState[4][1] << std::endl;

  }
  else
  {
    if ((float)fState[2][2] > 0.3)
    {
      fMotionProxy.setWalkTargetVelocity(0, 0.4, 0, 0.1);
    }
    else if((float)fState[2][2] < -0.3)
    {
      fMotionProxy.setWalkTargetVelocity(0, -0.4, 0, 0.1);
    }
    else
**/
    //{
      //if(abs((float)fState[2][2]) < 0.08)
      //{
        //fMemoryProxy.unsubscribeToMicroEvent("EKBallDetected", "Goalkeeper");
        //fMotionProxy.moveTo(0, 0.04, 0);
        //shoot();
      //}
    //}
    //fMotionProxy.setStiffnesses("Head", 0.0);
    //fMotionProxy.setStiffnesses("Body", 0.0);

/**
        if ((float)fState[2][2] > 1)
        {
          fMotionProxy.setWalkTargetVelocity(0, 0, 1.0, 0.1);
        }
        else if((float)fState[2][2] < -1)
        {
          fMotionProxy.setWalkTargetVelocity(0, 0, -1.0, 0.1);
        }
        else
        {
          if(abs((float)fState[2][2]) < 0.08)
          {
            fMemoryProxy.unsubscribeToMicroEvent("EKBallDetected", "Goalkeeper");
            fMotionProxy.moveTo(0, 0.04, 0);
            //shoot();
          }
        }
**/
  //}
}
