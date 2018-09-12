#define WIN32_LEAN_AND_MEAN
#include "socketapi/SocketAPI.h"

#include "modbus.h"
#include "modbus_exception.h"


/**
* Main Constructor of Modbus Object
* @param host IP Address of Host
* @param port Port for Connection
* @return     A Modbus Object
*/
Modbus::Modbus(string host, uint16_t port)
	:_socket(-1)
	,_server()
	,HOST(host)
	,PORT(port)
	,_slaveid(1)
	,_msg_id(1)
	,_connected(false)
{
}

/**
* Overloading Modbus Constructor with Default Port at 502
* @param host  IP Address of Host
* @return      A Modbus Object
*/
Modbus::Modbus(string host) 
{
	this->Modbus::Modbus(host, 502);
}


/**
* Destructor of Modbus Object
*/
Modbus::~Modbus(void) 
{
}


/**
* Set Slave ID
* @param id  Id of Slave in Server to Set
*/
void Modbus::ModbusSetSlaveId(int id) 
{
	_slaveid = id;
}

/**
* Build up Connection
* @return   If A Connection Is Successfully Built
*/
bool Modbus::ModbusConnect() 
{
	if (HOST == "" || PORT == 0) 
	{
		WLogError("%s Missing Host and Port", __FUNCTION__);
		return false;
	}
	else 
	{
		WLogInfo("%s Found Proper Host %s and Port %d", __FUNCTION__, HOST.c_str(), PORT);
	}

	if (InitSocket())
	{
		_socket = Connect(HOST.c_str(), PORT, SOCK_STREAM);
		if (_socket == -1)
		{
			WLogError("%s Cannot Connected to the %s", __FUNCTION__, HOST.c_str());
			return false;
		}
		else
		{
			WLogInfo("%s Connected to the %s successful", __FUNCTION__, HOST.c_str());
		}
	}
	else
	{
		return false;
	}

	_connected = true;
	return true;
}


/**
* Close the Connection
*/
void Modbus::ModbusClose() 
{
	Close(_socket);
	WLogDebug("Socket Closed");
}


/**
* Modbus Request Builder
* @param to_send   Message Buffer to Send
* @param address   Reference Address
* @param func      Functional Code
*/
void Modbus::ModbusBuildRequest(uint8_t *to_send, int address, int func) 
{
	to_send[0] = (uint8_t)(_msg_id >> 8);
	to_send[1] = (uint8_t)(_msg_id & 0x00FF);
	to_send[2] = 0;
	to_send[3] = 0;
	to_send[4] = 0;
	to_send[6] = (uint8_t)_slaveid;
	to_send[7] = (uint8_t)func;
	to_send[8] = (uint8_t)(address >> 8);
	to_send[9] = (uint8_t)(address & 0x00FF);
}


/**
* Write Request Builder and Sender
* @param address   Reference Address
* @param amount    Amount to Write
* @param func      Functional Code
* @param value     Value to Write
*/
void Modbus::ModbusWrite(int address, int amount, int func, uint16_t *value) 
{
	if (func == WRITE_COIL || func == WRITE_REG) 
	{
		uint8_t *to_send = new uint8_t[12];
		ModbusBuildRequest(to_send, address, func);
		to_send[5] = 6;
		to_send[10] = (uint8_t)(value[0] >> 8);
		to_send[11] = (uint8_t)(value[0] & 0x00FF);
		ModbusSend(to_send, 12);
		delete[] to_send;
	}
	else if (func == WRITE_REGS) 
	{
		uint8_t *to_send = new uint8_t[13 + 2 * amount];
		ModbusBuildRequest(to_send, address, func);
		to_send[5] = (uint8_t)(5 + 2 * amount);
		to_send[10] = (uint8_t)(amount >> 8);
		to_send[11] = (uint8_t)(amount & 0x00FF);
		to_send[12] = (uint8_t)(2 * amount);
		for (int i = 0; i < amount; i++) 
		{
			to_send[13 + 2 * i] = (uint8_t)(value[i] >> 8);
			to_send[14 + 2 * i] = (uint8_t)(value[i] & 0x00FF);
		}
		ModbusSend(to_send, 13 + 2 * amount);
		delete[] to_send;
	}
	else if (func == WRITE_COILS) 
	{
		uint8_t *to_send = new uint8_t[14 + (amount - 1) / 8];
		ModbusBuildRequest(to_send, address, func);
		to_send[5] = (uint8_t)(7 + (amount - 1) / 8);
		to_send[10] = (uint8_t)(amount >> 8);
		to_send[11] = (uint8_t)(amount >> 8);
		to_send[12] = (uint8_t)((amount + 7) / 8);
		for (int i = 0; i < amount; i++) 
		{
			to_send[13 + (i - 1) / 8] += (uint8_t)(value[i] << (i % 8));
		}
		ModbusSend(to_send, 14 + (amount - 1) / 8);
		delete[] to_send;
	}
}



/**
* Read Requeset Builder and Sender
* @param address   Reference Address
* @param amount    Amount to Read
* @param func      Functional Code
*/
void Modbus::ModbusRead(int address, int amount, int func)
{
	try
	{
		uint8_t *to_send = new uint8_t[12];
		ModbusBuildRequest(to_send, address, func);
		to_send[5] = 6;
		to_send[10] = (uint8_t)(amount >> 8);
		to_send[11] = (uint8_t)(amount & 0x00FF);
		ModbusSend(to_send, 12);
		delete[] to_send;
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}


/**
* Read Holding Registers           MODBUS FUNCTION 0x03
* @param address    Reference Address
* @param amount     Amount of Registers to Read
* @param buffer     Buffer to Store Data
*/
void Modbus::ModbusReadHoldingRegisters(int address, int amount, uint16_t *buffer) 
{
	if (_connected) 
	{
		if (amount > 65535 || address > 65535) 
		{
			throw ModbusAmountException();
		}
		ModbusRead(address, amount, READ_REGS);
		uint8_t to_rec[MAX_MSG_LENGTH] = {0};
		ssize_t k = ModbusReceive(to_rec);
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		try 
		{
			ModbusErrorHandle(to_rec, READ_REGS);
			for (int i = 0; i < amount; i++) 
			{
				buffer[i] = ((uint16_t)to_rec[9 + 2 * i]) << 8;
				buffer[i] += (uint16_t)to_rec[10 + 2 * i];
			}
		}
		catch (exception &e) 
		{
			throw e;
		}
	}
	else 
	{
		throw ModbusConnectException();
	}
}



/**
* Read Input Registers             MODBUS FUNCTION 0x04
* @param address     Reference Address
* @param amount      Amount of Registers to Read
* @param buffer      Buffer to Store Data
*/
void Modbus::ModbusReadInputRegisters(int address, int amount, uint16_t *buffer) 
{
	if (_connected) 
	{
		if (amount > 65535 || address > 65535) 
		{
			throw ModbusAmountException();
		}
		ModbusRead(address, amount, READ_INPUT_REGS);
		uint8_t to_rec[MAX_MSG_LENGTH] = {0};
		ssize_t k = ModbusReceive(to_rec);
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		try 
		{
			ModbusErrorHandle(to_rec, READ_INPUT_REGS);
			for (int i = 0; i < amount; i++) 
			{
				buffer[i] = ((uint16_t)to_rec[9 + 2 * i]) << 8;
				buffer[i] += (uint16_t)to_rec[10 + 2 * i];
			}
		}
		catch (exception &e) 
		{
			throw e;
		}
	}
	else 
	{
		throw ModbusConnectException();
	}
}


/**
* Read Coils           MODBUS FUNCTION 0x01
* @param address     Reference Address
* @param amount      Amount of Coils to Read
* @param buffer      Buffers to Store Data
*/
void Modbus::ModbusReadCoils(int address, int amount, bool *buffer) 
{
	if (_connected) 
	{
		if (amount > 2040 || address > 65535) 
		{
			throw ModbusAmountException();
		}
		ModbusRead(address, amount, READ_COILS);
		uint8_t to_rec[MAX_MSG_LENGTH] = {0};
		ssize_t k = ModbusReceive(to_rec);
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		try 
		{
			ModbusErrorHandle(to_rec, READ_COILS);
			for (int i = 0; i < amount; i++) 
			{
				buffer[i] = (bool)((to_rec[9 + i / 8] >> (i % 8)) & 1);
			}
		}
		catch (exception &e) 
		{
			throw e;
		}
	}
	else 
	{
		throw ModbusConnectException();
	}
}




/**
* Read Input Bits(Discrete Data)      MODBUS FUNCITON 0x02
* @param address   Reference Address
* @param amount    Amount of Bits to Read
* @param buffer    Buffer to store Data
*/
void Modbus::ModbusReadInputBits(int address, int amount, bool* buffer) 
{
	if (_connected) 
	{
		if (amount > 2040 || address > 65535) 
		{
			throw ModbusAmountException();
		}
		ModbusRead(address, amount, READ_INPUT_BITS);
		uint8_t to_rec[MAX_MSG_LENGTH] = {0};
		ssize_t k = ModbusReceive(to_rec);
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		try 
		{
			ModbusErrorHandle(to_rec, READ_INPUT_BITS);
			for (int i = 0; i < amount; i++) 
			{
				buffer[i] = (bool)((to_rec[9 + i / 8] >> (i % 8)) & 1);
			}
		}
		catch (exception &e) 
		{
			throw e;
		}
	}
	else 
	{
		throw ModbusConnectException();
	}
}


/**
* Write Single Coils         MODBUS FUNCTION 0x05
* @param address    Reference Address
* @param to_write   Value to Write to Coil
*/
void Modbus::ModbusWriteCoil(int address, bool to_write) 
{
	if (_connected) 
	{
		if (address > 65535) 
		{
			throw ModbusAmountException();
		}
		int value = to_write * 0xFF00;
		ModbusWrite(address, 1, WRITE_COIL, (uint16_t *)&value);
		uint8_t to_rec[MAX_MSG_LENGTH] = {0};
		ssize_t k = ModbusReceive(to_rec);
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		try 
		{
			ModbusErrorHandle(to_rec, WRITE_COIL);
		}
		catch (exception &e) 
		{
			throw e;
		}
	}
	else 
	{
		throw ModbusConnectException();
	}
}



/**
* Write Single Register        FUCTION 0x06
* @param address   Reference Address
* @param value     Value to Write to Register
*/
void Modbus::ModbusWriteRegister(int address, uint16_t value) 
{
	if (_connected) 
	{
		if (address > 65535) 
		{
			throw ModbusAmountException();
		}
		ModbusWrite(address, 1, WRITE_REG, &value);
		uint8_t to_rec[MAX_MSG_LENGTH] = {0};
		ssize_t k = ModbusReceive(to_rec);
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		try 
		{
			ModbusErrorHandle(to_rec, WRITE_COIL);
		}
		catch (exception &e) 
		{
			throw e;
		}
	}
	else 
	{
		throw ModbusConnectException();
	}
}


/**
* Write Multiple Coils        MODBUS FUNCTION 0x0F
* @param address  Reference Address
* @param amount   Amount of Coils to Write
* @param value    Values to Write
*/
void Modbus::ModbusWriteCoils(int address, int amount, bool *value) 
{
	if (_connected) 
	{
		if (address > 65535 || amount > 65535) 
		{
			throw ModbusAmountException();
		}
		uint16_t *temp = new uint16_t[amount];
		for (int i = 0; i < 4; i++) 
		{
			temp[i] = (uint16_t)value[i];
		}
		ModbusWrite(address, amount, WRITE_COILS, temp);
		delete[] temp;
		uint8_t to_rec[MAX_MSG_LENGTH] = {0};
		ssize_t k = ModbusReceive(to_rec);
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		try 
		{
			ModbusErrorHandle(to_rec, WRITE_COILS);
		}
		catch (exception &e) 
		{
			throw e;
		}
	}
	else 
	{
		throw ModbusConnectException();
	}
}




/**
* Write Multiple Registers    MODBUS FUNCION 0x10
* @param address Reference Address
* @param amount  Amount of Value to Write
* @param value   Values to Write
*/
void Modbus::ModbusWriteRegisters(int address, int amount, uint16_t *value) 
{
	if (_connected) 
	{
		if (address > 65535 || amount > 65535) 
		{
			throw ModbusAmountException();
		}
		ModbusWrite(address, amount, WRITE_REGS, value);
		uint8_t to_rec[MAX_MSG_LENGTH] = {0};
		ssize_t k = ModbusReceive(to_rec);
		if (k < 0)
		{
			throw ModbusConnectException();
		}
		try 
		{
			ModbusErrorHandle(to_rec, WRITE_REGS);
		}
		catch (exception &e) 
		{
			throw e;
		}
	}
	else 
	{
		throw ModbusConnectException();
	}
}


/**
* Data Sender
* @param to_send Requeset to Send to Server
* @param length  Length of the Request
* @return        Size of the request
*/
ssize_t Modbus::ModbusSend(uint8_t *to_send, int length) 
{
	_msg_id++;
	return Send(_socket, (char *)to_send, (size_t)length);
}


/**
* Data Receiver
* @param buffer Buffer to Store the Data
* @return       Size of the Incoming Data
*/
ssize_t Modbus::ModbusReceive(uint8_t *buffer) 
{
	ssize_t ret = 0;
	ret = Recv(_socket, (char *)buffer, 1024);
	if (ret > 1024)
	{
		return -1;
	}
	return ret;
}


/**
* Error Code Handler
* @param msg   Message Received from Server
* @param func  Modbus Functional Code
*/
void Modbus::ModbusErrorHandle(uint8_t *msg, int func) 
{
	if (msg[7] == func + 0x80) 
	{
		switch (msg[8]) 
		{
		case EX_ILLEGAL_FUNCTION:
			throw ModbusIllegalFunctionException();
		case EX_ILLEGAL_ADDRESS:
			throw ModbusIllegalAddressException();
		case EX_ILLEGAL_VALUE:
			throw ModbusIllegalDataValueException();
		case EX_SERVER_FAILURE:
			throw ModbusServerFailureException();
		case EX_ACKNOWLEDGE:
			throw ModbusAcknowledgeException();
		case EX_SERVER_BUSY:
			throw ModbusServerBusyException();
		case EX_GATEWAY_PROBLEMP:
		case EX_GATEWYA_PROBLEMF:
			throw ModbusGatewayException();
		default:
			break;
		}
	}
}



