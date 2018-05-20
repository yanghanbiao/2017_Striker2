option(WalkToDestination, (const Pose2f&) speed, (const Pose2f&) target)
{
	/**定义静态变量*/
	static unsigned int time = 0;
	static bool left = false;
	static bool right = false;
	
  common_transition
  {
	  /**如果左边手臂接触障碍，则赋值left为true，赋值time为当前的系统时间*/
	  if(theArmContactModel.status[Arms::left].contact == true)
	  {
		  left = true;
		  time = theFrameInfo.time;//系统时间
	  }
      /**如果右边手臂接触障碍，则赋值right为true，赋值tim为当前系统时间*/
	  if(theArmContactModel.status[Arms::right].contact == true)
	  {
		  right = true;
		  time = theFrameInfo.time;
	  }
	  
	  if((theFrameInfo.time - time) > 5000)//从接触障碍手背后持续五秒，可加特殊条件。
	  {
		  left = false;
		  right = false;
	  }
	  /**没有看到球且左手臂接触障碍物*/
	  if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && left == true)
	  {
		  
		  OUTPUT_TEXT("Arms::left");
		  goto leftArm;
	  }
	  /**同理*/
	  if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut  && right == true)
	  {
		  
		  OUTPUT_TEXT("Arms::right");
		  goto rightArm;
	  }
	  
  }	
  /**初始状态，往前走*/
  initial_state(walk)
  {
    transition
    {
	  /**障碍物在左边，看不到球时肯定要避障，看到球且球的距离大于1200mm，也避障*/
      if (theLibCodeRelease.ObsAtLorR == 1 && (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut || (theLibCodeRelease.timeSinceBallWasSeen < 300.f && theBallModel.estimate.position.norm() > 1200.f)))
		  goto leftObstacle;
	  /**障碍物在右边，看不到球时肯定要避障，看到球且球的距离大于1200mm，也避障*/  
	  if (theLibCodeRelease.ObsAtLorR == -1 && (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut || (theLibCodeRelease.timeSinceBallWasSeen < 300.f && theBallModel.estimate.position.norm() > 1200.f)))
		  goto rightObstacle;	 
	
    }
    action
    {
	  /**调用走路函数*/
      WalkToTarget(speed, target);
    }
  }

  /**左边有障碍物就往右边走一小段距离*/
  state(leftObstacle)
  {
	transition
	{
		/**若状态时间大于1500ms，则跳进walk*/
		if (state_time > 1500.f)
			goto walk;
	}  
    action
    {
		OUTPUT_TEXT("leftObstacle");		
		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(0.f, 0.f, -300.f));	
    }
  }
  
  /**右边有障碍物就往左边走一小段距离*/
  state(rightObstacle)
  {
	transition
	{
		if (state_time > 1500.f)
			goto walk;
	}  
    action
    {
		OUTPUT_TEXT("rightObstacle");
		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(0.f, 0.f, 300.f));
	
    }
  }
  
  /**左边有障碍物就往右边走一小段距离*/
   state(leftArm)
  {
	transition
	{
		/**若状态时间大于3000ms或者左边手臂没有接触障碍，则跳进walk*/
		if (state_time > 3000.f || left == false)
			goto walk;
	}  
    action
    {
		OUTPUT_TEXT("leftArm");		
		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(0.f, 0.f, -300.f));	
    }
  }
  
  /**右边有障碍物就往左边走一小段距离*/
  state(rightArm)
  {
	transition
	{
		/**若状态时间大于3000ms或者右边手臂没有接触障碍，则跳进walk*/
		if (state_time > 3000.f || right == false)
			goto walk;
	}  
    action
    {
		OUTPUT_TEXT("rightArm");
		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(0.f, 0.f, 300.f));
	
    }
  }
  
}
