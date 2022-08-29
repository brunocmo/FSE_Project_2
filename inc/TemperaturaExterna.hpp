#ifndef TEMPERATURAEXTERNA_HPP
#define TEMPERATURAEXTERNA_HPP

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <bme280.h>

class TemperaturaExterna {
    private:
        struct identifier
        {
            /* Variable to hold device address */
            uint8_t dev_addr;

            /* Variable that contains file descriptor */
            int8_t fd;
        };

        struct bme280_dev dev;

        struct identifier id;

        int8_t rslt;
    public: 
        TemperaturaExterna();
        ~TemperaturaExterna();

        static void user_delay_us(uint32_t period, void *intf_ptr);


        static void print_sensor_data(struct bme280_data *comp_data);


        static int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);


        static int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);


        static int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev);
};

#endif