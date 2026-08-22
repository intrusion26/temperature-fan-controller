#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int Ledstate = 0;
int Fanspeed = 0;
int Buzzerstate = 0;
int config = 1;
int SystemPower = 1;

void systemControl(float temperature);
void setpointControl(float temperature, float setpoint);
void displaySystem(float temperature);
void alertSignal();

int main() {
     int config;
     float temperature = 0;
     int manualFanSpeed;

     srand(time(0)); // for sensors
     while (1) {

     printf("0. Standby\n");
     printf("1. Auto Config\n");
     printf("2. Manual Config\n");
     printf("3. Exit\n");
     printf("Please select your choice: \n");

     while (scanf("%d", &config) != 1) {
         printf("Invalid input. Please enter a number (0-3): \n");
         while (getchar() != '\n');   // clear the bad input out of the buffer
     }

     if (config == 1 ){
            printf("AUTOMATED MODE\n");
            printf("1. Fixed Threshold Mode\n");
            printf("2. Setpoint Mode\n");
            printf("Please select your choice: \n");

            int autoSubMode;
            while (scanf("%d", &autoSubMode) != 1 || (autoSubMode != 1 && autoSubMode != 2)) {
                printf("Invalid input. Please enter 1 or 2: \n");
                while (getchar() != '\n');
            }

            temperature = rand() % 35;
            printf("Simulated Temperature: %.2f°C\n", temperature);
            for (int i = 0; i < 100000000; i++);

            if (autoSubMode == 1) {
                systemControl(temperature);
            } else {
                float setpointTarget;
                printf("Enter target temperature (setpoint): \n");
                while (scanf("%f", &setpointTarget) != 1) {
                    printf("Invalid input. Please enter a number: \n");
                    while (getchar() != '\n');
                }
                setpointControl(temperature, setpointTarget);
            }

     } else if  ( config == 2 ) {
         printf("MANUAL MODE\n");
         printf("Enter Fan speed (0 - 5): \n");

         while (scanf("%d", &manualFanSpeed) != 1 || manualFanSpeed < 0 || manualFanSpeed > 5) {
             printf("Invalid input. Please enter a whole number (0-5): \n");
             while (getchar() != '\n');
         }

         // Manual mode sets the fan directly. It no longer reuses
         // 'temperature' to smuggle a fan speed into systemControl().
         Fanspeed = manualFanSpeed;
         Ledstate = (manualFanSpeed > 0) ? 1 : 0;
         Buzzerstate = (manualFanSpeed == 5) ? 1 : 0;
     } else if (config == 3) {
         printf("Exiting ...\n");
         break;
    }else if (config == 0) {
        printf("On standby\n");
        printf("Shutting Down...\n");
        
        Fanspeed = 0;
        Ledstate = 0;
        Buzzerstate = 0;
        
    } else {
        printf("INVALID CHOICE\n");
        break;
       }
     displaySystem(temperature);
     }
    return 0;
}

void alertSignal() {
    for (int i = 0; i < 3; i++)
        {  // repeat 3 times
        // ON state
        Buzzerstate = 1;
        Ledstate = 1;
        printf("BUZZER ON | LED ON\n");
        sleep(1);

        // OFF state
        Buzzerstate = 0;
        Ledstate = 0;
        printf("BUZZER OFF | LED OFF\n");
        sleep(1);
    }

    // The condition that triggered this alert is still active when the
    // blinking finishes, so leave the buzzer/LED ON rather than ending
    // on the OFF phase of the last blink cycle.
    Buzzerstate = 1;
    Ledstate = 1;
}

void systemControl(float temperature) {

    if (SystemPower == 0) {
        Fanspeed = 0;
        Ledstate = 0;
        Buzzerstate = 0;
        return;
    }
    else if (temperature < 25) {
        Fanspeed = 0;   // OFF;
        printf("Temperature normal.\n");
   }
    else if (temperature >= 25 && temperature < 27) {
        Fanspeed = 1;
        Ledstate = 1;
        Buzzerstate = 1;
        printf("Turning Fan On\n");
    }
    else if (temperature >= 27 && temperature < 29) {
        Fanspeed = 2;
        Ledstate= 0;
        Buzzerstate = 1;
        printf("Increasing Fan speed\n");
    }
    else if (temperature >= 29 && temperature < 31) {
        Fanspeed = 3;
        Ledstate = 0;
        Buzzerstate = 1;
        printf("Increasing Fan speed\n");
    }
    else if (temperature >= 31 && temperature < 32) {
        Fanspeed = 4;
        Ledstate = 0;
        Buzzerstate = 1;
        printf("Increasing Fan speed\n");
    }
    else {
    Fanspeed = 5;   // Max speed;
    Ledstate = 1;
    Buzzerstate = 1;
    printf("CRITICAL TEMPERATURE!\n");
    alertSignal();
}
}

void setpointControl(float temperature, float setpoint) {

    if (SystemPower == 0) {
        Fanspeed = 0;
        Ledstate = 0;
        Buzzerstate = 0;
        return;
    }

    float diff = temperature - setpoint;   // how far above target we are

    if (diff <= 0) {
        Fanspeed = 0;
        Ledstate = 0;
        Buzzerstate = 0;
        printf("At or below target (%.2f°C). Fan off.\n", setpoint);
    }
    else if (diff < 2) {
        Fanspeed = 1;
        Ledstate = 1;
        Buzzerstate = 1;
        printf("Slightly above target. Turning fan on.\n");
    }
    else if (diff < 4) {
        Fanspeed = 2;
        Ledstate = 0;
        Buzzerstate = 1;
        printf("Above target. Increasing fan speed.\n");
    }
    else if (diff < 6) {
        Fanspeed = 3;
        Ledstate = 0;
        Buzzerstate = 1;
        printf("Well above target. Increasing fan speed.\n");
    }
    else if (diff < 7) {
        Fanspeed = 4;
        Ledstate = 0;
        Buzzerstate = 1;
        printf("Far above target. Increasing fan speed.\n");
    }
    else {
        Fanspeed = 5;   // Max speed
        Ledstate = 1;
        Buzzerstate = 1;
        printf("CRITICAL: Far exceeding target temperature!\n");
        alertSignal();
    }
}

void displaySystem(float temperature) {
    printf("\n===== SYSTEM STATUS =====\n");
    printf("Temperature: %.2f°C\n", temperature);
    printf("Fan: %s\n", Fanspeed ? "ON" : "OFF");
    printf("LED: %s\n", Ledstate ? "ON" : "OFF");
    printf("Buzzer: %s\n", Buzzerstate ? "ON" : "OFF");
}
