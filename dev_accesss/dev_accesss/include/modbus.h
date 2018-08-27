#ifndef __MODBUS_H__
#define __MODBUD_H__

#ifndef _UNISTD_H
#define _UNISTD_H
#include <io.h>
#include <process.h>
#endif /* _UNISTD_H */

#include "common.h"
#include <string.h>
#include <iostream>

#define MAX_MSG_LENGTH 260
using namespace std;
typedef  long ssize_t;
typedef unsigned char uint8_t;

//Function Code
enum {
	READ_COILS = 0x01,
	READ_INPUT_BITS = 0x02,
	READ_REGS = 0x03,
	READ_INPUT_REGS = 0x04,
	WRITE_COIL = 0x05,
	WRITE_REG = 0x06,
	WRITE_COILS = 0x0F,
	WRITE_REGS = 0x10,
};

//Exception Codes
enum {
	EX_ILLEGAL_FUNCTION = 0x01, // Function Code not Supported
	EX_ILLEGAL_ADDRESS = 0x02, // Output Address not exists
	EX_ILLEGAL_VALUE = 0x03, // Output Value not in Range
	EX_SERVER_FAILURE = 0x04, // Slave Deive Fails to process request
	EX_ACKNOWLEDGE = 0x05, // Service Need Long Time to Execute
	EX_SERVER_BUSY = 0x06, // Server Was Unable to Accept MB Request PDU
	EX_GATEWAY_PROBLEMP = 0x0A, // Gateway Path not Available
	EX_GATEWYA_PROBLEMF = 0x0B, // Target Device Failed to Response
};

class Modbus {
private:
	bool _connected;
	uint16_t PORT;
	int _socket;
	int _msg_id;
	int _slaveid;
	string HOST;
	struct sockaddr_in _server;
	void ModbusBuildRequest(uint8_t * to_send, int address, int func);
	void ModbusWrite(int address, int amount, int func, uint16_t * value);
	void ModbusRead(int address, int amount, int func);
	ssize_t ModbusSend(uint8_t * to_send, int length);
	ssize_t ModbusReceive(uint8_t * buffer);
	void ModbusErrorHandle(uint8_t * msg, int func);

public:
	Modbus();
	Modbus(string host, uint16_t port);
	Modbus(string host);
	~Modbus();
	void ModbusSetSlaveId(int id);
	bool ModbusConnect();
	void ModbusClose();


	void ModbusReadHoldingRegisters(int address, int amount, uint16_t * buffer);
	void ModbusReadInputRegisters(int address, int amount, uint16_t * buffer);
	void ModbusReadCoils(int address, int amount, bool * buffer);
	void ModbusReadInputBits(int address, int amount, bool * buffer);
	void ModbusWriteCoil(int address, bool to_write);
	void ModbusWriteRegister(int address, uint16_t value);
	void ModbusWriteCoils(int address, int amount, bool * value);
	void ModbusWriteRegisters(int address, int amount, uint16_t * value);

};

typedef shared_ptr<Modbus> ModbusPtr;

#endif // !__MODBUS_H__
