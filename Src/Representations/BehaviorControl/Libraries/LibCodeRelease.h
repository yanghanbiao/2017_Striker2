/**
 * @file LibCodeRelease.h
 */

#pragma once
#include "Tools/Function.h"
#include "Representations/Perception/PlayersPercepts/PlayersFieldPercept.h"
#include "Tools/Modeling/Obstacle.h"
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/Communication/TeamData.h"
//#include "Representations/Perception/PlayersPercepts/PlayersFieldPercept.h"
STREAMABLE(LibCodeRelease,
{
  FUNCTION(bool(float value, float min, float max)) between,
  
  (float)(0.f) angleToGoal,			/**机器人相对与地方球门中心角度*/
  (int)(0) timeSinceBallWasSeen,
  (bool)(false) movedBall,														        /**判断球是不是移动了，在下面计算判断，方法为球上一个位置和下一个位置的距离大于三厘米*/
  (float)(0.f) globalballx,			/**球的全局坐标，根据相对于机器人的角度和机器人的全局坐标求得*/
  (float)(0.f) globalbally,
  (bool)(false) detectedJersey,			/**判断视野中出现的球衣，与owntTeam结合使用判断看到的是己方还是敌方*/
  (bool)(false) ownTeam,			/**判断是否为己方球衣*/
  (float)(0.f) centerOnFieldx,		/**视野中看到球衣中心相对于机器人的坐标*/
  (float)(0.f) centerOnFieldy,
  (float)(0.f) Fieldx,				/**就是centerOnFieldx*/
  (float)(0.f) Fieldy,
  (float)(0.f) leftOnFieldx,
  (float)(0.f) leftOnFieldy,
  (float)(0.f) kickAngle,
  (float)(0.f) globalplayx,			/**视野中球衣的全局坐标*/
  (float)(0.f) globalplayy,
//  (int)(0) kickleg,				/**踢球用的哪个脚*/
//  (float)(0.f) turnbody,			/**转身的角度*/
//  (bool)(false) observesituation,
//  (bool)(false) detected,
  (float)(0.f) angle1,				/**转头的角度，本代码使用在射门的时候机器人前方观察事业的限制*/
  (float)(0.f) angle2,
  (float)(0.f) kickangle1,			/**踢球的角度*/
  (float)(0.f) kickangle2,
//  (bool)(false) alreadykick,		/**判断是不是已经踢完了*/
  (float)(0.f) lasttheBallModelx,	/**上一刻球的坐标*/
  (float)(0.f) lasttheBallModely,
  (float)(0.f) angleKickBackGround,	/**踢回中心圆*/
  (float)(0.f) distanceRobotToGoal,	/**机器人相对于中心的距离*/
  (float)(0.f) angleKickRobot,		/**机器人相对于其他机器人的角度*/
  (float)(0.f) angleKickRobotLeft,	/**对方机器人与左边球门的夹角*/
  (float)(0.f) angleKickRobotRight,	/**同理,右边*/
  (PlayersFieldPercept::PlayerOnField) allCenterOnField[10],/**关于球衣检测障碍物*/
  (int) numberOfJersey,
  (Obstacle) obstacle,				/**可以引用障碍物相关变量*/
  (bool)(false) obstacleRobot,		/**障碍机器人*/
  
  (float)(0.f)distanceBallToGoal,	/**球到球门的距离*/
  (float)(0.f)ballx,				/**队友传过来的球的全局坐标转换成相对于机器人自己的x坐标*/
  (float)(0.f)ballx2,				/**队友传过来的球偏移点的全局坐标转换成机器人自己的x坐标*/
  (float)(0.f)bally,				/**队友传过来的球的全局坐标转换成相对于机器人自己的y坐标*/
  (float)(0.f)bally2,				/**队友传过来的球偏移点的全局坐标转换成机器人自己的y坐标*/
  (float)(0.f)ballDistance,			/**以上ballx和bally求得的距离*/
  (float)(0.f)angleToBall,			/**队友传过来的球的全局坐标转换成相对于机器人的角度*/
  (float)(0.f)angleToBall2,			/**队友传过来的球的偏移点的全局坐标转换成相对于机器人的角度*/
  (float)(0.f)xx,					/**临时的变量，全局坐标*/
  (float)(0.f)yy,					/**临时变量，全局坐标*/
  (Vector2f) ballPosition,			/**队友传过来的球的全局坐标*/
  (TeamBallModel)contributors,		/**枚举类型，详情请看Representations/Modeling/TeamBallModel.h，0为只有自己看到，1和2为队友看到，3为自己和队友同时看到*/
  (Teammate) striker1,/**定义的角色，用于通信分角色的策略，详情看Representations/Communication/TeamData.h，分角色调用TeamdData的变量*/
  (Teammate) striker2,				/**同上，类似*/
  (Teammate) defender1,				/**类似*/
  (Teammate) defender2,				/**类似*/
  (Teammate) keeper,				/**类似*/
  (bool)(false) teamBallDistance,	/**队友看到球，球离机器人的距离*/
  (PlayersFieldPercept::PlayerOnField)  closestObstacle,
  (int)(0) ObsAtLorR, 				/**障碍物是在左边还是右边，1代表左边，-1代表右边*/
 
});
