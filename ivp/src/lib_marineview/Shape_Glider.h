/*****************************************************************/
/*    NAME: Michael Benjamin and John Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: Shape_Glider.h                                       */
/*    DATE: October 5th 2007                                     */
/*****************************************************************/

#ifndef SHAPE_GLIDER_HEADER
#define SHAPE_GLIDER_HEADER

float g_gliderBody[]=
{
   2.0,   0.0,
   3.0,   2.0, 
   6.0,  13.0,
   9.0,  32.0,
   9.2,  35.0, 
   9.0,  38.0, 
   6.0,  57.0, 
   3.0,  68.0, 
   2.0,  70.0, 
   0.0,  70.5, 
  -2.0,  70.0,
  -3.0,  68.0, 
  -6.0,  57.0, 
  -9.0,  38.0, 
  -9.2,  35.0, 
  -9.0,  32.0, 
  -6.0,  13.0, 
  -3.0,   2.0, 
  -2.0,   0.0
};

double       g_gliderCtrX     = 0.0;
double       g_gliderCtrY     = 35.0;
unsigned int g_gliderBodySize = 19;
double       g_gliderScale    = 1.5;

float g_gliderWing[]=
{
    5.0,  10.0, 
   33.0,  12.0, 
   33.0,  19.0, 
   12.0,  26.0, 
    0.0,  28.0, 
  -12.0,  26.0, 
  -33.0,  19.0, 
  -33.0,  12.0,
   -5.0,  10.0
};

unsigned int g_gliderWingSize = 9;

#endif



