#include systemc.h

#ifndef _HW_MODULES_H
#define _HW_MODULES_H

SC_MODULE(reg)
{
	sc_in_clk clk;
	sc_in <NN_DIGIT> data_in;
	sc_in <NN_DIGIT> data_out;
	sc_in <bool> reset;
	sc_in <bool> load;

	void reg_process();
	SC_CTOR(reg)
	{
		SC_CTHREAD(reg_process, clk.pos());
	}
};

SC_MODULE(mux)
{
	sc_in <NN_DIGIT> data_in_1;
	sc_in <NN_DIGIT> data_in_2;
	sc_in <bool> control;
	sc_out <NN_DIGIT> data_out

	void mux_process();
	SC_CTOR(mux)
	{
		SC_THREAD(mux_process);
		sensitive << control << data_in_1 << data_in_2;
	}
};

SC_MODULE(adder)
{
	sc_in <NN_DIGIT> summand_1;
	sc_in <NN_DIGIT> summand_2;
	sc_out <NN_DIGIT> sum;

	void adder_process();
	SC_CTOR(adder)
	{
		SC_THREAD(adder_process);
		sensitive << summand_1 << summand_2;
	}
};

SC_MODULE(subber)
{
	sc_in <NN_DIGIT> minuend;
	sc_in <NN_DIGIT> subtrahend;
	sc_out <NN_DIGIT> difference;

	void subber_process();
	SC_CTOR(subber)
	{
		SC_THREAD(subber_process);
		sensitive << minuend << subtrahend;
	}
};

SC_MODULE (multiplier)
{
	sc_in <NN_DIGIT> multiplicand_1;
	sc_in <NN_DIGIT> multiplicand_2;
	sc_out <NN_DIGIT> product;

	void multiplier_process();
	SC_CTOR(multiplier)
	{
		SC_THREAD(multiplier_process);
		sensitive << multiplicand_1 << multiplicand_2;
	}
};

SC_MODULE (comparator)
{
	sc_in <NN_DIGIT> reference;
	sc_in <NN_DIGIT> comparison;
	sc_out <bool> result;

	void comparator_process();
	SC_CTOR(comparator)
	{
		SC_THREAD(comparator_process);
		sensitive << reference << comparison;
	}
};

SC_MODULE (splitter)
{
	sc_in <NN_DIGIT> input;
	sc_out <NN_DIGIT> low;
	sc_out <NN_DIGIT> high;

	void splitter_process();
	SC_CTOR(splitter)
	{
		SC_THREAD(splitter_process);
		sensitive << input;
	}
};

SC_MODULE (highhalf)
{
	sc_in <NN_DIGIT> input;
	sc_out <NN_DIGIT> output;

	void highhalf_process();
	SC_CTOR(highhalf)
	{
		SC_THREAD(highhalf_process);
		sensitive << input;
	}
};

SC_MODULE (tohighhalf)
{
	sc_in <NN_DIGIT> input;
	sc_out <NN_DIGIT> output;

	void tohighhalf_process();
	SC_CTOR(tohighhalf)
	{
		SC_THREAD(tohighhalf_process);
		sensitive << input;
	}
};