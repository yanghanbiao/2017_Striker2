/** Handle penalty state (and the actions to be performed after leaving the penalty state).
 *   This option is one level higher than the main game state handling as penalties
 *   might occur in most game states. */
option(HandlePenaltyState)
{
  /** By default, the robot is not penalized and plays soccer according to the current game state.
  The chestbutton has to be pushed AND released to manually penalize a robot */
  initial_state(notPenalized)
  {
    transition
    {
      if(theRobotInfo.penalty != PENALTY_NONE)
        goto penalized;
    }
    action
    {
      HandleGameState();
    }
  }
  /** In case of any penalty, the robots stands still. */
  state(penalized)
  {
    transition
    {
      if(theRobotInfo.penalty == PENALTY_NONE)
        goto preUnpenalize;
    }
    action
    {
      PlaySound("penalized.wav");
      LookForward();
      SpecialAction(SpecialActionRequest::standHigh);
    }
  }

 state(preUnpenalize)
  {
    transition
    {
      if(theRobotInfo.penalty != PENALTY_NONE)
        goto penalized;
      else if(theGameInfo.state == STATE_INITIAL || state_time > 500)
	  {
		  switch(theRobotInfo.number)
		{
			case 1: goto SugoToField;	//KgoToField
			case 2: goto SugoToField;	//SugoToField
			case 3: goto SugoToField;	//D1goToField
			case 4: goto SugoToField;	//DgoToField
			case 5: goto SugoToField;	//SugoToField	notPenalized
			case 6: goto KgoToField;
			case 7: goto StgoToField;
			case 8: goto D1goToField;
			case 9: goto DgoToField;
			case 10: goto SugoToField;
		}			
	  }
        //goto goToField;
    }
      action
    {
      PlaySound("notPenalized.wav");
      LookForward();
      Stand();
    }
  }
  state(KgoToField)
  {
	  transition
	  {
		  if((std::abs(theRobotPose.rotation) < 5_deg && std::abs(theRobotPose.translation.y()) < 100 && std::abs((theRobotPose.translation.x() + 4200)) <100) || theLibCodeRelease.movedBall==true)  //给角度（-5,+5）的误差和坐标（-10cm，+10cm）的误差允许范围，后者libCodeRelease.movedBall == true表示看到球了，走到球
			goto notPenalized;
	  }
	  action
	  {
		  LookForward();
		  WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(-theRobotPose.rotation,-(theRobotPose.translation.x() + 4200),-theRobotPose.translation.y()));//第三个参数表示机器人坐标系下的y轴，-（theRobotPose.translation.x()+4200）表示机器人走到全局坐标系下x轴的-4200处，第四个参数-theRobotPose.translation.y()表示-theRobotPose.translation.y()+0即走到全局坐标系下y轴的0处。
	  }
  }
  state(StgoToField)
  {
	  transition
	  {
		  if((std::abs(theRobotPose.rotation) < 5_deg && std::abs(theRobotPose.translation.y()) < 100 && std::abs((theRobotPose.translation.x())) <100) || theLibCodeRelease.movedBall == true || theLibCodeRelease.timeSinceBallWasSeen < 300)  
			goto notPenalized;
	  }
	  action
	  {
		  LookForward();
		  WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(-theRobotPose.rotation,-(theRobotPose.translation.x()),-theRobotPose.translation.y()));
	  }
  }
  state(D1goToField)
  {
	  transition
	  {
		  if((std::abs(theRobotPose.rotation) < 5_deg && std::abs(theRobotPose.translation.y()) < 100 && std::abs((theRobotPose.translation.x() + 2000)) <100) || theLibCodeRelease.movedBall == true || theLibCodeRelease.timeSinceBallWasSeen < 300)  
			goto notPenalized;
	  }
	  action
	  {
		  LookForward();
		  WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(-theRobotPose.rotation,-(theRobotPose.translation.x() +2000),-theRobotPose.translation.y()));
	  }
  }
  state(DgoToField)
  {
	  transition
	  {
		  if((std::abs(theRobotPose.rotation) < 5_deg && std::abs(theRobotPose.translation.y()) < 100 && std::abs((theRobotPose.translation.x() + 2900)) <100) || theLibCodeRelease.movedBall == true || theLibCodeRelease.timeSinceBallWasSeen < 300)  
			goto notPenalized;
	  }
	  action
	  {
		  LookForward();
		  WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(-theRobotPose.rotation,-(theRobotPose.translation.x() +2900),-theRobotPose.translation.y()));
	  }
  }
  /**striker2的犯规上场，犯规上场需要考虑的问题是自定位可能是不准的，所以直接先直直的往前走10秒或者走到y为1000米的位置；因为犯规上场是从两边上场，所以theRobotPose.translation.y()加绝对值；设置当前状态*/
  state(SugoToField)
  {
	  transition
	  {
//		  if((std::abs(theRobotPose.rotation) < 5_deg && std::abs(theRobotPose.translation.y()) < 100 && std::abs((theRobotPose.translation.x()-3000)) <100) || theLibCodeRelease.movedBall == true || theLibCodeRelease.timeSinceBallWasSeen < 300)  
//			goto notPenalized;
		  if(std::abs(theRobotPose.translation.y()) < 1000.f || state_time > 10000.f)
			  goto notPenalized;
	  }
	  action
	  {
//		  LookForward();WalkToDestination
//		  WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(-theRobotPose.rotation,-(theRobotPose.translation.x()-3000),-theRobotPose.translation.y()));
		  OUTPUT_TEXT("SugoToField");
//		  WalkToTarget(Pose2f(30.f, 30.f, 30.f),Pose2f(0.f,std::abs(theRobotPose.translation.y()),0.f));
		  WalkToDestination(Pose2f(30.f, 30.f, 30.f),Pose2f(0.f,std::abs(theRobotPose.translation.y()),0.f));
		  LookLeftandRight();
		  Activity(BehaviorStatus::havepenalty);
//		  theHeadControlMode = HeadControl::lookForward;		  
	  }
  }
}