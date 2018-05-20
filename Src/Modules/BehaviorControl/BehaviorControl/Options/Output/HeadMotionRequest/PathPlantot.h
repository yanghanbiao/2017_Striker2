option(PathPlantot, (float) x,(float) y, (const Pose2f&) target , (const Pose2f&) speed,(float)oldBallNorm)
{
    //how to make two state work at the same time one work one store
   //float oldBallNorm=0; 
   //old ball norm
  static float pathTrueOffest=100.f;
  static bool wrongPath=false;
  /** Simply sets the necessary angles */
  //Striker.h  
 /* initial_state(cunchu)
  {
    transition
    //once their distance are bigger than it usded to be, quit quickly
    //first build a float to store the current number
    //then turn to the next state and compare it with now's if they have too much difference quit it
    {
      if(state_time>1000)
        goto toball;
    }
    action
    { 
    //if(std::abs(oldBallNorm-theBallModel.estimate.position.norm())>150.f)
      //oldBallNorm=theBallModel.estimate.position.norm();
      
          OUTPUT_TEXT("oldBallNorm");
          OUTPUT_TEXT(oldBallNorm);
    }
  }*/
  
  initial_state(toball)
  {
    transition
    //once their distance are bigger than it usded to be, quit quickly
    //first build a float to store the current number
    //then turn to the next state and compare it with now's if they have too much difference quit it
    //one way time one way distance
    {
      //if(action_done||state_time>8000)
        //goto zaiqueren;
      if(((std::sqrt(std::pow(x-theRobotPose.translation.x(),2)+std::pow(y-theRobotPose.translation.y(),2))-oldBallNorm)>200.f)&&!wrongPath)
        goto zaiqueren;
      if(((std::sqrt(std::pow(x-theRobotPose.translation.x(),2)+std::pow(y-theRobotPose.translation.y(),2))<130.f)||state_time>5000.f)&&!wrongPath)
          goto zaiqueren;
     if(((std::sqrt(std::pow(x-theRobotPose.translation.x(),2)+std::pow(y-theRobotPose.translation.y(),2))-oldBallNorm)<200.f)&&(std::sqrt(std::pow(x-theRobotPose.translation.x(),2)+std::pow(y-theRobotPose.translation.y(),2))<130.f)&&wrongPath)
         goto huigui;
          //changshijianbudong
      //if(state_time>3500.f)
      //goto cunchu;
    }
    action
    { OUTPUT_TEXT(wrongPath);
      OUTPUT_TEXT("wrongPath1");
      if(!wrongPath)
    {theMotionRequest=thePathPlanner.plan(target,speed,true);
      wrongPath=false; 
   }
      if(wrongPath)
    {WalkToTarget(speed,target);
          wrongPath=true;
    }
      //OUTPUT_TEXT(action_done);

    }
  }
  
  state(zaiqueren)
  {
    transition
    //once their distance are bigger than it usded to be, quit quickly
    //first build a float to store the current number
    //then turn to the next state and compare it with now's if they have too much difference quit it
    {
      if(state_time>1000)
        goto toball;
    }
    action
    { OUTPUT_TEXT(wrongPath);
          OUTPUT_TEXT("wrongPath2");
        //if(((theBallModel.estimate.position.norm()-oldBallNorm)>200.f)||(std::abs(oldBallNorm-theBallModel.estimate.position.norm())<130.f))
      wrongPath=true;
    //OUTPUT_TEXT(wrongPath);
    //OUTPUT_TEXT(oldBallNorm);
    }
  }
    
  state(huigui)
  {
    transition
    //once their distance are bigger than it usded to be, quit quickly
    //first build a float to store the current number
    //then turn to the next state and compare it with now's if they have too much difference quit it
    {
      if(state_time>1000)
        goto toball;
    }
    action
    { OUTPUT_TEXT(wrongPath);
          OUTPUT_TEXT("wrongPath3");
        //if(((theBallModel.estimate.position.norm()-oldBallNorm)>200.f)||(std::abs(oldBallNorm-theBallModel.estimate.position.norm())<130.f))
      wrongPath=false;
    //OUTPUT_TEXT(wrongPath);
    //OUTPUT_TEXT(oldBallNorm);
    }
  }
}
