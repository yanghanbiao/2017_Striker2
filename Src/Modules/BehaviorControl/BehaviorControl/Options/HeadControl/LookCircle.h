option(LookCircle, (float) (1.f) rightPan, (float) (-1.f) leftPan, (float) (0.f) Uptilt,(float) (0.38f) Downtilt,(float)(pi) speed, ((HeadMotionRequest) CameraControlMode)(autoCamera) camera)
{
  /** Set the head motion request. */
  initial_state(setRequest)
  {
    transition
    {
      if(action_done)
        goto state1;
    }
    action
    {
      SetHeadPanTilt(rightPan, 0.f, 30_deg);
    }
  }

  /** This state "notifies" the caller that the requested head angles are reached */
  state(state1)
  {
    transition
    {
	  if(action_done)
      goto state2;
    }
    action
    {
      SetHeadPanTilt(0.f, Uptilt, 30_deg);
    }
  }
  state(state2)
  {
	  transition
    {
	  if(action_done)
      goto state3;
    }
    action
    {
      SetHeadPanTilt(leftPan, 0.f, 30_deg);
    }
  }
  state(state3)
  {
	  transition
    {
	  if(action_done)
      goto setRequest;
    }
    action
    {
      SetHeadPanTilt(0.f, Downtilt, 30_deg);
    }
  }
}