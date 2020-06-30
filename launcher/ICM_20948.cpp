extern "C"
{
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
}
#include "ICM_20948.h"

ICM20948::ICM20948(uint16_t device_address): _devAddress(device_address)
{

}

ICM20948::~ICM20948()
{

}

bool ICM20948::init()
{
    //enable I2C module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);


    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

uint8_t ICM20948::read_register(uint16_t device_register)
{
    //specify that we want to communicate to device address with an intended write to bus
    I2CMasterSlaveAddrSet(I2C0_BASE, _devAddress, false);

    //the register to be read
    I2CMasterDataPut(I2C0_BASE, device_register);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    //wait for MCU to complete send transaction
    while(I2CMasterBusy(I2C0_BASE));

    //read from the specified slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, _devAddress, true);

    //send control byte and read from the register from the MCU
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //wait while checking for MCU to complete the transaction
    while(I2CMasterBusy(I2C0_BASE));

    //Get the data from the MCU register and return to caller
    return( I2CMasterDataGet(I2C0_BASE));
}

void ICM20948::write_register(uint16_t device_register, uint8_t device_data)
{
    //specify that we want to communicate to device address with an intended write to bus
    I2CMasterSlaveAddrSet(I2C0_BASE, _devAddress, false);

    //register to be read
    I2CMasterDataPut(I2C0_BASE, device_register);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterSlaveAddrSet(I2C0_BASE, _devAddress, true);

    //specify data to be written to the above mentioned device_register
    I2CMasterDataPut(I2C0_BASE, device_data);

    //wait while checking for MCU to complete the transaction
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

    //wait for MCU & device to complete transaction
    while(I2CMasterBusy(I2C0_BASE));
}

void ICM20948::select_bank(uint8_t bank)
{

    

}

uint8_t ICM20948::getAccelerometer_ID() {
    return (read_register(0x00));
}