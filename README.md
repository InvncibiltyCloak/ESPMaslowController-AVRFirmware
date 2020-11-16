# ESPMaslowController-AVRFirmware
The firmware for the ATmega328PB that is included in the ESPMaslowController project.

The ATmega328PB mostly just acts as a GPIO expander. However, since it is fully programmable, there is the possibility for additional behavior in the future.

## Command Protocol
Communication is via the SPI port. Each transaction should be 3 bytes long (any additional bytes will be ignored). The SS line is used for synchronization.

The first byte determines the command. The remaining two byte provide parameters which depend on the command.

### 0x00 = Null Command
This command is ignored.

### 0x01 thru 0x05 = Motor Setting
Command values of 1 through 5 will command motors 1 through 5.
The second byte of the transaction sets the output value for the motor.

The third byte is a series of flags. Bit 0 of the command sets the direction of the motor. The remaining bits are ignored.

## TODO

1. Have the ATmega328 act as a watchdog to turn off all outputs when no command is received after a certain amount of time.

2. 



