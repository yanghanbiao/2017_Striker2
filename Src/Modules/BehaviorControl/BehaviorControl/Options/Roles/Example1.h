option(Example1)
{
  initial_state(start)
  {
    transition
    {
	  OUTPUT_TEXT("Example1-initial_state-transition");
      if(state_time > 1000)
        goto start1;
    }
    action
    {
	  OUTPUT_TEXT("Example1-initial_state-action");
      LookForward();
      Stand();
    }
  }
   state(start1)
  {
    transition
    {
      if((std::abs(theRobotPose.rotation )<5_deg  && std::abs(theRobotPose.translation.x() -2500) <100 && std::abs(theRobotPose.translation.y()) <100 )  || (theLibCodeRelease.timeSinceBallWasSeen <300 && state_time > 4000) )
        goto turnToBall;
    }
    action
    {
	   LookForward();
	   WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(-theRobotPose.rotation, -(theRobotPose.translation.x() - 2500), -theRobotPose.translation.y()));
	  OUTPUT_TEXT("start");
    }
  }
  state(turnToBall)
  {
    transition
    {
	  OUTPUT_TEXT("Example1-turnToBall-transition");
	  if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(std::abs(theBallModel.estimate.position.angle()) < 5_deg)
        goto walkToBall;
	  
    }
    action
    {
	  OUTPUT_TEXT("Example1-turnToBall-action");
      
      WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
    }
  }

  state(walkToBall)
  {
    transition
    {
	  OUTPUT_TEXT("Example1-walkToBall-transition");
	  if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(theBallModel.estimate.position.norm() < 500.f)
        goto alignToGoal;
    }
    action
    {
	  OUTPUT_TEXT("Example1-walkToBall-action");
      LookForward();
      WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
    }
  }

  state(alignToGoal)
  {
    transition
    {
      OUTPUT_TEXT("Example1-alignToGoal-transition");
	  if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(std::abs(theLibCodeRelease.angleToGoal) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
        goto alignBehindBall;
    }
    action
    {
	  OUTPUT_TEXT("Example1-alignToGoal-action");
      LookForward();
      WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 400.f, theBallModel.estimate.position.y()));
    }
  }

  state(alignBehindBall)
  {
    transition
    {
	  OUTPUT_TEXT("Example1-alignBehindBall-transition");
	  if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(theLibCodeRelease.between(theBallModel.estimate.position.y(), 20.f, 50.f)
      && theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
      && std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
        goto kick;
    }
    action
    {
	  OUTPUT_TEXT("Example1-alignBehindBall-action");
      LookForward();
      WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 150.f, theBallModel.estimate.position.y() - 30.f));
    }
  }

  state(kick)
  {
    transition
    {
	  OUTPUT_TEXT("Example1-kick-transition");
      if(state_time > 3000 || (state_time > 10 && action_done))
        goto turnToBall;
    }
    action
    {
	  OUTPUT_TEXT("Example1-kick-action");
      LookForward();
      InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::left), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
    }
  }
  state(searchForBall)
  {
    transition
    {
	  OUTPUT_TEXT("Example1-searchForBall-transition");
      if(theLibCodeRelease.timeSinceBallWasSeen < 300)
        goto turnToBall;
    }
    action
    {
	  OUTPUT_TEXT("Example1-searchForBall-transition");
      LookForward();
      WalkAtRelativeSpeed(Pose2f(1.f, 0.f, 0.f));
    }
  }
}
