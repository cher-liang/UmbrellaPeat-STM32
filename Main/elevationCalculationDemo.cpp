#include <stdio.h>
#include <math.h>
#define PI 3.14159265
int main()
{
    int angle, reference, difference;
    float elevation;
    int arm_length = 17;

    while (1)
    {
        // Read the reference angle from the sensor
        printf("Enter the reference angle from the sensor (0-360): ");
        scanf("%d", &reference);

        // Read the angle from the sensor
        printf("Enter the angle from the sensor (0-360): ");
        scanf("%d", &angle);

        // Calculate the new angle relative to the reference angle
        difference = 180 - abs(abs(reference - angle) - 180);

        // Print the new anglegle to the console
        printf("Angle difference from the sensor is: %d\n", difference);

        elevation=arm_length*cos(difference*PI/180.0);
        printf("Arm length: %d\nElevation: %.2f\n",arm_length,elevation);
    }

    return 0;
}