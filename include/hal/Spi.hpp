/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SPI_HPP_
#define SAPI_SPI_HPP_

#include <sos/dev/spi.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"


namespace hal {

/*! \brief SPI Pin Assignment
 * \details This class allows simple manipulation of the spi_pin_assignment_t.
 *
 */
class SpiPinAssignment : public PinAssignment<spi_pin_assignment_t>{};


/*! \brief SPI Class
 * \details This class gives access to a SPI port.
 *
 * \code
 * #include <sapi/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 *   char buffer[16];
 *   Spi spi(0);           //access to SPI port 0
 *   spi.init();           //init SPI with default settings from BSP
 *   spi.read(buffer, 16); //read 16 bytes from the SPI
 *   spi.close();          //close the SPI (power it off)
 *   return 0;
 * }
 * \endcode
 */
class Spi : public Periph<spi_info_t, spi_attr_t, 's'>{
public:

    /*! \details Constructs a SPI object using \a port. */
    Spi(port_t port);

    enum {
        FLAG_IS_FORMAT_SPI = SPI_FLAG_IS_FORMAT_SPI,
        FLAG_IS_FORMAT_TI = SPI_FLAG_IS_FORMAT_TI,
        FLAG_IS_FORMAT_MICROWIRE = SPI_FLAG_IS_FORMAT_MICROWIRE,
        FLAG_IS_MODE0 = SPI_FLAG_IS_MODE0,
        FLAG_IS_MODE1 = SPI_FLAG_IS_MODE1,
        FLAG_IS_MODE2 = SPI_FLAG_IS_MODE2,
        FLAG_IS_MODE3 = SPI_FLAG_IS_MODE3,
        FLAG_SET_MASTER = SPI_FLAG_SET_MASTER,
        FLAG_SET_SLAVE = SPI_FLAG_SET_SLAVE,
        FLAG_IS_FULL_DUPLEX = SPI_FLAG_IS_FULL_DUPLEX,
        FLAG_IS_HALF_DUPLEX = SPI_FLAG_IS_HALF_DUPLEX,

        IS_FORMAT_SPI/*! See \ref SPI_FLAG_IS_FORMAT_SPI */ = SPI_FLAG_IS_FORMAT_SPI,
        IS_FORMAT_TI /*! See \ref SPI_FLAG_IS_FORMAT_TI */ = SPI_FLAG_IS_FORMAT_TI,
        IS_FORMAT_MICROWIRE /*! See \ref SPI_FLAG_IS_FORMAT_MICROWIRE */ = SPI_FLAG_IS_FORMAT_MICROWIRE,
        IS_MODE0 /*! See \ref SPI_FLAG_IS_MODE0 */ = SPI_FLAG_IS_MODE0,
        IS_MODE1 /*! See \ref SPI_FLAG_IS_MODE1 */ = SPI_FLAG_IS_MODE1,
        IS_MODE2 /*! See \ref SPI_FLAG_IS_MODE2 */ = SPI_FLAG_IS_MODE2,
        IS_MODE3 /*! See \ref SPI_FLAG_IS_MODE3 */ = SPI_FLAG_IS_MODE3,
        SET_MASTER /*! See \ref SPI_FLAG_SET_MASTER */ = SPI_FLAG_SET_MASTER,
        SET_SLAVE /*! See \ref SPI_FLAG_SET_SLAVE */ = SPI_FLAG_SET_SLAVE,
        IS_FULL_DUPLEX /*! See \ref SPI_FLAG_IS_FULL_DUPLEX */ = SPI_FLAG_IS_FULL_DUPLEX,
        IS_HALF_DUPLEX /*! See \ref SPI_FLAG_IS_HALF_DUPLEX */ = SPI_FLAG_IS_HALF_DUPLEX,
    };

    /*! \details swap a byte on the SPI bus */
    int swap(int byte) const;

    /*! \details Sets the SPI attributes.
     *
     * @param o_flags SPI Flags
     * @param freq SPI bitrate
     * @param width Data width
     * @param pin_assignment SPI pin assignment
     * @return Zero on success
     */
    int set_attr(u32 o_flags, u32 freq, u32 width = 8, const spi_pin_assignment_t * pin_assignment = 0) const {
        spi_attr_t attr;
        attr.o_flags = o_flags;
        attr.freq = freq;
        if( pin_assignment != 0 ){
            memcpy(&attr.pin_assignment, pin_assignment, sizeof(spi_pin_assignment_t));
        } else {
            memset(&attr.pin_assignment, 0xff, sizeof(spi_pin_assignment_t));
        }
        attr.width = width;
        return Periph::set_attr(attr);
    }

    /*! \details Initializes the SPI port as specified.
     *
     * @param o_flags SPI Flags
     * @param freq SPI bitrate
     * @param width Data width
     * @param pin_assignment SPI pin assignment
     * @return Zero on success	 *
     */
    int init(u32 o_flags, u32 freq, u32 width = 8, const spi_pin_assignment_t * pin_assignment = 0){

        if( open() < 0 ){
            return -1;
        }
        return set_attr(o_flags, freq, width, pin_assignment);
    }

    using Periph::init;
    using Periph::set_attr;

private:

};

}

#endif /* SAPI_SPI_HPP_ */
