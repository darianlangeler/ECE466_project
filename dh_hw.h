#ifndef _DH_HW_H_
#define _DH_HW_H_ 1

#include "systemc.h"
#include "digit.h"
#include "hw_modules.h"

SC_MODULE (dh_hw)
{
  sc_in_clk clk;

  // Externals
  sc_fifo_in <NN_DIGIT> from_sw0, from_sw1, from_sw2;
  sc_fifo_in <NN_HALF_DIGIT> from_sw3;

  sc_fifo_out <NN_DIGIT> to_sw0, to_sw1;
  sc_fifo_out <NN_HALF_DIGIT> to_sw2;

  // HW-SW Handshake signals
  sc_in <bool> hw_enable; 
  sc_out <bool> hw_done;

  // Controller Signals
  sc_signal<bool> reset, load;

  // Interconnect Signals
  sc_signal<NN_DIGIT> t0, t1, c_high, c_low, u, u_hh, u_hhs, v;
  sc_signal<bool> comp_mux_control, sub_comp_mux_control, sub_uhh_mux_control, sub_v_mux_control, comp_out;
  sc_signal<NN_DIGIT> MAX;
  sc_signal<NN_DIGIT> comp_mux_inA, comp_mux_inB, comp_mux_out, sub_comp_mux_inB,
                      sub_uhh_mux_inA, sub_uhh_mux_out, sub_v_mux_inA,
                      sub_v_mux_out;
  
  // Module Instantiations
  reg t0_reg, t1_reg, c_reg, a_reg;
  splitter c_split;
  multiplier u_mult, v_mult;
  subber mux_sub, uhhmax_sub, uhh_sub, v_sub;
  mux comp_mux, sub_com_mux, sub_uhh_mux, sub_v_mux;
  comparator comp;

  // process thread
  void process_hw();
  
  SC_CTOR (dh_hw): t0_reg("t0_reg"), t1_reg("t1_reg"), c_reg("c_reg"), a_reg("a_reg"),
  c_split("c_split"), u_mult("u_mult"), v_mult("v_mult"), mux_sub("mux_sub"),
  uhh_sub("uhh_sub"), v_sub("v_sub"), mux("mux"), comp("comp")
  {
    // Constants
    MAX.write(MAX_NN_DIGIT);
    comp_mux_inA.write(1);
    comp_mux_inB.write(0);

    // Define register connections
    t0_reg.clk(clk); t0_reg.data_in(from_sw0), t0_reg.data_out(t0), t0_reg.load(load), t0_reg.reset(reset);
    t1_reg.clk(clk); t1_reg.data_in(from_sw1), t1_reg.data_out(t1), t1_reg.load(load), t1_reg.reset(reset);
    c_reg.clk(clk); c_reg.data_in(from_sw2), c_reg.data_out(c), c_reg.load(load), c_reg.reset(reset);
    a_reg.clk(clk); a_reg.data_in(from_sw3), a_reg.data_out(a), a_reg.load(load), a_reg.reset(reset);

    // Define splitter connections
    c_split.input(c); c_split.low(c_low); c_split.high(c_high);

    // Define multiplier connections
    u_mult.multiplicand_1(c_low); u_mult.multiplicand_2(a); u_mult.product(u);
    v_mult.multiplicand_1(c_high); v_mult.multiplicand_2(a); v_mult.product(v);

    // Define subber connections
    mux_sub.minuend(t1); mux_sub.subtrahend(comp_mux_out); mux_sub.difference(comp_sub_inA);
    uhhmax_sub.minuend(MAX); uhhmax_sub.subtrahend(u_hh); uhhmax_sub.difference(u_hhs);
    uhh_sub.minuend(t1); uhh_sub.subtrahend(u_hh); uhh_sub.difference(sub_uhh_mux_inA);
    v_sub.minuend(t1); v_sub.subtrahend(v); v_sub.difference(sub_v_mux_inA);

    // Define mux connections
    comp_mux.data_in_1(comp_mux_inA); comp_mux.data_in_2(comp_mux_inB); comp_mux.data_out(comp_mux_out); comp_mux.control(comp_mux_control);
    sub_comp_mux.data_in_1(comp_mux_out); sub_comp_mux.data_in_2(t1); sub_comp_mux.data_out(t1); sub_comp_mux.control(sub_comp_mux_control);
    sub_uhh_mux.data_in_1(sub_uhh_mux_inA); sub_uhh_mux.data_in_2(t1); sub_uhh_mux.data_out(t1); sub_uhh_mux.control(sub_uhh_mux_control);
    sub_v_mux.data_in_1(sub_v_mux_inA); sub_v_mux.data_in_2(t1); sub_v_mux.data_out(t1); sub_v_mux.control(sub_v_mux_control);

    // Define comparator connections
    comp.reference(u_hhs); comp.comparison(u_hh); comp.result(comp_out);
    
    SC_CTHREAD (process_hw, clk.pos());
  }
  
};

#endif /* end _DH_HW_H_ */
