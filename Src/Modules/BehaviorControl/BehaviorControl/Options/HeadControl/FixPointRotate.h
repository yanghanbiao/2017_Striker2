option(FixPointRotate)
{
	/**站着*/
	initial_state(standup)
	{
		transition
		{
			if (state_time > 2000.f)
			{
				goto lookleftright;
			}
		}
		action
		{
			Activity(BehaviorStatus::gettingUp);
			Stand();
		}
	}
	/**向左向右看*/
	state(lookleftright)
	{
		transition
		{
			if ( state_time > 4000.f)
				goto lookforward;
		}
		
		action
		{
			LookLeftandRight();
		}
	}
	/**转身*/
	state(rotate)
	{
		transition
		{
			if (state_time > 2200.f)
				goto standup;
		}
		action
		{
			WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(90_deg, 0.f, 0.f));
		}
	}
			
	/**往前看*/
	state(lookforward)
	{
		transition
		{
			if (state_time > 500.f)
				goto rotate;
		}
		
		action 
		{
			theHeadControlMode = HeadControl::lookForward;
			OUTPUT_TEXT("look");
		}
	}
}