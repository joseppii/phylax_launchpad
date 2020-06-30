#ifndef ICM_20948_H
#define ICM_20948_H

#include <stdint.h>

class ICM20948
{
    uint8_t read_register(uint16_t device_register);
    void write_register(uint16_t device_register, uint8_t data);
    void select_bank(uint8_t bank);
public:
    ICM20948(uint16_t device_address);
    ~ICM20948();

    bool init();
    uint8_t getAccelerometer_ID();

private:
    uint16_t _devAddress;
};


#endif