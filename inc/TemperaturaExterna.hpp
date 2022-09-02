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

extern float temperatura;

class TemperaturaExterna {
    private:

    public: 
        TemperaturaExterna();
        ~TemperaturaExterna();

        float pegarValorTemperatura();


};

void user_delay_us(uint32_t period, void *intf_ptr);


void print_sensor_data(struct bme280_data *comp_data);


int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);


int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);


int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev);

#endif