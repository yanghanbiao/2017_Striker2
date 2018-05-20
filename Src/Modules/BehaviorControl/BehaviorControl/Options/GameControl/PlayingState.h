option(PlayingState)
{
		        /**调用Demo(),这里可以加出界判断，但是出界判断严重依赖自定位的准确成都，在比赛中为了保证机器人稳定实行，注释了出界判断*/
  initial_state(demo)
  {
	transition
	{
		/**看到球且球出界的情况下的出界判断*/
//		if((std::abs(theLibCodeRelease.globalballx) > 4700 || std::abs(theLibCodeRelease.globalbally) > 3200) && theLibCodeRelease.timeSinceBallWasSeen < 300)
//			goto outFieldBall;
		/**看不到球且机器人x出界情况下的出界判断*/
//		if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && std::abs(theRobotPose.translation.x()) > 4300.f )
//			goto adjustx;
		/**看不到球且机器人y出界的情况下的出界判断*/
//		if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && std::abs(theRobotPose.translation.y()) > 2800.f)
//			goto adjusty;	
			
	}  
    action
    {
//      OUTPUT_TEXT("theLibCodeRelease.timeSinceBallWasSeen");
//	  OUTPUT_TEXT(theLibCodeRelease.timeSinceBallWasSeen);
//	  OUTPUT_TEXT(theBehaviorParameters.ballNotSeenTimeOut);
//	  OUTPUT_TEXT("//");
	  /**调用函数*/
	  Demo();
    }
  }
  /**transition:丢球后或者状态时间超过4000ms，跳转到demo;action:站着，盯着球看*/
 state(outFieldBall)
  {
	transition
	{
		/**如果丢球或者状态时间超过4000ms，跳进demo状态*/
		if (state_time >4000.f || theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
			goto demo;
	}  
    action
    {
		OUTPUT_TEXT("Playing_outFieldBall");
		/**站着*/
		Stand();
		/**头盯着球*/
		FollowTheTarget(theBallModel.estimate.position.angle());
    }
  }
  /**先调整机器人的角度来正面对敌方球门。备注：看到球的条件可能实现不理想，假如调整的角度还不合适时就看到球，可能会乱走*/
  state(adjustx)
  {
	transition
	{
		/**如果状态时间超过4000ms，或者机器人相对全局坐标的角度的绝对值小于20度，或者机器人看到球，则跳进outFieldPosex状态*/
		if (state_time >4000.f || std::abs(theRobotPose.rotation) < 20_deg || theLibCodeRelease.timeSinceBallWasSeen < 600)
			goto outFieldPosex;
	}  
    action
    {
		OUTPUT_TEXT("Playing_adjustx");
		/**调整角度*/
		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, 0.f, 0.f));
		/**往前看*/
		LookForward();
	
    }
  }
//  
  /**同上，类似*/
  state(adjusty)
  {
	transition
	{
		if (state_time >4000.f || std::abs(theRobotPose.rotation) < 20_deg || theLibCodeRelease.timeSinceBallWasSeen < 600)
			goto outFieldPosey;
	}  
    action
    {
		OUTPUT_TEXT("Playing_adjusty");
		/**调整角度*/
		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, 0.f, 0.f));
		/**往前看*/
		LookForward();
    }
  }
// /**往界内走*/
  state(outFieldPosex)
  {
	transition
	{	
		/**假如状态时间大于4000ms，或者看到球，或者机器人的全局x坐标小于4000mm，则跳进demo状态*/
		if (state_time >4000.f || theLibCodeRelease.timeSinceBallWasSeen < 600 || std::abs(theRobotPose.translation.x()) < 4000.f)
			goto demo;
	}  
    action
    {
		OUTPUT_TEXT("Playing_outFieldPosex");
		/**速度百分之三十，机器人正面对敌方球门，走回界内*/
		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, -theRobotPose.translation.x(), 0.f));
		/**左右看*/
		LookLeftandRight();
    }
  }
// /**同上，类似，往界内走*/
  state(outFieldPosey)
  {
	transition
	{
		if (state_time >4000.f || theLibCodeRelease.timeSinceBallWasSeen < 600 || std::abs(theRobotPose.translation.y()) < 2500.f)
			goto demo;
	}  
    action
    {
		OUTPUT_TEXT("Playing_outFieldPosey");
		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, 0.f, -theRobotPose.translation.y()));
		LookLeftandRight();
    }
  }

}