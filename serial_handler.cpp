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

#include "serial_handler.h"

//-----------------------------
// Class: serial_handler
//-----------------------------
// Provides storage of serial connection parameters
//-----------------------------

//TODO: Learn how C++ handles conversion of ints to unsigned chars and check handling of negative integers

serial_handler::serial_handler(){
        // ttyS0 is the GPIO serial interface as of Pi 3, comport 0 in rs232 library
        rs_comport_number = DEFAULT_COMPORT;
        // Roomba 405 communicates at 57600, newer models at 115200
        rs_baudrate = DEFAULT_BAUDRATE;
        // Serial mode , 8N1 is 8 data bits, no parity, 1 stop bit
        char temp_array[4] = DEFAULT_SERIAL_MODE;
        for (int i=0; i < 4; i++) {
                rs_mode[i] = temp_array[i];
        }
        // Serial rx buffer
        rs_buffer = new unsigned char[DEFAULT_SERIAL_BUFFER_SIZE];
        rs_buffer_size = DEFAULT_SERIAL_BUFFER_SIZE;

        // Initialize buffer
        for (int i = 0; i < rs_buffer_size; i++) {
                rs_buffer[i] = 0;
        }
}

serial_handler::serial_handler(int rs_c_n, int rs_b, char* rs_m, int rs_b_s){
        rs_comport_number = rs_c_n;
        rs_baudrate = rs_b;
        for (int i=0; i < 4; i++) {
                rs_mode[i] = rs_m[i];
        }
        rs_buffer = new unsigned char[rs_b_s];
        rs_buffer_size = rs_b_s;

        // Initialize buffer
        for (int i = 0; i < rs_buffer_size; i++) {
                rs_buffer[i] = 0;
        }
}

serial_handler::~serial_handler(){
        // Clean up buffer array
        delete[] rs_buffer;
}

#ifndef _UTILITIES_H_INCLUDED
        // TODO
        // Define console_log here
#endif

// Convert byte to binary string and display
// Taken from: http://www.cplusplus.com/forum/general/46392/
// FIXME: This should return the binary instead of just dumping it to cout
void serial_handler::binout(unsigned char _x)
{
        unsigned int _size = sizeof(char) * 8;
        for(unsigned _s = 0; _s < _size; ++_s)
                std::cout << ((_x >> (_size - 1 - _s)) & 1);
}

// Open serial port
int serial_handler::rs_open(){
        // RS232_OpenComport returns 1 if it fails, but I prefer returning -1
        if ( RS232_OpenComport(rs_comport_number, rs_baudrate, rs_mode) != 0 ) {
                return -1;
        } else {
                return 0;
        }
}

// FIXME: Connect this to a timer, approx. 100ms
int serial_handler::rs_poll(){
        // Returns number of bytes copied into rs_buffer from serial interface
        return RS232_PollComport(rs_comport_number, rs_buffer, rs_buffer_size);
}

// Send a single byte
int serial_handler::rs_send(unsigned char rs_send_byte){
        // RS232_SendByte returns 1 if it fails, but I prefer returning -1
        if ( RS232_SendByte(rs_comport_number, rs_send_byte) != 0 ) {
                return -1;
        } else {
                return 0;
        }
}

// Send an array of bytes. Returns number of bytes sent or -1 if it fails
int serial_handler::rs_send_buffer(unsigned char* rs_send_buffer, int rs_send_buf_size){
        return RS232_SendBuf(rs_comport_number, rs_send_buffer, rs_send_buf_size);
}

// Close serial port
void serial_handler::rs_close(){
        RS232_CloseComport(rs_comport_number);
}

// Converts string of ints to array of bytes
int serial_handler::string_to_bytes(std::string s_data_string, unsigned char* raw_bytes){

        // Largest legal command is a 16 note song (34 bytes, or 2N+2 where N = # of notes)
        int int_bytes[34];
        //unsigned char raw_bytes[34];

        // Use istringstream to parse data string
        std::string string_byte = "";
        std::istringstream iss(s_data_string);

        // Convert string bytes to raw bytes (unsigned char)
        unsigned int i = 0;
        while (iss >> string_byte) {
                int_bytes[i] = std::stoi(string_byte);
                raw_bytes[i] = (unsigned char) int_bytes[i];
                i++;
        }
        return i;
}

// Send one byte at a time (converted from integer) with console output
// FIXME: Add error checking
int serial_handler::send_byte(int s_data){
        unsigned char raw_byte = '0';
        raw_byte = (unsigned char) s_data;

        std::cout << "Sending ";
        std::cout << s_data << " ";
        std::cout << std::setbase(16);
        std::cout << "(0x" << s_data << " ";
        std::cout << std::setbase(10);
        binout(s_data);
        std::cout << ")" << std::endl;

        if ( rs_send(raw_byte) != 0) {
                int errsv = errno;
                std::cout << "Failed to send data" << std::endl;
                std::cout << "Errno: " << errsv << std::endl;
                return -1;
        } else {
                return 0;
        }
}

// Send array of bytes with console output
int serial_handler::send_byte_array(unsigned char* raw_bytes, int l){

        // Display data
        std::cout << std::endl << "Sending " << l << " bytes:" << std::endl;
        for (int k=0; k < l; k++) {
                std::cout << "[";
                std::cout << (int) raw_bytes[k] << " ";
                std::cout << std::setbase(16);
                std::cout << "0x" << (int) raw_bytes[k] << " ";
                std::cout << std::setbase(10);
                binout(raw_bytes[k]);
                std::cout << "] ";
        }
        std::cout << std::endl << std::endl;

        // Send raw bytes
        if ( rs_send_buffer(raw_bytes, l) < 0) {
                int errsv = errno;
                std::cout << "Failed to send data" << std::endl;
                std::cout << "Errno: " << errsv << std::endl;
                return -1;
        } else {
                return 0;
        }
}

// Send array of bytes (converted from string of integers) with console output
// NOTE: THIS DOES NOT SEND STRINGS over the serial console. It converts a string of integers into an array of unsigned bytes and sends those
// FIXME: Add error checking
int serial_handler::send_byte_array(std::string s_data_string){

        unsigned char raw_bytes[34];
        int l = string_to_bytes(s_data_string, raw_bytes);

        if ( send_byte_array( raw_bytes, l ) < 0) {
                int errsv = errno;
                std::cout << "Failed to send data" << std::endl;
                std::cout << "Errno: " << errsv << std::endl;
                return -1;
        } else {
                return 0;
        }

}
