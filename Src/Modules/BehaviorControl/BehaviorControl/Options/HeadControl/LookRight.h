option(LookRight, (float)(0.38f) tilt, (float)(-0.8f) pan)
{
  /** Simply sets the necessary angles */
  initial_state(LookRight)
  {
    action
    {
      SetHeadPanTilt(pan, tilt, 150_deg);
    }
  }
}