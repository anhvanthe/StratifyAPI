/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef EMA_HPP_
#define EMA_HPP_

#include <mcu/types.h>
#include <cstdio>

namespace Calc {

/*! \brief Macro for creating the alpha value for 32-bit calculations */
/*! \details This value can be set between 0.0 and 1.0.  More weight is
 * given to older samples when the valeu is close to 0.0.  When the value is set
 * to 1.0, no averaging occurs.
 */
#define DSP_EMA_I32_ALPHA(x) ( (u16)(x * 65535) )

/*! \brief Macro for creating the alpha avelu for 16-bit calculations */
/*! \details See \ref DSP_EMA_I32_ALPHA for details */
#define DSP_EMA_I16_ALPHA(x) ( (u8)(x * 255) )

/*! \brief Exponential Moving Average template class */
/*! \details This class is a template for an exponential moving average (EMA)
 * calculation.  The EMA is a type of low pass filter and is helpful when trying
 * to smooth out data that is sampled on the ADC.
 *
 * The following is an example of using the Ema class using 32-bit integers.
 * \code
 * #include <stfy/Hal.hpp>
 * #include <stfy/Calc.hpp>
 * #include <cstdio>
 *
 *  //first initialize a filter with a value of 0 and with an averaging constant of 0.1
 * Ema_i32 filter(0, DSP_EMA_I32_ALPHA(0.1));
 *  //Or do:  Ema<u16, i32, i64> filter(0, DSP_EMA_I32_ALPHA(0.1));
 *
 *  //now use the ADC to get some data
 * Adc adc(0);
 * adc_sample_t sample;
 * adc.init(1<<0); //initialize with channel 0 enabled
 *
 * for(i=0; i < 100; i++){  //take 100 samples and filter as we go
 * 	adc.read(0, &sample, sizeof(sample));
 * 	filter.calc(sample);
 * 	printf("%d %d %d;\n", i, sample, filter.avg());
 * }
 * \endcode
 *
 * The filter is easy-to-use and uses minimal resources.  For more information on
 * EMA filtering see <a href="http://stratifylabs.co/embedded%20design%20tips/2013/10/04/Tips-An-Easy-to-Use-Digital-Filter">this wiki article</a>.
 *
 */
template<typename intsmall, typename intmedium, typename intlarge>class Ema {
public:
	/*! \details Construct a new Ema object
	 *
	 * @param start Initial value
	 * @param alpha Averaging value
	 */
	Ema(intmedium start, intsmall alpha){ _average = start;  this->alpha = alpha; }
	static intmedium small_max(){ return 1<<(8*sizeof(intsmall)); }

	/*! \details Calculate the next average using an input value
	 *
	 * @param in Input value
	 * @return The updated average (same as average())
	 */
	intmedium calc(intmedium in){
		intlarge tmp0;
		tmp0 = (intlarge)in * (alpha) + (intlarge)_average * (small_max() - alpha);
		_average = (intmedium)(((intlarge)tmp0 + (intlarge)small_max()) >> (sizeof(intsmall)*8));
		return _average;
	}

	/*! \details Access the current average (no calculations are made here)
	 *
	 * @return The current average value
	 */
	intmedium average() const { return _average; }
	intmedium avg() const { return _average; }

	/*! \details Set the average value.  This method can be
	 * used to update the initial value
	 *
	 * @param v The new initial value
	 */
	void set(intmedium v){ _average = v; }
private:
	intmedium _average;
	intsmall alpha;
};

/*! \brief Exponential Moving Average class (i32) */
/*! \details See \ref Ema for details */
class Ema_i32 : public Ema<u16, i32, i64> {
public:
	Ema_i32(i32 start, u16 alpha) : Ema(start, alpha){}
};

/*! \brief Exponential Moving Average class (i16) */
/*! \details See \ref Ema for details */
class Ema_i16 : public Ema<u8, i16, i32> {
public:
	Ema_i16(i16 start, u8 alpha) : Ema(start, alpha){}
};

/*! \brief Exponential Moving Average class (u32) */
/*! \details See \ref Ema for details */
class Ema_u32 : public Ema<u16, u32, u64> {
public:
	Ema_u32(u32 start, u16 alpha) : Ema(start, alpha){}
};

/*! \brief Exponential Moving Average class (u16) */
/*! \details See \ref Ema for details */
class Ema_u16 : public Ema<u8, u16, u32> {
public:
	Ema_u16(u16 start, u8 alpha) : Ema(start, alpha){}
};

/*! \brief Exponential Moving Average class (float) */
/*! \details See \ref Ema for details */
class Ema_float {
public:
	Ema_float(float start, float alpha){ this->alpha = alpha; average_ = start; }
	static float small_max(){ return 1.0; }
	float calc(float in){
		float tmp;
		average_ = in * (alpha) + average_ * (1.0 - alpha);
		return average_;
	}
	float avg() const { return average_; }
	float average() const { return average_; }
	void set(float v){ average_ = v; }
private:
	float average_;
	float alpha;
};


}

/* namespace Dsp */
#endif /* EMA_HPP_ */