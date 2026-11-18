# Smart Agricultural Robot: Field Monitoring & Irrigation Recommendation in C   



## Assignment 09: Smart Agricultural Robot - Field Monitoring and Irrigation Recommendation Using File Handling in C

## 1. Problem Statement

Develop a C program that stores soil moisture, temperature, humidity, crop health readings and GPS coordinates collected by an agricultural robot. Read the stored data to identify areas requiring irrigation and generate field-monitoring reports.

## 2. Objectives

- Store agricultural field data collected by a robot in a file.
- Read stored data and analyze it.
- Identify areas requiring irrigation (low soil moisture).
- Generate a field-monitoring summary report.

## 3. Theory

### What Is a File?

A file is a collection of data stored on a storage device. Files allow data to be stored permanently, even after a program has finished executing.

In C programming, file handling allows us to create, open, read, write, append, and close files using functions provided by the standard input/output library.

The `stdio.h` header file contains the declarations of commonly used file handling functions.

### What Is File Handling in C?

File handling is the process of performing operations on files through a program. It is useful when information needs to be stored permanently or transferred from one file to another.

Common file handling operations include:

- Opening a file.
- Reading data from a file.
- Writing data to a file.
- Appending data to a file.
- Closing a file.

### What Is a `FILE *` Pointer?

A `FILE *` pointer is used to refer to a file opened by a C program. It allows file handling functions to access the information associated with that file.

**Syntax:**
```c
FILE *fp;
```

**Example:**
```c
FILE *fp;
fp = fopen("input.txt", "r");
```

Here, `fp` is a file pointer used to access the file named `input.txt`.

### What Is the `fopen()` Function?

The `fopen()` function opens a file in the specified mode and returns a file pointer.

**Syntax:**
```c
FILE *fopen(const char *filename, const char *mode);
```

**Example:**
```c
FILE *fp = fopen("input.txt", "r");
```

In this example, the file `input.txt` is opened in read mode.

If the file cannot be opened, `fopen()` returns `NULL`. Therefore, the returned pointer should be checked before performing file operations.

### What Is the `fclose()` Function?

The `fclose()` function closes an opened file and releases the resources associated with it.

**Syntax:**
```c
fclose(fp);
```

Closing files after completing file operations is important because it ensures that buffered output is properly handled and resources are released.

### What Are `fgetc()` and `fputc()`?

- **`fgetc()`** reads one character at a time from a file.
- **`fputc()`** writes one character at a time to a file.

**Example:**
```c
int ch;

ch = fgetc(fp);
fputc(ch, fp2);
```

The return value of `fgetc()` should be stored in an `int`, not a `char`, so that it can represent every possible character value as well as the special value `EOF`, which indicates the end of a file or a read error.

### Different File Opening Modes

| Mode | Purpose |
|---|---|
| `"r"` | Opens an existing file for reading. |
| `"w"` | Opens a file for writing. Creates it if necessary and truncates it if it already exists. |
| `"a"` | Opens a file for appending data at the end. |
| `"r+"` | Opens an existing file for both reading and writing. |
| `"w+"` | Opens a file for reading and writing; creates or truncates it. |
| `"a+"` | Opens a file for reading and appending. |

For this assignment, the source file is opened in `"r"` mode, while the destination file is opened in `"w"` mode.

## 4. Algorithm

1. Start.
2. Define FieldRecord structure.
3. Take sensor data from user and append to file.
4. Read all records from file.
5. For each record:
   - Check soil moisture, crop health, temperature and humidity.
6. Decide irrigation and field status.
7. Generate and display field-monitoring report (field-wise decisions).

## 5. C Program

```c
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
```

### Explanation of the Program

- `#include <stdio.h>` includes the standard input/output library containing file handling functions.
- `FILE *source, *destination;` declares two file pointers.
- `int ch;` stores each character read from the source file and can also represent `EOF`.
- `fopen("input.txt", "r")` opens the source file in read mode.
- `source == NULL` checks whether the source file failed to open.
- `fopen("output.txt", "w")` opens the destination file in write mode.
- `destination == NULL` checks whether the destination file failed to open.
- `fgetc(source)` reads one character from the source file.
- `fputc(ch, destination)` writes that character to the destination file.
- `EOF` indicates that the reading loop has reached the end of the file or encountered a read error.
- `ferror(source)` checks whether a file-reading error occurred.
- `fclose()` closes the files after processing.
- `return 0;` indicates successful program execution.

**Important:** Create `input.txt` in the same working directory as the program before running it. Opening `output.txt` in write mode will create it if it does not exist, or erase its previous contents if it does.

## 6. Sample Input and Output

### Operation: Copying File Contents

**Contents of `input.txt`:**
```text
Hello World!
This is a file handling program in C.
```

**Output displayed on the terminal:**
```text
File copied successfully.
```

**Contents of `output.txt` after execution:**
```text
Hello World!
This is a file handling program in C.
```

The destination file contains the same text as the source file.

## 7. Flowchart

```mermaid
flowchart TD
    A([Start]) --> B[/Input / Read field data/]
    B --> C[For each record]
    C --> D{Moisture < 30%?}
    D -- Yes --> E["Mark 'Irrigation Required'"]
    D -- No --> F["'No Irrigation Needed'"]
    E --> G["Generate field report<br/>(total fields, irrigation fields,<br/>low health fields)"]
    F --> G
    G --> H([End])
```

## 8. FAQs

### Q1. What is a `FILE *` pointer?

A `FILE *` pointer is used to refer to an opened file and allows the program to perform file operations using functions such as `fopen()`, `fgetc()`, `fputc()`, and `fclose()`.

### Q2. Explain the functioning of `fprintf()` and `fscanf()`.

- **`fprintf()`** writes formatted data to a file.
- **`fscanf()`** reads formatted data from a file.

**Example:**
```c
#include <stdio.h>

int main()
{
    FILE *fp;
    int number;

    fp = fopen("data.txt", "w");

    if (fp == NULL)
        return 1;

    fprintf(fp, "%d", 100);
    fclose(fp);

    fp = fopen("data.txt", "r");

    if (fp == NULL)
        return 1;

    fscanf(fp, "%d", &number);
    printf("%d\n", number);

    fclose(fp);

    return 0;
}
```

**Output:**
```text
100
```

### Q3. Explain different file opening modes.

The `"r"` mode opens an existing file for reading. The `"w"` mode opens a file for writing and clears existing contents. The `"a"` mode appends data at the end. The `"r+"`, `"w+"`, and `"a+"` modes allow both reading and writing, with different rules for file creation and truncation.

### Q4. What is the difference between `fgetc()` and `fputc()`?

`fgetc()` reads one character from a file, whereas `fputc()` writes one character to a file.

### Q5. What is `EOF`?

`EOF` stands for End Of File. It is a special integer value returned by input functions such as `fgetc()` when the end of the file is reached or a read error occurs. The `ferror()` function can be used to distinguish a read error from normal end-of-file.

### Q6. Why should a file be closed after use?

Closing a file releases resources and ensures that buffered output is handled properly. It also helps prevent unnecessary resource usage.

## 9. Conclusion

Thus, a C program was implemented to copy the contents of one file to another using file handling. The experiment demonstrated file pointers, file opening modes, and functions such as `fopen()`, `fgetc()`, `fputc()`, and `fclose()`. It also illustrated how to check file-opening, reading, and writing errors.
