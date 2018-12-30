//--------------------------
// Serial module header
//--------------------------
// Description:
// Provides basic serial console functions
//--------------------------
// Author: Layne Bernardo
// Email: retnuh66@gmail.com
//
// Created July 24th, 2018
// Modified: August 11th, 2018
// Version 0.2
//--------------------------


#ifndef _SERIAL_HANDLER_H_INCLUDED
#define _SERIAL_HANDLER_H_INCLUDED

// Default settings
//-----------------------------
// Serial comm port (ttyS0 is the GPIO serial interface as of Pi 3, comport 0 in rs232 library)
#define DEFAULT_COMPORT 0
// Startup baudrate (Roomba 405 communicates at 57600, newer models at 115200)
#define DEFAULT_BAUDRATE 57600
// Default mode (8N1 is 8 data bits, no parity, 1 stop bit)
#define DEFAULT_SERIAL_MODE {'8', 'N', '1', '\0'}
// Number of bytes to buffer from serial connections
#define DEFAULT_SERIAL_BUFFER_SIZE 4096
//-----------------------------

// Includes
//-----------------------------
// Standard libraries
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
//-----------------------------
// Serial interface library
#include "rs232/rs232.h"
//-----------------------------

// Provides preset parameters for serial connection to Roomba
class serial_handler
{
  public:
    // ttyS0 is the GPIO serial interface as of Pi 3, comport 0 in rs232 library
    int rs_comport_number;
    // Roomba 405 communicates at 57600, newer models at 115200
    int rs_baudrate;
    // Serial mode , 8N1 is 8 data bits, no parity, 1 stop bit
    char rs_mode[4];

    // Serial rx buffer
    unsigned char* rs_buffer;
    // Serial buffer size
    int rs_buffer_size;

    serial_handler();
    serial_handler(int rs_c_n, int rs_b, char* rs_m, int rs_b_s);
    ~serial_handler();

    // Converts decimal to binary and dumps it to cout
    void binout(unsigned char);

    // Open serial port
    int rs_open();

    // Connect this to a timer, approx. 100ms
    int rs_poll();

    // Send a single byte
    int rs_send(unsigned char rs_send_byte);

    // Send an array of bytes. Returns number of bytes sent or -1 if it fails
    int rs_send_buffer(unsigned char* rs_send_buffer, int rs_send_buf_size);

    // Close serial port
    void rs_close();

    int send_byte(int);

    int string_to_bytes(std::string s_data_string, unsigned char* raw_bytes);

    int send_byte_array(unsigned char*, int);
    int send_byte_array(std::string);

};

#endif // serial_handler.h
