/** A test striker option without common decision */
option(Striker)
{
  
  static bool flag = false;
  static unsigned int time = 0;
//  static bool left = false;
//  static bool right = false;
//  static float linshibnorm;
//  static float linshitnorm;
  /**common_transition与其他state同时判断*/
  common_transition
  {
	  /**若左手臂或者右手臂判断有接触，则将flag赋值为true,将系统时间赋值到time*/
	  if(theArmContactModel.status[Arms::left].contact == true || theArmContactModel.status[Arms::right].contact == true)
		  
	  {
		  flag = true;		  		  
		  time = theFrameInfo.time;//theFrameInfo.time为全局的时间，从机器人开始运行代码从0开始，正常的话半场十分钟总共600000毫秒。
	  }
	   
//      if(theArmContactModel.status[Arms::left].contact == true)
//	  {
//		  left = true;
//	  }
//      
//	  if(theArmContactModel.status[Arms::right].contact == true)
//	  {
//		  right = true;
//	  }
	  /**若手臂接触障碍，则将左手臂和右手臂同时收起*/
	  if(flag == true)
	  {
		
		  theArmMotionRequest.armMotion[Arms::left] = theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
		  theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back;
	       
	  }
	  /**从接触障碍手背后持续五秒，可加特殊条件*/
	  if((theFrameInfo.time - time) > 5000)
	  {
		  flag = false;
//		  left = false;
//		  right = false;
	  }
	  /**若手臂没有接触障碍，则把手放下*/
	  if(flag == false)
	  {
		  
		  theArmMotionRequest.armMotion[Arms::left] = theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::none;
	      theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::useDefault;
	   
	  }
	  
//	  if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && (left == true || (theLibCodeRelease.ObsAtLorR == 1 && state_time <3000.f)))
//	  {
//		  
//		  OUTPUT_TEXT("Arms::left");
//		  goto leftAvoid;
//	  }
//	  
//	  if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut  && (right == true || (theLibCodeRelease.ObsAtLorR == -1 && state_time <3000.f)))
//	  {
//		  
//		  OUTPUT_TEXT("Arms::right");
//		  goto rightAvoid;
//	  }
//	  
  }


//
// state(rightAvoid)
//  {
//	transition
//	{
//		if(theLibCodeRelease.timeSinceBallWasSeen < 300)
//			goto turnToBall;
//			
//		if (state_time > 3000.f)
//			goto searchForBall2;
//			
//		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
//			goto turnToTeamBall;
//	 		
//		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
//			goto turnToTeamBall2;	
//	}  
//    action
//    {
//		OUTPUT_TEXT("rightAvoid");
//		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(0.f, 0.f, 300.f));
//	
//    }
//  }
////
// state(leftAvoid)
//  {
//	transition
//	{
//		if(theLibCodeRelease.timeSinceBallWasSeen < 300)
//			goto turnToBall;
//		
//		if (state_time > 3000.f)
//			goto searchForBall2;
//			
//		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
//			goto turnToTeamBall;
//	 		
//		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
//			goto turnToTeamBall2;	
//	}  
//    action
//    {
//		OUTPUT_TEXT("leftAvoid");
//		
//		WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(0.f, 0.f, -300.f));
//	
//    }
//  }
	
	
  /**初始状态*/
  initial_state(start)
  {
    transition
    {
	  /**若我方发球，则直接跳转进searchForBall2*/
      if(theGameInfo.kickOffTeam == theOwnTeamInfo.teamNumber)
			goto searchForBall2;
	  /**若球移动或者状态时间大于10000ms或者犯规状态后，则跳转进searchForBall2*/		
	  else if(theLibCodeRelease.movedBall == true || state_time >10000 || theBehaviorStatus.activity == 18)		
		     goto searchForBall2;

    }
    action
    {
	  OUTPUT_TEXT("start");
      LookForward();
      Stand();
    }
  }
  

  /**机器人看到球后，先转向球，身体正对球*/
  state(turnToBall)
  {
    transition
    {
	   /**若自己和队友同时看到球且队友离球更近且球离自己大于200m，则跳转waitForBall*/
	  if(theLibCodeRelease.teamBallDistance == true && theTeamBallModel.isValid == true && theTeamBallModel.contributors ==3 && theBallModel.estimate.position.norm() >= 200.f)
			goto waitForBall;	
		/**若没有看到球，则先走到最后看到球的位置找球*/
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
			goto searchForBall;
	   /**若机器人面对球的角度小于5°且和队友同时看到球且球离自己更近或者只有自己一个人看到球或者球离自己小于500m，则走向球*/
      if(std::abs(theBallModel.estimate.position.angle()) < 5_deg && ((theTeamBallModel.isValid == true && theLibCodeRelease.teamBallDistance == false && theTeamBallModel.contributors ==3) || theTeamBallModel.contributors ==0 || theBallModel.estimate.position.norm() < 500.f))
			goto walkToBall;		
	  
	  
    } 
    action
    {
	  
	  OUTPUT_TEXT("turnToBall");
//      OUTPUT_TEXT("teamBallDistance");
//	  OUTPUT_TEXT(theLibCodeRelease.teamBallDistance);
//	  OUTPUT_TEXT("contributors");
//	  OUTPUT_TEXT(theTeamBallModel.contributors);
//	  OUTPUT_TEXT("norm");WalkToDestination
//	  OUTPUT_TEXT(theBallModel.estimate.position.norm());
	  OUTPUT_TEXT("//");
      //LookForward();
      WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));//转相应的角度,面对球
//	  FollowTheTarget(theBallModel.estimate.position.angle());
	  LookForward();
    }
  }
  
	/**等球，用于自己和队友同时看到球且队友离球更近的情况，等球在看到球的情况下*/
	state(waitForBall)
  {
	  transition
	  {   
		  /**若看不见球，则找球*/
		  if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut || theTeamBallModel.isValid == false)
              goto searchForBall2;
		  /**若球动了或者队友离球更远或者只有自己一个人看到球或者机器人走到相应等球的位置或者状态时间大于5000ms或者机器人全局坐标横坐标大于3500m,则跳进turnToBall*/
		  if(theLibCodeRelease.movedBall == true || theLibCodeRelease.teamBallDistance == false || (theTeamBallModel.contributors ==0 && theTeamBallModel.isValid == true) || std::abs(theRobotPose.translation.x() - theLibCodeRelease.xx) < 500.f || std::abs(theRobotPose.translation.y() - theLibCodeRelease.ballPosition.y()) > 800 ||state_time > 5000.f || theRobotPose.translation.x() > 3500.f)
			  goto turnToBall;
		  /**若只有队友看到球自己没有看到球且球在自己的前方，则跳进turnToTeamBall*/	  
		  if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			  goto turnToTeamBall;
		   /**若只有队友看到球自己没有看到球且球在自己的后方，则跳进turnToTeamBall2*/
		  if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			  goto turnToTeamBall2;
	  }
	  action
	  {
			OUTPUT_TEXT("waitForBall2222");
//			OUTPUT_TEXT("contributors");
//			OUTPUT_TEXT(theTeamBallModel.contributors);
//			OUTPUT_TEXT("teamBallDistance");
//			OUTPUT_TEXT(theLibCodeRelease.teamBallDistance);
//			OUTPUT_TEXT("norm");
//			OUTPUT_TEXT(theBallModel.estimate.position.norm());
			OUTPUT_TEXT("//");
			FollowTheTarget(theBallModel.estimate.position.angle());
			WalkToTarget(Pose2f(30.f,30.f,30.f),Pose2f(theBallModel.estimate.position.angle(),theLibCodeRelease.ballx2,theLibCodeRelease.bally2));//原地转向球	
	  }
  }
  /**走向球*/
  state(walkToBall)
  {
    transition
    {
	  /**若自己和队友同时看到球且队友离球更近且球离自己大于200m，则跳转waitForBall*/
	  if(theLibCodeRelease.teamBallDistance == true && theTeamBallModel.isValid == true && theTeamBallModel.contributors ==3 && theBallModel.estimate.position.norm() >= 200.f)
				goto waitForBall;	
	  /**若球丢了，则先到最后看到球的位置找球*/
      if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
	   /**若球动了，则转向球turnToBall*/
	  if (theLibCodeRelease.movedBall == true)
				goto turnToBall;	 
	   /**若走到离球距离小于600mm，则面对球门*/
	  if (theBallModel.estimate.position.norm() < 600.f)//机器人和球之间的距离
				goto alignToGoal;
				
	  			
		
    }
    action
    {
	  OUTPUT_TEXT("walkToBall");
//WalkToDestination	  
//	  OUTPUT_TEXT("teamBallDistance");
//	  OUTPUT_TEXT(theLibCodeRelease.teamBallDistance);
//	  OUTPUT_TEXT("contributors");
//	  OUTPUT_TEXT(theTeamBallModel.contributors);
//	  OUTPUT_TEXT("norm");
//	  OUTPUT_TEXT(theBallModel.estimate.position.norm());
	  OUTPUT_TEXT("//");
      FollowTheTarget(theBallModel.estimate.position.angle());	  
//      WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(theBallModel.estimate.position.angle(),theBallModel.estimate.position.x()-300.f,theBallModel.estimate.position.y()+30.f));//球在机器人的坐标
	  WalkToTarget(Pose2f(15.f, 15.f, 15.f), Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 35.f));
    }
  }
  
  /**机器人走到球的附近后，一般不会面对球门的，机器人在控制球后，首要任务应该是立即面对球门，产生射门威胁，而面对球门需要分几种情况，最简单就是机器人正面对球门，这只需要给一个机器人相对于球门中心的角度就可以调整过来，其次是机器人背对球门，这种情况需要先机器人绕着球先转圈，转到正面；最后一种情况就是虽然机器人也是面对敌方半场，但是机器人相对于球门的角度绝对值太大，也同样需要机器人先绕着球转圈，转到正面合适的角度*/
  state(alignToGoal)
  {
    transition
    {
	  /**若球丢了，则先走到最后丢球的位置找球*/
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
			goto searchForBall;
	  /**若球动了或者球离自己的距离大于600mm，很可能自己碰到球或者球被抢走了，导致球已经不在原来的位置了，则需要转向球*/
	  if (theLibCodeRelease.movedBall == true || (std::abs(theBallModel.estimate.position.norm()) > 600.f))
			goto turnToBall;
	  /**若机器人相对于球门中心点的角度小于15°且球离机器人距离小于400mm，调整左右距离合适，没有发现障碍物，即使发现障碍物，机器人的x坐标大于3800mm，则再进一步调整角度和踢球位置*/
	  if (std::abs(theLibCodeRelease.angleToGoal) < 15_deg && theBallModel.estimate.position.norm() < 400.f && std::abs(theBallModel.estimate.position.y() + 35.f)<100.f && (theLibCodeRelease.obstacleRobot == false || (theLibCodeRelease.obstacleRobot == true && theRobotPose.translation.x() >= 3800.f)))
		 //angletoGoal是对方球门相对于自己的角度 && 球相对于自己的y坐标在（-200，+200）
			goto alignBehindBall;
	  /**同上类似，不同的是若检测到障碍物，则相应根据障碍物调整踢球角度和踢球位置*/
	  if (std::abs(theLibCodeRelease.angleToGoal) < 15_deg && theBallModel.estimate.position.norm() < 400.f && std::abs(theBallModel.estimate.position.y() + 35.f)<100.f  && theLibCodeRelease.obstacleRobot == true && theRobotPose.translation.x() < 3800.f)  //距离球门比较远的话，再识别机器人，特别靠近禁区就可以直接射门踢死角
			goto alignBehindRobot;	


    }
    action
    {
      LookForward();
	  OUTPUT_TEXT("alignToGoal");
	  OUTPUT_TEXT("theLibCodeRelease.obstacleRobot");
	  OUTPUT_TEXT(theLibCodeRelease.obstacleRobot);
	  OUTPUT_TEXT("//");
	  /**若机器人朝向背对敌方球门*/
	  if(theRobotPose.rotation > pi/2 ||theRobotPose.rotation < -pi/2)
		  {
			 /**在球门的左边*/ 
			if( theLibCodeRelease.angleToGoal <0.f)
			{
				
				  OUTPUT_TEXT("leftdoor");
				  WalkAtRelativeSpeed(Pose2f(-30.f, 0.f, 50.f));//顺时针
				  FollowTheTarget(theBallModel.estimate.position.angle());
			}
			else
			{
				   OUTPUT_TEXT("rightdoor");
				   WalkAtRelativeSpeed(Pose2f(30.f, 0.f, -50.f));//逆时针
				   FollowTheTarget(theBallModel.estimate.position.angle());//头跟着球
				   
			}
		  }
	   /**机器人面向对方球门*/
	  else 
          {	   /**若机器人朝向面对球门的角度太大，此时调整位置射门时，可能会碰到球，需要再次调整*/
			  if(std::abs(theLibCodeRelease.angleToGoal)>pi/4)
			 {
//				 OUTPUT_TEXT("std::abs(theLibCodeRelease.angleToGoal)>pi/4");
				 OUTPUT_TEXT(std::abs(theLibCodeRelease.angleToGoal));
				 /**angleToGoal大于pi/4这种情况一般是当球在y小于零的区域及机器人在球的左边，这是可以设想角度太大，直接走到射门位置的话，一般会碰到球*/
				 if(theLibCodeRelease.angleToGoal>pi/4)
				 {					
//						OUTPUT_TEXT("pi/4");
						WalkAtRelativeSpeed(Pose2f(30.f, 0.f, -50.f));//逆时针相对于一点绕圆转
						FollowTheTarget(theBallModel.estimate.position.angle());
						
				 }
			 /**同上，类似*/	 
			 else
				{					
//						OUTPUT_TEXT("-pi/4");
						WalkAtRelativeSpeed(Pose2f(-30.f, 0.f, 50.f));//顺时针相对于一点绕圆转
						FollowTheTarget(theBallModel.estimate.position.angle());
//						OUTPUT_TEXT(theLibCodeRelease.globalbally);					
				}
			 }
			 /**机器人相对于球门的角度合适，直接调整相应角度和踢球位置*/
			 else
			 {
			  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 220.f, theBallModel.estimate.position.y()+35.f));//调整踢球角度
			  FollowTheTarget(theBallModel.estimate.position.angle());
			  
			 }
		  }
	}
  }
  
  /**调整踢球角度和踢球位置，机器人设置右脚踢球，为了使得踢球合适，球的中心应在右脚中心附近，球和脚的前后距离也应该合适，关于踢球的角度，在这个状态下的话就是单纯的踢死角，在y>0时往球门y=-300位置踢，在y<0时往球门y=300位置踢*/
  state(alignBehindBall)
  {
    transition
    {
	  /**若球丢了，则走到最后看到球的位置找球*/
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
			goto searchForBall;
	  /**若球动了，或者球的距离大于1000mm，则转向球*/
	  if(theLibCodeRelease.movedBall == true || std::abs(theBallModel.estimate.position.norm()) > 1000.f )      
			goto turnToBall;//球动,球距离自己太远      
	  /**若球相对于机器人的位置y在区间[-42,-28]，x在区间[180,210]，或者角度合适，则踢球*/
	  if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -42.f,-28.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 180.f, 210.f)&& (std::abs(theLibCodeRelease.angleToGoal)<5_deg||std::abs(theLibCodeRelease.angle1)<5_deg||std::abs(theLibCodeRelease.angle2)<5_deg))
			goto kick;//调整到合适位置后,踢球		
    }
    action
    {
      LookForward();
	  OUTPUT_TEXT("alignBehindBall");
	 
	  FollowTheTarget(theBallModel.estimate.position.angle());
	 /**若球离球门的距离太大，则先让机器人带球走*/
     if(theLibCodeRelease.distanceRobotToGoal > 5000.f)//距离大的话,就走着踢
	    WalkToTarget(Pose2f(20.f, 20.f, 20.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 35.f));
	 /**距离小，就采取策略，踢死角*/
	 else
	{		/**球在球场左半部分*/
		   if(theLibCodeRelease.globalbally>0.f)
		  {
			 
			 
			    WalkToTarget(Pose2f(15.f, 15.f, 15.f), Pose2f(theLibCodeRelease.angle2, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 35.f));
				OUTPUT_TEXT("left");
//				OUTPUT_TEXT(theLibCodeRelease.globalbally);
			 
		  }
		  /**球在右边*/
		  else
		  {
			 
			 
			    WalkToTarget(Pose2f(15.f, 15.f, 15.f), Pose2f(theLibCodeRelease.angle1, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 35.f));
				OUTPUT_TEXT("right");
//				OUTPUT_TEXT(theLibCodeRelease.globalbally);
			 
	     }
	}
	
   }
  }
  /**根据障碍物的位置进行踢球角度的选择，若障碍物和两侧球门柱位置互相做比较，哪边的角度大，就往哪边射门*/
  state(alignBehindRobot)
  {
	  transition
	  {
		/**若机器人看不到球，则走到最后看到球的位置*/  
		if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
			goto searchForBall;
		/**若球动了或者球相对于机器人的距离大于600mm，则转向球*/	
		if(theLibCodeRelease.movedBall == true || std::abs(theBallModel.estimate.position.norm()) > 600.f )
			goto turnToBall;//球动,球距离自己太远
		/**若球相对于机器人的位置y在区间[-42,-28]，x在区间[180,210]，或者角度合适，则踢球*/
		if(state_time > 1500.f||(theLibCodeRelease.between(theBallModel.estimate.position.y(), -42.f,-28.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 180.f, 210.f)&& (std::abs(theLibCodeRelease.angleToGoal)<5_deg||std::abs(theLibCodeRelease.angle1)<5_deg||std::abs(theLibCodeRelease.angle2)<5_deg)))		
			goto kick;//调整到合适位置后,踢球
	  }
	  
	  action
	  {
		  OUTPUT_TEXT("alignBehindRobot");
		  
		  if(theLibCodeRelease.angleKickRobotLeft<theLibCodeRelease.angleKickRobotRight)
				   WalkToTarget(Pose2f(15.f, 15.f, 15.f), Pose2f(theLibCodeRelease.angle2, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 35.f));
		  else
				   WalkToTarget(Pose2f(15.f, 15.f, 15.f), Pose2f(theLibCodeRelease.angle1, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 35.f));
	  }
  }		
  /**踢球的动作，包含两种踢法，走着踢和大脚踢，走着踢的相应距离参数和以上对应*/
  state(kick)
  {
    transition
    {
		/**若球动了，状态时间大于20ms且动作完成，则转向球*/
		if((theLibCodeRelease.movedBall == true) ||(state_time >10.f && action_done)|| state_time>10000.f)
			goto turnToBall;
		/**若球丢了，则走到最后看到球的位置找球*/
		if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
			goto searchForBall;
		/**若状态时间大于3000ms，状态时间大于20ms且动作完成，则跳进开始状态*/	
		if(state_time > 3000 || (state_time > 10 && action_done))//说明已经踢过了
			goto start;
    }
    action
    {
      LookForward();
	  OUTPUT_TEXT("kick");
	  /**球离球门太远，或者角度不合适，走着踢*/
	  if(theLibCodeRelease.distanceRobotToGoal >5000.f||std::abs(theLibCodeRelease.angle1-theLibCodeRelease.angle2)<5_deg)//比较难踢就走着踢
		  InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 60.f, theBallModel.estimate.position.y()+35.f));
	  else 
	    Shoot(KickRequest::greatKickForward);//开大脚
		 
	}
  }
  
  
  /**找球状态，看不到球后，机器人找球*/
    state(searchForBall2)
  {
    transition
    {
	 /**若看到球，则转向球*/	
	 if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
	 /**队友看到球且球相对于机器人在机器人的前方*/		
	 if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 /**队友看到球且球相对于机器人在机器人的后方*/		
	 if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;		
	 /**若机器人全局坐标在球场x区间[0,1000]范围，或者大约在位置(2700,0),则走到中心圆位置*/		
	 if((state_time>20000.f && theRobotPose.translation.x()<1000.f && theRobotPose.translation.x() > 0.f)||(std::abs(theRobotPose.translation.x()-2700.f)<300.f&&std::abs(theRobotPose.translation.y())<300.f&&state_time>18000.f))
			goto backMiddle2;		
	 /**若机器人全局坐标x小于0*/			
	 if(state_time>20000.f && theRobotPose.translation.x()<0.f )
			goto fromOwnFieldBackMiddle2;
	 /**若机器人全局坐标x大于1000mm，则机器人走到2700mm位置*/	
	 if((state_time>20000.f&&theRobotPose.translation.x()>1000.f)/*||(std::abs(theRobotPose.translation.x())<300.f&&std::abs(theRobotPose.translation.y())<300.f&&state_time>18000.f )*/)//找不到球
			goto backyuan2;						
	 /**若后卫和自己有通信联系且后卫犯规，则回到己方半场找球*/		
	 if(state_time > 17000.f && theTeamBallModel.isValid == false && theRobotPose.translation.x() > -1500.f && ((theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000 && theLibCodeRelease.defender1.isPenalized == true) || (theLibCodeRelease.defender2.isPenalized == true && theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000)))
			goto backOwnField2;
	 /**类似同上，不同的是多了-1500这个条件，这是为了让机器人朝向敌方球门*/		
	 if(state_time > 17000.f && theTeamBallModel.isValid == false && theRobotPose.translation.x() < -1500.f && ((theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000 && theLibCodeRelease.defender1.isPenalized == true) || (theLibCodeRelease.defender2.isPenalized == true && theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000)))
			goto ownField2;		
						
    }
    action
    {
		OUTPUT_TEXT("searchForBall2222222");
	  
//		OUTPUT_TEXT("ballPosition.x()");
//		OUTPUT_TEXT(theLibCodeRelease.ballPosition.x());
//		OUTPUT_TEXT(theLibCodeRelease.ballx2);		
//		OUTPUT_TEXT("ballPosition.y()");
//		OUTPUT_TEXT(theLibCodeRelease.ballPosition.y());
//		OUTPUT_TEXT(theLibCodeRelease.bally2);
//		OUTPUT_TEXT("angleToBall");
//		OUTPUT_TEXT(theLibCodeRelease.angleToBall2);
//		OUTPUT_TEXT("teamBallDistance");
//		OUTPUT_TEXT(theLibCodeRelease.teamBallDistance);
		OUTPUT_TEXT("Contributors");
		OUTPUT_TEXT(theTeamBallModel.contributors);
		OUTPUT_TEXT("theTeamBallModel.isValid");
		OUTPUT_TEXT(theTeamBallModel.isValid);
//		OUTPUT_TEXT("defender1.timeWhenLastPacketReceived");
//		OUTPUT_TEXT(theLibCodeRelease.defender1.timeWhenLastPacketReceived);
//		OUTPUT_TEXT("defender2.timeWhenLastPacketReceived");
//		OUTPUT_TEXT(theLibCodeRelease.defender2.timeWhenLastPacketReceived);
//		OUTPUT_TEXT("defender1.isPenalized");
//		OUTPUT_TEXT(theLibCodeRelease.defender1.isPenalized);
//		OUTPUT_TEXT("defender2.isPenalized");
//		OUTPUT_TEXT(theLibCodeRelease.defender2.isPenalized);
		OUTPUT_TEXT("///");
      FixPointRotate();
    }
  }
  
 /**用于当机器人丢球后的找球，机器人走到最后看到球的位置找球，因为通常球都是按照一定轨迹运动的，机器人走到最后看到球的位置再找球的好处是，可以大概猜测球的位置，更容易找球*/  
 state(searchForBall)
  {
    transition
    {
	 /**若看到球，则转向球*/		
	 if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
	 /**若走到最后看到球的位置或者状态时间大于6000ms，则原地找球*/		
	 if(state_time>6000.f||(std::abs(theRobotPose.translation.x()-theBallModel.lastPerception.x())<100.f && std::abs(theRobotPose.translation.y()- theBallModel.lastPerception.y())<100.f))
			goto searchForBall2;
	 
     if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
	 if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;		
		
    }
    action
    {
	  OUTPUT_TEXT("searchForBall");
//      WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(theBallModel.lastPerception.angle(), theBallModel.lastPerception.x(), theBallModel.lastPerception.y()));
     LookLeftandRight();
//	 WalkToDestination
	 WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(theBallModel.lastPerception.angle(), theBallModel.lastPerception.x(), theBallModel.lastPerception.y()));
    }
  } 
  
/**回到中心圆位置附近，左右开始横着找球的开始状态*/      
state(backMiddle)
  {
    transition
    {	
		
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若走到中心圆附近，则转向右，开始左右找球*/	
		if(state_time >8000.f||(std::abs(theRobotPose.translation.x())<300.f&&std::abs(theRobotPose.translation.y())<300.f))
			goto MiddleRight2;

	  
    }
    action
    {  
	  OUTPUT_TEXT("backMiddle");	
//	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi), (theRobotPose.translation.x() ), theRobotPose.translation.y()));
//	  WalkToDestination
	  WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi), (theRobotPose.translation.x() ), theRobotPose.translation.y()));
     LookLeftandRight();
    }
  }
  
  
  /**机器人转向己方球门，这样子是为了机器人从敌方球门回到中心圆的时候是正着走，保证机器人的稳定性*/
  state(backMiddle2)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若角度调整好，面对己方球门，则开始走到中心圆*/	
		if(state_time >8000.f||std::abs(theRobotPose.rotation)>pi*11/12)
			goto backMiddle;

	  
    }
    action
    {  
	  OUTPUT_TEXT("backMiddle222222222");	
	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi), 0.f, 0.f));
//	theMotionRequest=thePathPlanner.plan(Pose2f(-(theRobotPose.rotation-pi),  0.f, 0.f),Pose2f(30.f, 30.f, 30.f),false);
    LookLeftandRight();
    }
  }
 

/**往右边走，边走边左右看*/
state(MiddleRight)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若走到右边相应位置后，则原地找球*/	
		if(state_time >8000.f||(theRobotPose.translation.y()<-1000.f))
			goto searchForBallRight;

	  
    }
    action
    {  
	  OUTPUT_TEXT("MiddleRight");	
//	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation+pi/2), (theRobotPose.translation.y() + 1000.f), 0.f));
//	  WalkToDestination
	 WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation+pi/2), (theRobotPose.translation.y() + 1000.f), 0.f));
     LookLeftandRight();
    }
  }
  
  
  /**转向右边(始终把机器人朝向敌方球门为参考方向)*/
  state(MiddleRight2)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若转向右边，调整角度合适，则开始往右边走*/	
		if(state_time >8000.f||(theRobotPose.rotation> -pi*7/12 && theRobotPose.rotation< -pi*5/12))
			goto MiddleRight;

	  
    }
    action
    {  
	  OUTPUT_TEXT("MiddleRight222222222");	
	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation+pi/2), 0.f, 0.f));
     LookLeftandRight();
    }
  }
 
 
 /**往左边走*/
state(MiddleLeft)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若走到相应位置，则原地找球*/	
		if(state_time >14000.f||(theRobotPose.translation.y() >1200.f))
			goto searchForBallLeft;

	  
    }
    action
    {  
	  OUTPUT_TEXT("MiddleLeft");	
//	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi/2), -(theRobotPose.translation.y() - 1000.f), 0.f));
//	  WalkToDestination
	 WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi/2), -(theRobotPose.translation.y() - 1000.f), 0.f));
     LookLeftandRight();
    }
  }
  
  
  /**往左边转身*/
  state(MiddleLeft2)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若往左边转身合适角度，则往左边走*/	
		if(state_time >8000.f||(theRobotPose.rotation> pi*5/12 && theRobotPose.rotation< pi*7/12))
			goto MiddleLeft;

	  
    }
    action
    {  
	  OUTPUT_TEXT("MiddleLeft222222222");	
	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi/2), 0.f, 0.f));
     LookLeftandRight();
    }
  } 
 
 
/**走到中心圆位置*/ 
state(Middle)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若走到中心圆附近，则原地找球，再做相应找球决定*/	
		if(state_time >8000.f || std::abs(theRobotPose.translation.y())<300.f )
			goto searchForBall2;

	  
    }
    action
    {  
	  OUTPUT_TEXT("Middle");	
//	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation+pi/2), theRobotPose.translation.y(), 0.f));
//	  WalkToDestination
	  WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation+pi/2), theRobotPose.translation.y(), 0.f));
     LookLeftandRight();
    }
  }
  
  
  /**转身面对中心圆位置*/
  state(Middle2)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若转身角度调整合适，则走到中心圆位置*/	
		if(state_time >8000.f||(theRobotPose.rotation> -pi*7/12 && theRobotPose.rotation< -pi*5/12))
			goto Middle;

	  
    }
    action
    {  
	  OUTPUT_TEXT("Middle222222222");	
	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation+pi/2), 0.f, 0.f));
     LookLeftandRight();
    }
  }
  
/**从己方半场走到中心圆附近*/
state(fromOwnFieldBackMiddle)
  {
     transition
     {
         if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			  goto turnToBall;
			  
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	  
		/**若走到中心圆附近，则开始左右找球*/	  
		 if(std::abs(theRobotPose.translation.x() - 1200.f ) < 300.f&&std::abs(theRobotPose.translation.y()) < 300.f)
              goto MiddleRight2;
			  
		 if(state_time > 12000.f)
			  goto searchForBall2;

		 
     }
	 
     
     action
     {
		 OUTPUT_TEXT("fromOwnFieldBackMiddle");
//         WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, -(theRobotPose.translation.x() - 1200.f ), -theRobotPose.translation.y()));
//	  WalkToDestination		 
		 WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, -(theRobotPose.translation.x() - 1200.f ), -theRobotPose.translation.y()));
         LookLeftandRight();
     }
  }
  
  /**实行这个状态的前提是机器人在己方球门，转身面对敌方球门，更好的走路*/
  state(fromOwnFieldBackMiddle2)
  {
     transition
     {
         if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			  goto turnToBall;
			  
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;  
		 /**若转向敌方球门调整角度合适，则走向中心圆附近*/	  
		 if(state_time >8000.f || std::abs(theRobotPose.rotation)<pi/12)
              goto fromOwnFieldBackMiddle;

		 
     }
     action
     {
		 OUTPUT_TEXT("fromOwnFieldBackMiddle222");
         WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation,0.f, 0.f));
         LookLeftandRight();
     }
  }
  
/**原地找球，用在左右走的右边这个点*/
state(searchForBallRight)
  {
     transition
     {
         if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			  goto turnToBall;
			  
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	  
			  
		 if(state_time >8000.f)
              goto MiddleLeft2;

		 
     }
	 
     
     action
     {
		 OUTPUT_TEXT("searchForBallRight");
         FixPointRotate();
     }
  }
/**原地找球，用在左右走的左边这个点*/  
state(searchForBallLeft)
  {
     transition
     {
         if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			  goto turnToBall;
			  
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	  
			  
		 if(state_time >8000.f)
              goto Middle2;

		 
     }
	 
     
     action
     {
		 OUTPUT_TEXT("searchForBallLeft");
         FixPointRotate();
     }
  }  
  
  
 
 /**走到2700位置找球*/ 
state(backyuan)
  {
     transition
     {
         if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			  goto turnToBall;
			  
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	  
		 /**若找不到球，走到相应位置，则原地找球做决定*/	  
		 if(state_time >8000.f||(std::abs(theRobotPose.translation.x() - 2500.f)<300.f&&std::abs(theRobotPose.translation.y())<300.f))
              goto searchForBall2;

		 
     }
	 
     
     action
     {
		 OUTPUT_TEXT("backyuan");
//         WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, -(theRobotPose.translation.x() - 2500.f), -theRobotPose.translation.y()));
//	  WalkToDestination
		WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, -(theRobotPose.translation.x() - 2500.f), -theRobotPose.translation.y()));
         LookLeftandRight();
     }
  }
  
  /**走到2700位置前，先面对敌方球门*/
  state(backyuan2)
  {
     transition
     {
         if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			  goto turnToBall;
			  
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	  
		 /**若面对敌方球门角度合适，则走到2700位置*/	  
		 if(state_time >8000.f || std::abs(theRobotPose.rotation)<pi/12)
              goto backyuan;

				 
     }
     action
     {
		 OUTPUT_TEXT("backyuan22222222");
         WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation,0.f, 0.f));
         LookLeftandRight();
     }
  }
  
  
  /**队友看到球，且球相对于自己是在后面，先原地转身，让球相对于自己的前方，注意此时自己是看不到球的，在队友和自己之间肯定优先选择自己，但是当自己看不到球，就要依赖与队友传过来球的位置*/
  state(turnToTeamBall2)
  {
    transition
    {
	  if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
				goto turnToBall;
	  /**若全队没人看到球，则立即原地找球*/
      if(state_time >10000.f || theTeamBallModel.isValid == false)
				goto searchForBall2;	  	
	  /**若刚好朝向球或者刚好转一个半圆，则走向队友传过来的球的位置*/
	  if(std::abs(theLibCodeRelease.angleToBall) < 30_deg || state_time > 4000.f)
				goto walkToTeamBall;
	  
    } 
    action
    {
	  OUTPUT_TEXT("turnToTeamBall22222");
//	  OUTPUT_TEXT("angleToBall");
//	  OUTPUT_TEXT(theLibCodeRelease.angleToBall2);
//	  OUTPUT_TEXT("teamBallDistance");
//	  OUTPUT_TEXT(theLibCodeRelease.teamBallDistance);
	  OUTPUT_TEXT("theTeamBallModel.isValid");
	  OUTPUT_TEXT(theTeamBallModel.isValid);
//	  OUTPUT_TEXT(theLibCodeRelease.angleToBall2);
//	  OUTPUT_TEXT(theLibCodeRelease.ballx2);
//	  OUTPUT_TEXT(theLibCodeRelease.bally2);
	  OUTPUT_TEXT("//");
      LookForward();
	  WalkAtRelativeSpeed(Pose2f(1.f, 0.f, 0.f));
//	  LookLeftandRight();

    }
  }

 /**转向队友传过来的球的位置，注意此时自己是看不到球的*/
 state(turnToTeamBall)
  {
    transition
    {
	  if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
	  /**若机器人相对于球的虚拟点的角度调整合适，则走向虚拟点*/
      if(std::abs(theLibCodeRelease.angleToBall2) < 20_deg || state_time >10000.f)
			goto walkToTeamBall;
	  /**若全队没人看到球，则原地找球*/
	  if(theTeamBallModel.isValid == false || state_time >12000.f)
			goto searchForBall2;			
		
    } 
    action
    {
	  OUTPUT_TEXT("turnToTeamBall");
//	  OUTPUT_TEXT("angleToBall");
//	  OUTPUT_TEXT(theLibCodeRelease.angleToBall);
//	  OUTPUT_TEXT("teamBallDistance");
//	  OUTPUT_TEXT(theLibCodeRelease.teamBallDistance);
	  OUTPUT_TEXT("theTeamBallModel.isValid");
	  OUTPUT_TEXT(theTeamBallModel.isValid);
//	  OUTPUT_TEXT(theLibCodeRelease.angleToBall2);
//	  OUTPUT_TEXT(theLibCodeRelease.ballx2);
//	  OUTPUT_TEXT(theLibCodeRelease.bally2);
	  OUTPUT_TEXT("//");
      //LookForward();
      WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(theLibCodeRelease.angleToBall2, 0.f, 0.f));//转相应的角度,面对球
//	  FollowTheTarget(theLibCodeRelease.angleToBall2);
	  LookLeftandRight();
    }
  }
  /**走到队友传过来球的虚拟位置附近*/
    state(walkToTeamBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < 600.f )
			goto turnToBall;
		 
	  /**若走到虚拟点附近，则再转向球*/
	  if(std::abs(theRobotPose.translation.x() - theLibCodeRelease.xx)<300.f && std::abs(theRobotPose.translation.y() - theLibCodeRelease.yy)<300.f )
			goto turnToTeamBallAgain;
	  /**若全队没人看到球，则立即找球*/
	  if( theTeamBallModel.isValid == false || state_time > 12000.f || theRobotPose.translation.x() > 3500.f)
			  goto searchForBall2;
			  
//	  if(theLibCodeRelease.teamBallDistance == true && theTeamBallModel.isValid == true && theTeamBallModel.contributors ==3)
//			  goto waitForBall;			  
	  
	 
    }
    action
    {
      FollowTheTarget(theLibCodeRelease.angleToBall);
	  OUTPUT_TEXT("walkToTeamBall");
	  OUTPUT_TEXT("theTeamBallModel.isValid");
	  OUTPUT_TEXT(theTeamBallModel.isValid);
//	  OUTPUT_TEXT("teamBallDistance");
//	  OUTPUT_TEXT(theLibCodeRelease.teamBallDistance);
//	  OUTPUT_TEXT(theLibCodeRelease.angleToBall2);
//	  OUTPUT_TEXT(theLibCodeRelease.ballx2);
//	  OUTPUT_TEXT(theLibCodeRelease.bally2);
//	  OUTPUT_TEXT("0000000000");WalkToDestination
//	  OUTPUT_TEXT(theRobotPose.translation.x() - theLibCodeRelease.xx);
//	  OUTPUT_TEXT(theRobotPose.translation.y() - theLibCodeRelease.yy);
	  OUTPUT_TEXT("/////");
//      WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(theLibCodeRelease.angleToBall2,theLibCodeRelease.ballx2,theLibCodeRelease.bally2));//球在机器人的坐标
	  WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(theLibCodeRelease.angleToBall2,theLibCodeRelease.ballx2,theLibCodeRelease.bally2));
    }
  }
/**机器人走到虚拟点附近后，一般不会是朝向球的方向，这时需要再转向球*/
state(turnToTeamBallAgain)
  {
    transition
    {
	  if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;	 	
	  /**若转向球角度合适，或者全队没人看到球，则立即找球*/
	  if(theTeamBallModel.isValid == false || state_time > 8000.f || std::abs(theLibCodeRelease.angleToBall) < 30_deg)
			goto searchForBall2;			
		
//	  if(theLibCodeRelease.teamBallDistance == true && theTeamBallModel.isValid == true && theTeamBallModel.contributors ==3)
//			  goto waitForBall;
	  	
	  
    } 
    action
    {
	  OUTPUT_TEXT("turnToTeamBallAgain000");
//	  OUTPUT_TEXT("angleToBall");
//	  OUTPUT_TEXT(theLibCodeRelease.angleToBall);
//	  OUTPUT_TEXT("teamBallDistance");
//	  OUTPUT_TEXT(theLibCodeRelease.teamBallDistance);
	  OUTPUT_TEXT("theTeamBallModel.isValid");
	  OUTPUT_TEXT(theTeamBallModel.isValid);
//	  OUTPUT_TEXT(theLibCodeRelease.angleToBall2);
//	  OUTPUT_TEXT(theLibCodeRelease.ballx2);
//	  OUTPUT_TEXT(theLibCodeRelease.bally2);
	  OUTPUT_TEXT("/////");
      //LookForward();
      WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(theLibCodeRelease.angleToBall, 0.f, 0.f));//转相应的角度,面对球
	  FollowTheTarget(theLibCodeRelease.angleToBall);
    }
  }

  /**回到己方半场，这通常是后卫犯规才回来*/
  state(backOwnField)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若后卫不犯规，则回到中心圆附近*/	
		if(theRobotPose.translation.x()<0.f && ((theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000 && theLibCodeRelease.defender1.isPenalized == false) || (theLibCodeRelease.defender2.isPenalized == false && theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000)))
			goto fromOwnFieldBackMiddle2;	
		/**若机器人走到己方半场相应位置，则找球做决定*/	
		if(state_time >8000.f||(std::abs(theRobotPose.translation.x()+2300.f)<300.f&&std::abs(theRobotPose.translation.y())<300.f))
			goto searchForBall2;

    }
    action
    {  
	  OUTPUT_TEXT("backOwnField");		
//	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi), (theRobotPose.translation.x() +2300.f), theRobotPose.translation.y()));
//	  WalkToDestination
	 WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi), (theRobotPose.translation.x() +2300.f), theRobotPose.translation.y()));
     LookLeftandRight();
    }
  }
  
  
  /**转身，为了走得更稳*/
  state(backOwnField2)
  {
    transition
    {	
		if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			goto turnToBall;
			
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	
		/**若后卫不犯规，则转向中心圆*/	
		if(theRobotPose.translation.x()<0.f && ((theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000 && theLibCodeRelease.defender1.isPenalized == false) || (theLibCodeRelease.defender2.isPenalized == false && theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000)))
			goto fromOwnFieldBackMiddle2;	
		/**若机器人朝向己方半场调整角度合适，则回到己方半场*/	
		if(state_time >8000.f||std::abs(theRobotPose.rotation)>pi*11/12)
			goto backOwnField;
    }
    action
    {  
	  OUTPUT_TEXT("backOwnField222222222");	
	  WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-(theRobotPose.rotation-pi), 0.f, 0.f));
     LookLeftandRight();
    }
  }
/**转向面对敌方半场*/  
state(ownField)
  {
     transition
     {
         if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			  goto turnToBall;
			  
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	  
			  
		 if(theRobotPose.translation.x()<0.f && ((theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000 && theLibCodeRelease.defender1.isPenalized == false) || (theLibCodeRelease.defender2.isPenalized == false && theLibCodeRelease.defender1.timeWhenLastPacketReceived >10000)))
			goto fromOwnFieldBackMiddle2;		  
			  
		 if(state_time >8000.f||(std::abs(theRobotPose.translation.x() + 2300.f)<300.f&&std::abs(theRobotPose.translation.y())<300.f))
              goto searchForBall2;

		 
     }
	 
     
     action
     {
		 OUTPUT_TEXT("backyuan");
//         WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, -(theRobotPose.translation.x() + 2300.f), -theRobotPose.translation.y()));
//	  WalkToDestination
		 WalkToDestination(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation, -(theRobotPose.translation.x() + 2300.f), -theRobotPose.translation.y()));
         LookLeftandRight();
     }
  }  
  
 /**转身面对敌方半场*/ 
  state(ownField2)
  {
     transition
     {
         if(theLibCodeRelease.timeSinceBallWasSeen < 600.f)
			  goto turnToBall;
			  
		if(theLibCodeRelease.ballx2 > 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall;
	 		
		if(theLibCodeRelease.ballx2 < 0.f && theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2))	
			goto turnToTeamBall2;	  
			  
		 if(state_time >8000.f || std::abs(theRobotPose.rotation)<pi/12)
              goto ownField;

		 
     }
     action
     {
		 OUTPUT_TEXT("ownField222222");
         WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(-theRobotPose.rotation,0.f, 0.f));
         LookLeftandRight();
     }
  }
  
  
  
}

