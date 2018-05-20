/**
 * @file LibCodeRelease.cpp
 */

#include "LibCodeReleaseProvider.h"

class theRobotPose;
MAKE_MODULE(LibCodeReleaseProvider, behaviorControl);

void LibCodeReleaseProvider::update(LibCodeRelease& libCodeRelease)
{
  /**踢球的角度，16年遗留的代码，我没用过*/
  libCodeRelease.kickAngle = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 600.f)).angle();
  /**机器人面对敌方球门时，机器人相对于左边球门全局坐标为y=300的位置踢球，用于踢球的角度来躲过守门员的防守*/
  libCodeRelease.angle1 = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 300.f)).angle();
  /**机器人面对敌方球门时，机器人相对于左边球门全局坐标为y=-300的位置踢球，用于踢球的角度来躲过守门员的防守*/
  libCodeRelease.angle2 = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, -300.f)).angle();
  /**由相对于机器人的坐标计算出球的全局坐标*/
  libCodeRelease.globalballx = theRobotPose.translation.x()+std::sqrt(std::pow(theBallModel.estimate.position.x(),2)+std::pow(theBallModel.estimate.position.y(),2))*std::cos(theRobotPose.rotation+theBallModel.estimate.position.angle());
  /**同理*/
  libCodeRelease.globalbally = theRobotPose.translation.y()+std::sqrt(std::pow(theBallModel.estimate.position.x(),2)+std::pow(theBallModel.estimate.position.y(),2))*std::sin(theRobotPose.rotation+theBallModel.estimate.position.angle());
  /**现在距离球最后一次看到的时间*/
  libCodeRelease.timeSinceBallWasSeen = theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen);
  /**机器人相对球门中心的角度,用于调整机器人角度射门*/
  libCodeRelease.angleToGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
  
  /**机器人与球门中心的距离*/
  libCodeRelease.distanceRobotToGoal = std::sqrt(std::pow(theRobotPose.translation.y(),2) + std::pow(4500 - theRobotPose.translation.x(),2));
  /**球到球门中心的距离*/
  libCodeRelease.distanceBallToGoal=std::sqrt(std::pow(libCodeRelease.globalbally,2)+std::pow(4500-libCodeRelease.globalballx,2));
  
 
  int i=0;
  /**判断球是否移动，上一次球的位置和现在球的位置之间的距离大于30cm则认为球移动了，移动为true，未移动为false*/
  if(std::sqrt(std::pow( libCodeRelease.lasttheBallModelx -  libCodeRelease.globalballx ,2)+ std::pow( libCodeRelease.lasttheBallModely - libCodeRelease.globalbally,2)) > 300)
	{
		 libCodeRelease.movedBall = true;
	}
	
	if(std::sqrt(std::pow( libCodeRelease.lasttheBallModelx -  libCodeRelease.globalballx ,2)+ std::pow( libCodeRelease.lasttheBallModely - libCodeRelease.globalbally,2)) < 300)
	{
		 libCodeRelease.movedBall = false;
	}
	/**放在判断的后面赋值，在判断时获取的才是上一时刻的球的全局坐标*/
	libCodeRelease.lasttheBallModelx = libCodeRelease.globalballx;
    libCodeRelease.lasttheBallModely = libCodeRelease.globalbally;
	/**通过球衣判断其他机器人的位置，球衣识别未解决，策略中也没用过，详情看Representations/Perception/PlayersPercepts/PlayersFieldPercept.h*/
  for(const PlayersFieldPercept::PlayerOnField& p: thePlayersFieldPercept.players)
	{
		libCodeRelease.ownTeam = p.ownTeam;
		libCodeRelease.detectedJersey = p.detectedJersey;
		libCodeRelease.centerOnFieldx = p.centerOnField.x();
		libCodeRelease.centerOnFieldy = p.centerOnField.y();
		libCodeRelease.leftOnFieldx = p.leftOnField.x();
		libCodeRelease.leftOnFieldy = p.leftOnField.y();
		libCodeRelease.allCenterOnField[i] = p;//利用数组存储多个机器人相关变量
		i++;
		
	}
	libCodeRelease.numberOfJersey=i;
	/**判断障碍物的变量，多次调试发现这个变量极为不可靠，详情看Representations/Modeling/ObstacleModel.h*/
   for(const auto& p: theObstacleModel.obstacles)
	{
		libCodeRelease.obstacle = p;
	}
	
  libCodeRelease.between = [&](float value, float min, float max) -> bool
  {
    return value >= min && value <= max;
  };
  /**自己对准检测到的机器人的角度，需要更详细了解inversePose这个已经封装好的函数*/
  libCodeRelease.angleKickRobot = (theRobotPose.inversePose * Vector2f(libCodeRelease.centerOnFieldx, libCodeRelease.centerOnFieldy)).angle();
  /**视野中机器人和angle1的夹角*/
  libCodeRelease.angleKickRobotLeft = std::abs(libCodeRelease.angle1-libCodeRelease.angleKickRobot);
  /**视野中机器人和angle2的夹角,通过比较哪边射门角度大，就往哪边踢球*/
  libCodeRelease.angleKickRobotRight = std::abs(libCodeRelease.angle2-libCodeRelease.angleKickRobot);
   libCodeRelease.obstacleRobot = false;
   /**判断前方是否有障碍*/
   if(libCodeRelease.between(libCodeRelease.obstacle.type,2,7))
	   libCodeRelease.obstacleRobot = true;
	/**假如有障碍,利用公式计算出该障碍的全局坐标*/
  if(libCodeRelease.obstacleRobot == true)
  {
      
	  libCodeRelease.globalplayx = libCodeRelease.obstacle.center.x() * std::cos(theRobotPose.rotation) + libCodeRelease.obstacle.center.y() * std::sin(theRobotPose.rotation) + theRobotPose.translation.x();
	  libCodeRelease.globalplayy = -libCodeRelease.obstacle.center.x() * std::sin(theRobotPose.rotation) + libCodeRelease.obstacle.center.y() * std::cos(theRobotPose.rotation) - theRobotPose.translation.y();
	
  }
  
  
	  /**teammate中的数据会对应bush中的角色，赋值给相应的角色*/
  for(auto const& teammate : theTeamData.teammates)
    {
        switch(teammate.number)
        {
            case 1:
                libCodeRelease.keeper = teammate; break;
            case 2:
                libCodeRelease.striker1 = teammate; break;
            case 3:
                libCodeRelease.defender1 = teammate; break;
            case 4:
                libCodeRelease.defender2 = teammate; break;
            case 5:
                libCodeRelease.striker2 = teammate; break;
            default: break;
        }
    }
	  /**队友看到球且有效的情况下，传过来的球的坐标是全局坐标，需要转换成相对于机器人的坐标*/
  if(theTeamBallModel.isValid == true && (theTeamBallModel.contributors ==1 || theTeamBallModel.contributors == 2 || theTeamBallModel.contributors == 3))	
  {  
	/**赋值*/
	libCodeRelease.ballPosition = theTeamBallModel.position;

	/**计算相对坐标x，这是因为walkToTarget函数的输入参数是相对于机器人坐标，其实也可以尝试把这个封装成一个函数*/
	libCodeRelease.ballx = (libCodeRelease.ballPosition.x()-theRobotPose.translation.x())*std::cos(theRobotPose.rotation)+(libCodeRelease.ballPosition.y()-theRobotPose.translation.y())*std::sin(theRobotPose.rotation);
	/**计算相对坐标y*/
	libCodeRelease.bally = -(libCodeRelease.ballPosition.x()- theRobotPose.translation.x())*std::sin(theRobotPose.rotation)+(libCodeRelease.ballPosition.y()-theRobotPose.translation.y())*std::cos(theRobotPose.rotation);
	/**计算距离*/
	libCodeRelease.ballDistance = std::sqrt(std::pow(libCodeRelease.ballx,2)+std::pow(libCodeRelease.bally,2));	
	
	/**计算相对球机器人的角度，在范围为-90°到90°很准确，即机器人身前，机器人身后的话，一般不能直接用这个角度，需要让机器人先转身*/
	libCodeRelease.angleToBall = ((std::acos((libCodeRelease.ballPosition.x()-theRobotPose.translation.x())/libCodeRelease.ballDistance)-theRobotPose.rotation)+(std::asin((libCodeRelease.ballPosition.y()-theRobotPose.translation.y())/libCodeRelease.ballDistance)-theRobotPose.rotation))/2;
	
	/**计算一个虚拟的偏移点，可以类比与篮球中快攻的跑位策略，当队友看到球时，自己先到达前面不影响队友射门的位置去接应，当球处于很深的位置，离敌方球门非常近，那么在后面罚球点位置接应，接应的位置不能太深*/
	/**机器人在球的身后1500位置且球大于-1500，之所以这样，是担心移动时会撞到队友*/
	if(theRobotPose.translation.x() < (libCodeRelease.ballPosition.x() - 1500.f) && libCodeRelease.ballPosition.x() > -1500.f)
	{
	/**球在中间位置，机器人为了不影响队友射门则往相对于球的一侧移动*/
	if(libCodeRelease.ballPosition.y() <= 1000.f && libCodeRelease.ballPosition.y() >= -1000.f && libCodeRelease.ballPosition.x() < 2500.f)
	{
		if(theRobotPose.translation.y() > libCodeRelease.ballPosition.y())
		{
			libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
			libCodeRelease.yy = libCodeRelease.ballPosition.y() + 1300.f;
		}
		else
		{
			libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
			libCodeRelease.yy = libCodeRelease.ballPosition.y() - 1300.f;
		}

	}
	
	/**球如果在很靠近边缘位置，则让机器人往里面接应，不过这样子可能会影响队友射门*/
	if(libCodeRelease.ballPosition.y() > 1000.f && libCodeRelease.ballPosition.x()<2500.f)
	{
		libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
		libCodeRelease.yy = libCodeRelease.ballPosition.y() - 1300.f;
	}
	/**同理*/
	if(libCodeRelease.ballPosition.y() < -1000.f && libCodeRelease.ballPosition.x() <2500.f)
	{
		libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
		libCodeRelease.yy = libCodeRelease.ballPosition.y() + 1300.f;
	}
	/**防止机器人走到太深的位置*/
	if(libCodeRelease.ballPosition.x() < 2500.f && theRobotPose.translation.x() > 3200.f)
	{
		libCodeRelease.xx = 3000.f;
	}
	/**防止机器人走到太深的位置*/
	if(libCodeRelease.ballPosition.x()>=2500.f)
	{
		libCodeRelease.xx = 3000.f;
		libCodeRelease.yy = 0.f;
	}
	}
	
	/**机器人在球后1500mm的前面，相应的偏移点赋值，以下同理*/	
	if(theRobotPose.translation.x() > (libCodeRelease.ballPosition.x() - 1500.f) && libCodeRelease.ballPosition.x() > -1500.f)
	{
		if(libCodeRelease.ballPosition.y() <= 1500.f && libCodeRelease.ballPosition.y() >= -1500.f  && libCodeRelease.ballPosition.x() < 2500.f)
	{
		if(theRobotPose.translation.y() > libCodeRelease.ballPosition.y())
		{
			libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
			libCodeRelease.yy = libCodeRelease.ballPosition.y() + 1300.f;
		}
		else
		{
			libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
			libCodeRelease.yy = libCodeRelease.ballPosition.y() - 1300.f;
		}

	}
	
//
	if(libCodeRelease.ballPosition.y() > 1500.f  && libCodeRelease.ballPosition.x()<2500.f)
	{
		libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
		libCodeRelease.yy = libCodeRelease.ballPosition.y() - 1300.f;
	}
	
	if(libCodeRelease.ballPosition.y() < -1500.f  && libCodeRelease.ballPosition.x()<2500.f)
	{
		libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
		libCodeRelease.yy = libCodeRelease.ballPosition.y() + 1300.f;
	}
	
	if(libCodeRelease.ballPosition.x() < 2500.f && theRobotPose.translation.x() > 3200.f)
	{
		libCodeRelease.xx = 3000.f;
	}
	
	if(libCodeRelease.ballPosition.x()>=2500.f)
	{
		libCodeRelease.xx = 3000.f;
		libCodeRelease.yy = 0.f;
	}
	}
	/**如果后卫犯规，且球在球场-1500mm的位置，判断后卫是否犯规需要结合timeWhenLastPacketReceived，因为isPenalized默认就是true，先保证和后卫是有通信联系的，才能去判断后卫是否犯规，这一点非常重要，因为比赛时有可能是队友是连不上wifi的，其他赋值同理*/
	if(libCodeRelease.ballPosition.x() < -1500.f && ((libCodeRelease.defender1.timeWhenLastPacketReceived >10000 && libCodeRelease.defender1.isPenalized == true) || (libCodeRelease.defender2.isPenalized == true && libCodeRelease.defender1.timeWhenLastPacketReceived >10000)))
	{
		if(libCodeRelease.ballPosition.y() <= 1000.f && libCodeRelease.ballPosition.y() >= -1000.f)
	{
		if(theRobotPose.translation.y() > libCodeRelease.ballPosition.y())
		{
			libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
			libCodeRelease.yy = libCodeRelease.ballPosition.y() + 750.f;
		}
		else
		{
			libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
			libCodeRelease.yy = libCodeRelease.ballPosition.y() - 750.f;
		}

	}
	
//
	if(libCodeRelease.ballPosition.y() > 1000.f)
	{
		libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
		libCodeRelease.yy = libCodeRelease.ballPosition.y() - 750.f;
	}
	
	if(libCodeRelease.ballPosition.y() < -1000.f)
	{
		libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1000.f;
		libCodeRelease.yy = libCodeRelease.ballPosition.y() + 750.f;
	}
	}
	
	/**后卫不犯规且球在己方半场*/
	if(libCodeRelease.ballPosition.x() < -1500.f && ((libCodeRelease.defender1.timeWhenLastPacketReceived >10000 && libCodeRelease.defender1.isPenalized == false) || (libCodeRelease.defender2.isPenalized == false && libCodeRelease.defender1.timeWhenLastPacketReceived >10000)))
	{
		if(libCodeRelease.ballPosition.y() <= 1000.f && libCodeRelease.ballPosition.y() >= -1000.f  )
	{
		if(theRobotPose.translation.y() > libCodeRelease.ballPosition.y())
		{
			libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1500.f;
			libCodeRelease.yy = libCodeRelease.ballPosition.y() + 750.f;
		}
		else
		{
			libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1500.f;
			libCodeRelease.yy = libCodeRelease.ballPosition.y() - 750.f;
		}

	}
	
//
	if(libCodeRelease.ballPosition.y() > 1000.f )
	{
		libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1500.f;
		libCodeRelease.yy = libCodeRelease.ballPosition.y() - 750.f;
	}
	
	if(libCodeRelease.ballPosition.y() < -1000.f )
	{
		libCodeRelease.xx = libCodeRelease.ballPosition.x() + 1500.f;
		libCodeRelease.yy = libCodeRelease.ballPosition.y() + 750.f;
	}
	
	}
	
	/**相应的，计算偏移点相对于机器人的坐标,这是因为walkToTarget函数的输入参数是相对于机器人坐标，其实也可以尝试把这个封装成一个函数*/
	libCodeRelease.ballx2 = (libCodeRelease.xx-theRobotPose.translation.x())*std::cos(theRobotPose.rotation)+(libCodeRelease.yy-theRobotPose.translation.y())*std::sin(theRobotPose.rotation);	  
	libCodeRelease.bally2 = -(libCodeRelease.xx- theRobotPose.translation.x())*std::sin(theRobotPose.rotation)+(libCodeRelease.yy-theRobotPose.translation.y())*std::cos(theRobotPose.rotation);

	libCodeRelease.angleToBall2 = ((std::acos((libCodeRelease.xx-theRobotPose.translation.x())/std::sqrt(std::pow(libCodeRelease.ballx2,2)+std::pow(libCodeRelease.bally2,2)))-theRobotPose.rotation)+(std::asin((libCodeRelease.yy-theRobotPose.translation.y())/std::sqrt(std::pow(libCodeRelease.ballx2,2)+std::pow(libCodeRelease.bally2,2)))-theRobotPose.rotation))/2;
	
  }
  /**队友和自己同时看到球的情况下，判断谁离球更近，誰离球更近谁处理球*/
  if(theTeamBallModel.isValid == true && theTeamBallModel.contributors == 3)
  {
	/**getTimeSince先确定角色，谁看到球，比如striker1看到球；再相应用striker1计算离球的距离，和自己离球的距离进行比较，如果自己离球更远，则为true，以下同理*/
	if(theFrameInfo.getTimeSince(libCodeRelease.striker1.theBallModel.timeWhenLastSeen) < 500 && (float)std::sqrt(std::pow(theTeamBallModel.position.x()-theRobotPose.translation.x(),2) + std::pow(theTeamBallModel.position.y()-theRobotPose.translation.y(),2)) > (float)std::sqrt(std::pow(theTeamBallModel.position.x()-libCodeRelease.striker1.theRobotPose.translation.x(),2) + std::pow(theTeamBallModel.position.y()-libCodeRelease.striker1.theRobotPose.translation.y(),2)))
		libCodeRelease.teamBallDistance = true;
	
	if(theFrameInfo.getTimeSince(libCodeRelease.defender1.theBallModel.timeWhenLastSeen) < 500 && (float)std::sqrt(std::pow(theTeamBallModel.position.x()-theRobotPose.translation.x(),2) + std::pow(theTeamBallModel.position.y()-theRobotPose.translation.y(),2)) > (float)std::sqrt(std::pow(theTeamBallModel.position.x()-libCodeRelease.defender1.theRobotPose.translation.x(),2) + std::pow(theTeamBallModel.position.y()-libCodeRelease.defender1.theRobotPose.translation.y(),2)))
		libCodeRelease.teamBallDistance = true;
		
	if(theFrameInfo.getTimeSince(libCodeRelease.defender2.theBallModel.timeWhenLastSeen) < 500 && (float)std::sqrt(std::pow(theTeamBallModel.position.x()-theRobotPose.translation.x(),2) + std::pow(theTeamBallModel.position.y()-theRobotPose.translation.y(),2)) > (float)std::sqrt(std::pow(theTeamBallModel.position.x()-libCodeRelease.defender2.theRobotPose.translation.x(),2) + std::pow(theTeamBallModel.position.y()-libCodeRelease.defender2.theRobotPose.translation.y(),2)))
		libCodeRelease.teamBallDistance = true;	
		
	if(theFrameInfo.getTimeSince(libCodeRelease.keeper.theBallModel.timeWhenLastSeen) < 500 && (float)std::sqrt(std::pow(theTeamBallModel.position.x()-theRobotPose.translation.x(),2) + std::pow(theTeamBallModel.position.y()-theRobotPose.translation.y(),2)) > (float)std::sqrt(std::pow(theTeamBallModel.position.x()-libCodeRelease.keeper.theRobotPose.translation.x(),2) + std::pow(theTeamBallModel.position.y()-libCodeRelease.keeper.theRobotPose.translation.y(),2)))
		libCodeRelease.teamBallDistance = true;		
	
  }
  
  
  
  
    /**检测看到的障碍物,用于避障*/
	libCodeRelease.closestObstacle.centerOnField.x()=0;
        for(const auto& obstacle : thePlayersFieldPercept.players)
            {
                if(libCodeRelease.closestObstacle.centerOnField.x()==0)
                    libCodeRelease.closestObstacle=obstacle;
                else if(obstacle.centerOnField.x()<libCodeRelease.closestObstacle.centerOnField.x())
                    libCodeRelease.closestObstacle=obstacle;
            }
		if(libCodeRelease.closestObstacle.centerOnField.x()<450 && libCodeRelease.closestObstacle.centerOnField.x()>0 && std::abs(libCodeRelease.closestObstacle.centerOnField.y())<250 && std::abs(libCodeRelease.closestObstacle.centerOnField.y())>0)
        {
            if(libCodeRelease.closestObstacle.centerOnField.y()>0)
                libCodeRelease.ObsAtLorR=1;
            else  libCodeRelease.ObsAtLorR=-1;
        }
        else libCodeRelease.ObsAtLorR=0;

}