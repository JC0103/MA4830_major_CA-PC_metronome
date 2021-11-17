#include <stdio.h>

char waveforms[20] = "Sine";
float frequency = 2;
float amplitude = 2.3;

void WriteSettings(){
    char buffer [100];
    FILE *fp;
    fp = fopen("setting.txt", "w+");  // create file and write or overwrite if file existed
    sprintf(buffer, "Waveforms: %s\nFrequency(Hz): %f\nAmplitude: %f", waveforms, frequency, amplitude);
    fputs(buffer, fp);
    fclose(fp);
}

void ReadSettings(){
    FILE *fp;
    fp = fopen("setting.txt", "r+");   //read and write
    fscanf(fp, "%*s %s", waveforms);
    fscanf(fp, "%*s %f", &frequency);
    fscanf(fp, "%*s %f", &amplitude);
    fclose(fp);
}

int main(){
    WriteSettings();
    ReadSettings();
    printf("%s \n %f \n %f \n", waveforms, frequency, amplitude);

    
}