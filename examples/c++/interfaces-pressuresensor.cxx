/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <exception>
#include <iostream>
#include <stddef.h>

#include "bme280.hpp"
#include "bmpx8x.hpp"
#include "iPressureSensor.hpp"
#include "mraa/common.h"
#include "upm_utilities.h"

#define EDISON_I2C_BUS 1
#define FT4222_I2C_BUS 0

//! [Interesting]
// Simple example of using iPressureSensor to determine
// which sensor is present and return its name.
// iPressureSensor is then used to get readings from sensor

upm::iPressureSensor*
getPressureSensor()
{
    upm::iPressureSensor* pressureSensor = NULL;
    try {
        pressureSensor = new upm::BME280(mraa_get_sub_platform_id(FT4222_I2C_BUS));
        return pressureSensor;
    } catch (std::exception& e) {
        std::cerr << "BME280: " << e.what() << std::endl;
    }

    try {
        pressureSensor = new upm::BMPX8X(EDISON_I2C_BUS);
        return pressureSensor;
    } catch (std::exception& e) {
        std::cerr << "BMPX8X: " << e.what() << std::endl;
    }
    return pressureSensor;
}

int
main()
{
    upm::iPressureSensor* sensor = getPressureSensor();

    if (sensor == NULL) {
        std::cout << "Pressure sensor not detected" << std::endl;
        return 1;
    }

    std::cout << "Pressure sensor " << sensor->Name() << " detected" << std::endl;

    try {
        std::map<std::string, float> values = sensor->Pressure();
        for (std::map<std::string, float>::const_iterator it = values.begin();
                it != values.end(); ++it)
            std::cout << it->first << " = " << it->second
                << sensor->Unit(it->first) << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    delete sensor;
    return 0;
}

//! [Interesting]
