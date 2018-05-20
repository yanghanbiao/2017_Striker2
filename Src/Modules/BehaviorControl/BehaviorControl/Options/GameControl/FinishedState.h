option(FinishedState)
{
  /* position has been reached -> stand and wait */
  /**进球后，自己回到上场的位置，自定位可以比较准确*/
  initial_state(stand)
  {
	  transition
	  {
		  if(std::abs(theRobotPose.translation.y() + 750.f) < 100.f && (theRobotPose.translation.x()+750.f)<100.f)
			  goto turn;
	  }
    action
    {
		OUTPUT_TEXT("finishedStand");
	/**为了往前走，走得稳一点，若机器人在x=-750的前面，则先转身，面对己方球门*/	
	if(std::abs(theRobotPose.translation.x()+750.f)>0.f)	
	{	
	   /**若角度太大，先调整角度面对己方球门*/	
	  if(std::abs(theRobotPose.rotation - pi) > 15_deg)
	  {
		WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(-(theRobotPose.rotation - pi), 0.f, 0.f));
		LookLeftandRight();
	  }
	  /**若角度合适，直接走到点(-750,-750)附近*/
	  else	
	  {	  
	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi), (theRobotPose.translation.x()+750.f ), theRobotPose.translation.y()+750.f));
		LookLeftandRight();	  
	  }
	  
	}
	/**为了往前走，走得稳一点，若机器人在x=-750的后面，则先转身，面对敌方球门*/
	else
	{
	  /**若角度太大，则先调整角度面对敌方球门*/	
	  if(std::abs(theRobotPose.rotation ) > 15_deg)
	  {
		WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(-(theRobotPose.rotation), 0.f, 0.f));
		LookLeftandRight();
	  }
	  /**若角度合适，则直接走到点(-750,-750)附近*/
	  else	
	  {	  
	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation), -(theRobotPose.translation.x()+750.f ), -(theRobotPose.translation.y()+750.f)));
      	LookLeftandRight();  	  
	  }
	}
		
    }
  }
  /**调整角度面对敌方球门*/
    state(turn)
  {
	  transition
	  {
			/**调整好角度后跳进positiononfield*/
			if(std::abs(theRobotPose.rotation) < 5_deg)
				  goto positiononfield;
	  }
	  action
	  {
		  OUTPUT_TEXT("finishedturn");
		  WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(-theRobotPose.rotation, 0.f, 0.f));
	      LookForward();
	  }
  }
	/**方向面对敌方球门，再次调整位置，点(-750,-750)*/
  state(positiononfield)
  {
	  transition
	  {
		  if(std::abs(theRobotPose.translation.x()+750) < 100 &&std::abs(theRobotPose.translation.y() + 750.f) < 10 && std::abs(theRobotPose.rotation) < 5_deg)
			  goto wait;
	  }
	  action
	  {
		  OUTPUT_TEXT("finishedPositiononfield");
		  WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(-theRobotPose.rotation,-(theRobotPose.translation.x()+750),-(theRobotPose.translation.y()+750)));
		  LookLeftandRight();
	  }
  }
  /**等待裁判开始比赛*/
  state(wait)
  {
	  action
	  {
		   OUTPUT_TEXT("finishedWait");
		   Stand();
		   LookLeftandRight();
	  }
  }
}
