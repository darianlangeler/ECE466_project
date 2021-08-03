#include "systemc.h"
#include "digit.h"
#include "dh_hw.h"

#define WAIT 0
#define INPUT 1
#define UPDATE 2
#define T1_UPDATE_1 3
#define T1_UPDATE_2 4
#define T1_UPDATE_3 5
#define BONUS 6
#define OUTPUT 7
#define FINISH 8

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
          state = UPDATE;
          break;
        case UPDATE:
          load.write(false);
          comp_mux_control.write(comp_out.read());
          state = T1_UPDATE_1;
          break;
        case T1_UPDATE_1:
          sub_comp_mux_control.write(true);
          state = T1_UPDATE_2;
          break;
        case T1_UPDATE_2:
          sub_comp_mux_control.write(false);
          sub_uhh_mux_control.write(true);
          state = T1_UPDATE_3;
          break;
        case T1_UPDATE_3:
          sub_uhh_mux_control.write(false);
          sub_v_mux_control.write(true);
          // Get values for bonus
          t[0] = from_sw0.read();
          t[1] = from_sw1.read();
          c = from_sw2.read();
          aHigh = from_sw3.read();
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

