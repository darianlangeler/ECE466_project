#include "systemc.h"
#include "digit.h"
#include "dh_hw.h"

#define WAIT 0
#define EXECUTE 1
#define OUTPUT 2
#define FINISH 3

void dh_hw::process_hw()
{

  NN_DIGIT t[2], c, u, v;
  NN_HALF_DIGIT aHigh, cLow, cHigh;
  
  // initially not done and in wait state
  hw_done.write(false);
  state = WAIT;
  for (;;) {  
      
      switch(state)
      {
        case WAIT:
          if (hw_enable.read() == true)
          {
            state = EXECUTE;
          }
          break;
        case EXECUTE:
          t[0] = from_sw0.read();
          t[1] = from_sw1.read();
          c = from_sw2.read();
          aHigh = from_sw3.read();
          // Original code from NN_DigitDivHH():  

          /*** Begin: Required part (to do: Datapath + Control) ***/
          cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
          cLow = (NN_HALF_DIGIT)LOW_HALF (c);
      
          u = (NN_DIGIT)aHigh * (NN_DIGIT)cLow;
          v = (NN_DIGIT)aHigh * (NN_DIGIT)cHigh;
          
          if ((t[0] -= TO_HIGH_HALF (u)) > (MAX_NN_DIGIT - TO_HIGH_HALF (u))) t[1]--;
          
          t[1] -= HIGH_HALF (u);
          t[1] -= v;
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
            state = WAIT;
          }
          break;
      }
      wait();

      /*** End: Required part ***/

      /*** Begin: Bonus part (optional: Extra Datapath + Extra Control) ***/
      while ((t[1] > cHigh) || ((t[1] == cHigh) && (t[0] >= TO_HIGH_HALF (cLow)))) {
          if ((t[0] -= TO_HIGH_HALF (cLow)) > MAX_NN_DIGIT - TO_HIGH_HALF (cLow)) t[1]--;
          t[1] -= cHigh;
          aHigh++;
      }
      /*** End: Bonus part ***/
		
      // Write outputs (blocking FIFO access)


  }
	  	  
}

