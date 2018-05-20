option(StrikerReady)
{
  /* position has been reached -> stand and wait */
  /**机器人从点(-1000,-3000)附近上场，为了自定位准确，在站起来之前朝向中心圆调整自定位，上场前机器人需要先调整角度正对场内*/
 initial_state(turnForward)
  {
	  transition
	  {
		  /**调整角度正对场内，方向垂直于边线，若小于10°，则跳进stand*/
		  if(std::abs(theRobotPose.rotation - pi/2) < 10_deg)
			  goto stand;
	  }
    action
    {
		/**转个角度，往右看中心圆*/
		OUTPUT_TEXT("turnForward");
		WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(-(theRobotPose.rotation - pi/2), 0.f, 0.f));
		LookRight();

    } 
 } 
  
  /**正面对场内，走到点(-750,-750)*/ 
  state(stand)
  {
	  transition
	  {
		  /**若走到全局坐标y=-750范围100左右，或者状态时间超过10000ms，则跳进turn*/
		  if(std::abs(theRobotPose.translation.y() + 750.f) < 100 || state_time > 10000.f)
			  goto turn;
	  }
    action
    {
	  /**速度百分之三十，注意，用WalkToTarget函数走到一个点，参数是相对坐标，除非是0，一般不能直接一个常数*/
	  OUTPUT_TEXT("stand");
		
	  WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(-(theRobotPose.rotation - pi/2), -(theRobotPose.translation.y() + 750.f), (theRobotPose.translation.x() + 750)));
	  LookRight();

    }
  }
  /**正面对敌方球门，往前看*/
  state(turn)
  {
	  transition
	  {
			  /**若面对敌方球门小于5°，则跳进positiononfield*/
			  if(std::abs(theRobotPose.rotation) < 5_deg)
				  goto positiononfield;
	  }
	  action
	  {
		  WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(-theRobotPose.rotation, 0.f, 0.f));
		  LookForward();
	  }
  }
  /**正面对敌方球门，再次调整位置，处于点(-750,-750)，向左向右看*/
  state(positiononfield)
  {
	  transition
	  {
		  if((std::abs(theRobotPose.translation.x()+750) < 100 &&std::abs(theRobotPose.translation.y() + 750.f) < 10 && std::abs(theRobotPose.rotation) < 5_deg) || state_time > 10000.f)
			  goto wait;
	  }
	  action
	  {
		  WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(-theRobotPose.rotation,-(theRobotPose.translation.x()+750),-(theRobotPose.translation.y()+750)));
		  LookLeftandRight();
		  OUTPUT_TEXT("positiononfield");
	  }
  }
  /**站着，向左向右看，等待开始*/
  state(wait)
  {
	  action
	  {
		   Stand();
		   LookLeftandRight();
		   OUTPUT_TEXT("wait");
	  }
  }
}
