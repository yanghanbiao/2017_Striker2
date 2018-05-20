option(PathPlantoball, (const Pose2f&) target , (const Pose2f&) speed,(float)oldBallNorm)
{

  static float pathTrueOffest=100.f;
  static bool wrongPath=false;
//这是一个旗，判断路径规划是否出问题或者达到目标点时应该退出路径规划
  
  initial_state(toball)
  {
    transition
    {

    	if(((theBallModel.estimate.position.norm()-oldBallNorm)>200.f)&&!wrongPath)
    	    goto zaiqueren;
    	if(((std::abs(theBallModel.estimate.position.norm())<130.f)||state_time>5000.f)&&!wrongPath)
        	goto zaiqueren;
     	if(((theBallModel.estimate.position.norm()-oldBallNorm)<200.f)&&(std::abs(theBallModel.estimate.position.norm())<130.f)&&wrongPath)
        	goto huigui;
    }
    action
    { 
	OUTPUT_TEXT(wrongPath);
	OUTPUT_TEXT("wrongPath1");
      	if(!wrongPath)
    	{
		      theMotionRequest=thePathPlanner.plan(target,speed,true);
	        wrongPath=false; 
   	}
      	if(wrongPath)
    	{
		WalkToTarget(speed,target);
        	wrongPath=true;
    	}
    }
  }
  
  state(zaiqueren)
  {
    transition
    {
      	if(state_time>1000)
        	goto toball;
    }
    action
    { 
	OUTPUT_TEXT(wrongPath);
  OUTPUT_TEXT("wrongPath2");
      	wrongPath=true;
    }
  }
    
  state(huigui)
  {
    transition
    {
      	if(state_time>1000)
        	goto toball;
    }
    action
    { 
	OUTPUT_TEXT(wrongPath);
  OUTPUT_TEXT("wrongPath3");
        wrongPath=false;
    }
  }
}