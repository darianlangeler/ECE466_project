#include systemc.h

#ifndef _HW_MODULES_H
#define _HW_MODULES_H

void reg:reg_process()
{
	if(reset.read() == true)
	{
		data_out.write(0);
	}
	else
	{
		if(load.read() == true)
		{
			data_out.write(data_in.read());
		}
	}
}

void mux:mux_process()
{
	if (control.read() == true)
	{
		data_out.write(data_in_1.read());
	}
	else
	{
		data_out.write(data_in_2.read());
	}
}

void adder:adder_process()
{
	sum.write(summand_1.read() + summand_2.read());
}

void subber:subber_process()
{
	difference.write(minuend.read() - subtrahend.read());
}

void multiplier:multiplier_process()
{
	product.write(multiplicand_1.read() * multiplicand_2.read());
}

void comparator:comparator_process()
{
	if (reference.read() < comparison.read())
	{
		result.write(true);
	}
	else
	{
		result.write(false);
	}
}