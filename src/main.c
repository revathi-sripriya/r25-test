#include <stdint.h>
#include <stdio.h>

int main(int argc, char** argv) {
    const char *port_name_1 = "/dev/ttyUSB0"; // Replace with your SBUS port
    const char *port_name_2 = "/dev/ttyUSB1"; // Replace with your Sabertooth port

    FILE *sbus;
    FILE *sabertooth;

    // Open serial ports
    sbus = fopen(port_name_1, "rb");
    sabertooth = fopen(port_name_2, "wb"); // Use "wb" for binary write

    if (!sbus || !sabertooth) {
        // Handle file opening errors
        printf("Error opening files\n");
        return 1;
    }

    uint8_t sbus_packet[25]; // Increased size to 25

    uint16_t *channel;

    // Read data from RC transmitter using SBUS
    read_SBUS(sbus_packet, 25, sbus);

    int pwm;

    // Parse SBUS packet
    channel = parse_buffer(sbus_packet);

    // Get PWM range for Sabertooth 1
    pwm = interpolation(channel[0]);

    // Write data to Sabertooth 1
    write_to_SB(sabertooth, pwm);

    // Close the files
    fclose(sbus);
    fclose(sabertooth);

    return 0;
}

void read_SBUS(uint8_t *packet, uint8_t size, FILE *file) {
    // Read the packet from the file
    fread(packet, 1, size, file);
}

void write_to_SB(FILE *file, int pwm) {
    // Write the PWM value to the Sabertooth (You need to implement this function based on the Sabertooth protocol)
    fprintf(file, "%d\n", pwm); // Assuming Sabertooth uses line-ending characters
}
