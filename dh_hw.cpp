#include "systemc.h"
#include "digit.h"
#include "dh_hw.h"

#define WAIT 0
#define INPUT 1
#define EXECUTE 2
#define BONUS 3
#define OUTPUT 4
#define FINISH 5

void dh_hw::process_hw()
{

  NN_DIGIT t[2], c, u, v;
  NN_HALF_DIGIT aHigh, cLow, cHigh;
  
  // initially not done and in wait state
  hw_done.write(false);
  state = WAIT;
  for (;;) 
  {  
      switch(state)
      {
        case WAIT:
          if (hw_enable.read() == true)
          {
            state = INPUT;
          }
          break;
        case INPUT:
          load.write(true);
          break;
        case EXECUTE:
          t[0] = from_sw0.read();
          t[1] = from_sw1.read();
          c = from_sw2.read();
          aHigh = from_sw3.read();
          /*** Begin: Required part (to do: Datapath + Control) ***/
          cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
          cLow = (NN_HALF_DIGIT)LOW_HALF (c);
      
          u = (NN_DIGIT)aHigh * (NN_DIGIT)cLow;
          v = (NN_DIGIT)aHigh * (NN_DIGIT)cHigh;
          
          if ((t[0] -= TO_HIGH_HALF (u)) > (MAX_NN_DIGIT - TO_HIGH_HALF (u))) t[1]--;
          
          t[1] -= HIGH_HALF (u);
          t[1] -= v;
          state = BONUS;
          break;
        case BONUS:
          /*** Begin: Bonus part (optional: Extra Datapath + Extra Control) ***/
          while ((t[1] > cHigh) || ((t[1] == cHigh) && (t[0] >= TO_HIGH_HALF (cLow)))) {
              if ((t[0] -= TO_HIGH_HALF (cLow)) > MAX_NN_DIGIT - TO_HIGH_HALF (cLow)) t[1]--;
              t[1] -= cHigh;
              aHigh++;
          }
          /*** End: Bonus part ***/
          state = OUTPUT;
          break;
        case OUTPUT:
          to_sw0.write(t[0]);
          to_sw1.write(t[1]);
          to_sw2.write(aHigh);

          hw_done.write(true);

          state = FINISH;
          break;
        case FINISH:
          if (hw_enable.read() == false)
          {
            hw_done.write(false);
            reset.write(false);
            state = WAIT;
          }
          break;
      }
    wait();
  }
	  	  
}

