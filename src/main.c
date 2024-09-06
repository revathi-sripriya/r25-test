#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <serial.h>
#include <string.h>
#include <parsing.h>

// Function declarations
FILE *open_file(const char *port_name, const char *mode);
void read_SBUS(uint8_t *packet, uint8_t size, FILE *file);
uint16_t *parse_buffer(uint8_t *packet);
int interpolation(uint16_t value);
void write_to_SB(FILE *file, int pwm);
void close_file(FILE *file);

int main(int argc, char** argv) {
    const char *port_name_1 = "/dev/ttyUSB0"; // Replace with your SBUS port
    const char *port_name_2 = "/dev/ttyUSB1"; // Replace with your Sabertooth port
    
    FILE *sbus;
    FILE *sabertooth;

    // Open serial ports
    sbus = open_file(port_name_1, "rb");
    sabertooth = open_file(port_name_2, "wb"); // Use "wb" for binary write

    // to store sbus packets
    uint8_t sbus_packet[15];

    if (!sbus || !sabertooth) {
        // Handle file opening errors
        printf("Error opening files\n");
        return 1;
    }

    // to store value of indiviual RC channel
    uint16_t *channel;

    // Read data from RC transmitter using SBUS
    read_SBUS(sbus_packet, 25, sbus);

    // pwm value after interpolation
    int pwm;

    // Parse SBUS packet
    channel = parse_buffer(sbus_packet);

    // Get PWM range for Sabertooth 1
    pwm = interpolation(channel[0]);

    // Write data to Sabertooth 1
    write_to_SB(sabertooth, pwm);

    // Close the files
    close_file(sbus);
    close_file(sabertooth);

    return 0;
}

// Function to open a file
FILE *open_file(const char *port_name, const char *mode) {
    FILE *file = fopen(port_name, mode);
    if (file == NULL) {
        printf("Error opening file: %s\n", port_name);
        return NULL;
    }
    return file;
}

// Function to read data from SBUS
void read_SBUS(uint8_t *packet, uint8_t size, FILE *file) {
    // Read the packet from the file
    fread(packet, size, 1, file);
}

// Function to parse the SBUS packet
uint16_t *parse_buffer(uint8_t *packet) {
    // Parse the SBUS packet and return the channels
    // (You need to implement this function based on the SBUS protocol)
    // Example:
    static uint16_t channels[16];
    // ... parse packet and populate channels ...
    return channels;
}

// Function to interpolate the value
int interpolation(uint16_t value) {
    // Interpolate the value based on the range
    // (You need to implement this function based on your requirements)
    // Example:
    int interpolated_value = (value * 2000) / 1024; // 1024 for SBUS
    return interpolated_value;
}

// Function to write the PWM value to the Sabertooth motor controller
void write_to_SB(FILE *file, int pwm) {
    // Write the PWM value to the Sabertooth (You need to implement this function based on the Sabertooth protocol)
    // Example:
    fprintf(file, "%d\n", pwm); // Assuming Sabertooth uses line-ending characters
}

// Function to close the file
void close_file(FILE *file) {
    fclose(file);
}
