/** All option files that belong to the current behavior have to be included by this file. */

#include "Options/Soccer.h"

#include "Options/HeadControl/HeadControl.h"
#include "Options/HeadControl/LookForward.h"
#include "Options/HeadControl/LookLeftandRight.h"
#include "Options/HeadControl/FollowTheTarget.h"
#include "Options/HeadControl/LookCircle.h"
#include "Options/HeadControl/FixPointRotate.h"
#include "Options/HeadControl/LookRight.h"

#include "Options/GameControl/HandleGameState.h"
#include "Options/GameControl/HandlePenaltyState.h"
#include "Options/GameControl/PlayingState.h"
#include "Options/GameControl/ReadyState.h"
#include "Options/GameControl/StrikerReady.h"
#include "Options/GameControl/FinishedState.h"


#include "Options/Output/Activity.h"
#include "Options/Output/Annotation.h"
#include "Options/Output/HeadMotionRequest/SetHeadPanTilt.h"
#include "Options/Output/MotionRequest/InWalkKick.h"
#include "Options/Output/MotionRequest/SpecialAction.h"
#include "Options/Output/MotionRequest/Stand.h"
#include "Options/Output/MotionRequest/WalkAtAbsoluteSpeed.h"
#include "Options/Output/MotionRequest/WalkAtRelativeSpeed.h"
#include "Options/Output/MotionRequest/WalkToTarget.h"
#include "Options/Output/MotionRequest/GetUpEngine.h"
#include "Options/Output/PlaySound.h"
#include "Options/Output/MotionRequest/Shoot.h"
#include "Options/Output/MotionRequest/WalkToGoal.h"
#include "Options/Output/HeadMotionRequest/PathPlantoball.h"
#include "Options/Output/HeadMotionRequest/PathPlantot.h"
#include "Options/Output/MotionRequest/WalkToDestination.h"

#include "Options/Roles/Striker.h"
#include "Options/Roles/Example1.h"




#include "Options/Skills/GetUp.h"

#include "Options/DemoOptions/Demo.h"
#include "Options/DemoOptions/Waving.h"

#include "Options/Tools/ButtonPressedAndReleased.h"

