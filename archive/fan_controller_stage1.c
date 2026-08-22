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
void displaySystem(float temperature);
void alertSignal();

int main() {
     int config;
     float temperature;

     srand(time(0)); // for sensors
     while (1) {

     printf("0. Standby\n");
     printf("1. Auto Config\n");
     printf("2. Manual Config\n");
     printf("3. Exit\n");
     printf("Please select your choice: \n");
     scanf("%d", &config);

    
     if (config == 1 ){
            printf("AUTOMATED MODE\n");

             temperature = rand() % 35;
            printf("Simulated Temperature: %.2f°C\n", temperature);
            for (int i = 0; i < 100000000; i++);

     } else if  ( config == 2 ) {
         printf("MANUAL MODE\n");
         printf("Enter Fan speed (0 - 5): \n");
         scanf("%f", &temperature);
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
     systemControl(temperature);
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
        alertSignal();
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
}
}

void displaySystem(float temperature) {
    printf("\n===== SYSTEM STATUS =====\n");
    printf("Temperature: %.2f�C\n", temperature);
    printf("Fan: %s\n", Fanspeed ? "ON" : "OFF");
    printf("LED: %s\n", Ledstate ? "ON" : "OFF");
    printf("Buzzer: %s\n", Buzzerstate ? "ON" : "OFF");
}



