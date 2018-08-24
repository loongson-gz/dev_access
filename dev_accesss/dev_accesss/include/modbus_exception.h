//
// Created by Fanzhe on 5/29/2017.
//

#ifndef MODBUSPP_ModbusException_H
#define MODBUSPP_ModbusException_H

#include <exception>
using namespace std;


/**
 * MODBS EXCPETION
 */
class ModbusException : public exception {
public:
    string msg;
    virtual const char* what() const throw()
    {
        return "A Error In Modbus Happened!";
    }
};


/**
 * Represent Connection Issue in Class
 */
class ModbusConnectException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Having Modbus Connection Problem";
    }
};


/**
 * Illegal Function      Error response Function 0x01
 */
class ModbusIllegalFunctionException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Illegal Function";
    }
};


/**
 * Illegal Address      Error Response Function 0x02
 */
class ModbusIllegalAddressException: public ModbusException {
public:
    string msg = "test";
    const char* what() const throw()
    {
        return "Illegal Address";
    }
};


/**
 * Illegal Data Vlaue   Error Response Funciton 0x03
 */
class ModbusIllegalDataValueException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Illegal Data Value";
    }
};


/**
 * Server Failure       Error Response Function 0x04
 */
class ModbusServerFailureException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Server Failure";
    }
};


/**
 * Acknowledge          Error Response Function 0x05
 */
class ModbusAcknowledgeException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Acknowledge";
    }
};



/**
 * Server Busy           Error Response Function 0x06
 */
class ModbusServerBusyException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Server Busy";
    }
};


/**
 * Gate Way Problem     Error Response Function 0x0A 0x0B
 */
class ModbusGatewayException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Gateway Problem";
    }
};


/**
 * Buffer Exception
 * Buffer is Too Small for Data Storage
 */
class ModbusBufferException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Size of Buffer Is too Small!";
    }
};


/**
 * Amount Exception
 * Address or Amount Input is Wrong
 */
class ModbusAmountException: public ModbusException {
public:
    virtual const char* what() const throw()
    {
        return "Too many Data!";
    }
};



#endif //MODBUSPP_ModbusException_H
