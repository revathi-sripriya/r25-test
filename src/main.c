#include <stdint.h>
#include <stdio.h>
#include <serial.h>
#include <parsing.h>

int main(int argc, char** argv) {
    // Get port names from arguments
    char *port_name_1 = argv[1]; // SBUS
    char *port_name_2 = argv[2]; // Sabertooth1

    // File pointers for serial communication
    FILE *sbus;
    FILE *sabertooth;

    // To store SBUS packets and RC channel values
    uint8_t sbus_packet[25]; // Size increased to 25
    uint16_t *channel;       // Pointer to store parsed channel values

    // Open the serial ports for reading and writing
    sbus = fopen(port_name_1, "rb");
    sabertooth = fopen(port_name_2, "wb"); // Binary write

    if (!sbus || !sabertooth) {
        // Handle file opening errors
        printf("Error opening serial ports.\n");
        return 1;
    }

    // Read SBUS data
    read_SBUS(sbus_packet, 25, sbus); // Reads 25 bytes from SBUS

    // Parse the SBUS packet to extract channel values
    channel = parse_buffer(sbus_packet);

    // Perform interpolation to convert channel value to PWM
    int pwm = interpolation(channel[0]);

    // Write PWM to Sabertooth
    write_to_SB(sabertooth, pwm);

    // Close the serial ports
    fclose(sbus);
    fclose(sabertooth);

    return 0;
}

// Function to read SBUS packets from file
void read_SBUS(uint8_t *packet, uint8_t size, FILE *file) {
    // Read the SBUS packet from the file
    fread(packet, 1, size, file);
}

// Function to write PWM values to the Sabertooth
void write_to_SB(FILE *file, int pwm) {
    // Write the PWM value to the Sabertooth
    fprintf(file, "%d\n", pwm); // Assuming Sabertooth uses line-ending characters
}
