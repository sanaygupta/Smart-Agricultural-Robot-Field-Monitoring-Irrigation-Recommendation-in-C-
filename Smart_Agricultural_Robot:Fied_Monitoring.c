#include <stdio.h>
#include <string.h>
#define MOISTURE_LIMIT 30.0

#define HEALTH_LIMIT 70
typedef struct {
    int id;
    float moisture, temperature, humidity;
    int cropHealth;
    float latitude, longitude;
} FieldRecord;

void addRecord() {
    FILE *fp = fopen("field_data.txt", "a");
    if (!fp) { printf("Error: Cannot open file\n"); return; }
    FieldRecord r;
    printf("Enter Field ID: "); scanf("%d", &r.id);
    printf("Enter Soil Moisture (%%): "); scanf("%f", &r.moisture);
    printf("Enter Temperature (°C): "); scanf("%f", &r.temperature);
    printf("Enter Humidity (%%): "); scanf("%f", &r.humidity);
    printf("Enter Crop Health (%%): "); scanf("%d", &r.cropHealth);
    printf("Enter Latitude (°): "); scanf("%f", &r.latitude);
    printf("Enter Longitude (°): "); scanf("%f", &r.longitude);
    fwrite(&r, sizeof(FieldRecord), 1, fp);
    fclose(fp);
}

void generateReport() {
    FILE *fp = fopen("field_data.txt", "r");
    FILE *rep = fopen("field_report.txt", "w");
    if (!fp) { printf("No Readings Found\n"); return; }
    FieldRecord r;
    int total = 0, irrig = 0, low_health = 0;
    fprintf(rep, "=== Agricultural Field Monitoring Report ===\n");
    fprintf(rep, "Date : 16-Nov-2026\n");
    while (fread(&r, sizeof(FieldRecord), 1, fp)) {
        total++;
        if (r.moisture < MOISTURE_LIMIT) irrig++;
        if (r.cropHealth < HEALTH_LIMIT) low_health++;
    }
    fprintf(rep, "Total Fields : %d\n", total);
    fprintf(rep, "Irrigation Required : %d\n", irrig);
    // Yahan pe '%' ko '%%' kar diya hai taaki warning na aaye
    fprintf(rep, "Crop Health (Low < 70%%) : %d\n\n", low_health);
    fprintf(rep, "--- Field-wise Details ---\n");
    fprintf(rep, "ID\tGPS (Lat, Long)\tStatus\n");
    
    rewind(fp);
    while (fread(&r, sizeof(FieldRecord), 1, fp)) {
        char status[50] = "Normal";
        if (r.moisture < MOISTURE_LIMIT && r.cropHealth < HEALTH_LIMIT)
            strcpy(status, "Irrigation + Attention");
        else if (r.moisture < MOISTURE_LIMIT)
            strcpy(status, "Irrigation Required");
        else if (r.cropHealth < HEALTH_LIMIT)
            strcpy(status, "Crop Stress (Attention)");
        
        fprintf(rep, "%d\t(%.4f, %.4f)\t%s\n", r.id, r.latitude, r.longitude, status);
    }
    fclose(fp);
    fclose(rep);
    printf("Report generated: field_report.txt\n");
}

int main() {
    int ch;
    while (1) {
        printf("\n--- Smart Agri Robot ---\n");
        printf("1. Add Field Reading\n");
        printf("2. Generate Field Report\n");
        printf("3. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &ch);
        if (ch == 1) addRecord();
        else if (ch == 2) generateReport();
        else if (ch == 3) break;
    }
    return 0;
}
