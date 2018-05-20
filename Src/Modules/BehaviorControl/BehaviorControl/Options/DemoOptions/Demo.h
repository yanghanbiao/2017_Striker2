/**
 * @file Demo.h
 *
 * @author <a href="mailto:jesse@tzi.de">Jesse Richter-Klug</a>
 */

option(Demo)
{
  common_transition
  {
    switch(theLibDemo.demoGameState)
    {
      case LibDemo::waving:
        goto waving;
      case LibDemo::normal:
        goto normal;
      default:
        FAIL("Unknown demo game state.");
    }
  }

initial_state(normal)
  { 
	action
	{   
		//if(theRobotInfo.number == 1)
			//Keeper();
		//if(theRobotInfo.number == 2)
			Striker();
			//Example1();
		//if(theRobotInfo.number == 3)
			//Defender1();
	    //if(theRobotInfo.number == 4)
			//Defender();
		//if(theRobotInfo.number == 5)
			//Supporter();
			/*case 1: Keeper();
			case 2: Striker();
			case 3: Defender1();
			case 4: Defender();
			case 5: Supporter();
			case 6: Keeper();
			case 7: Striker();
			case 8: Defender1();
			case 9: Defender();
			case 10: Supporter();*/
		}			
		//Defender();
		//Striker(); 
		//Defender1();
		//Keeper();
	
  }
  state(waving)
  {
    action
    {
      Activity(BehaviorStatus::waving);
      theHeadControlMode = HeadControl::lookForward;
      Waving();
    }
  }
}
