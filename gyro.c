#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define X_ASCII 0x78
#define Y_ASCII 0x79
#define Z_ASCII 0x7A
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define LED_4 4
#define LED_5 5
#define LED_6 6
#define LED_7 7

void Gyro_read (char axe, char* buff) {
    FILE *file = NULL;
    char path[100];
    sprintf (path,"/sys/devices/platform/soc/30800000.aips-bus/30a50000.i2c/i2c-3/3-0020/iio:device1/in_anglvel_%c_raw", axe);
    file = fopen(path, "r");


    if (file == NULL) {
        printf("Gyro read error on axis %d", axe);
        return;
    }
    fgets(buff, 255, (FILE*)file);
    fclose(file);
}

void allume_led (int index) {
    FILE *file = NULL;
    char path[100];
    sprintf (path,"/sys/class/leds/d%d/brightness", index);
    file = fopen(path, "r+");

    if (file == NULL) {
        printf("Cant modify led d%d\n", index);
        return;
    }
    fputs("1", file);
    fclose(file);
}

void eteint_led (int index) {
    FILE *file = NULL;
    char path[100];
    sprintf (path,"/sys/class/leds/d%d/brightness", index);
    file = fopen(path, "r+");

    if (file == NULL) {
        printf("Cant modify led d%d\n", index);
        return;
    }
    fputs("0", file);
    fclose(file);
}

void maj_leds (int indexOFF, int indexON) {
    eteint_led (indexOFF);
    allume_led (indexON);
}

 int main() {
     FILE *axe_x = NULL, *axe_y = NULL, *axe_z = NULL;
     char axe_x_val[255];
     char axe_y_val[255];
     char axe_z_val[255];
     int index_led_act = LED_2;
     int axe_x_int, axe_y_int, axe_z_int;
     int tempo_decale = 0;
    
    for(int i = 0; i < 7; i++){
        eteint_led(i);
    }
    allume_led(LED_2);
    while (1) {
        //lecture des données
        Gyro_read(X_ASCII, axe_x_val);
        Gyro_read(Y_ASCII, axe_y_val);
        Gyro_read(Z_ASCII, axe_z_val);
        printf("  X=%s  Y=%s  Z=%s \n",axe_x_val, axe_y_val, axe_z_val);

        //traitement des infos
        axe_x_int = atoi(axe_x_val);
        axe_y_int = atoi(axe_y_val);
        axe_z_int = atoi(axe_z_val);

        //transcription sur les leds
        tempo_decale += axe_x_int;  //fonction à affiner, utiliser log peut-être ???
        if (tempo_decale < -1000) {     //leds vers la gauche
            if (index_led_act > LED_0) {
                maj_leds (index_led_act, index_led_act - 1);
                index_led_act -= 1;
            }
            tempo_decale = 0;
        }
        else if (tempo_decale > 1000) { //leds vers la droite
            if (index_led_act < LED_7) {
                maj_leds (index_led_act, index_led_act+1);
                index_led_act += 1;
            }
            tempo_decale = 0;
        }


        sleep(3);
    }
}
