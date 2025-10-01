#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char name[50];
    char result[20];
    char date[12];   // YYYY-MM-DD
    char feedback[100];
} EmployeeEvaluation;

EmployeeEvaluation employees[MAX];
int count = 0;

// อ่านข้อมูลจาก CSV
void load_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("ไม่พบไฟล์ %s, จะสร้างใหม่เมื่อบันทึกข้อมูล\n", filename);
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // ข้าม header

    while (fgets(line, sizeof(line), file)) {
        if (count >= MAX) break;
        char *token = strtok(line, ",");
        if (token) strcpy(employees[count].name, token);

        token = strtok(NULL, ",");
        if (token) strcpy(employees[count].result, token);

        token = strtok(NULL, ",");
        if (token) strcpy(employees[count].date, token);

        token = strtok(NULL, ",");
        if (token) {
            token[strcspn(token, "\n")] = 0; // ตัด \n
            strcpy(employees[count].feedback, token);
        }
        count++;
    }
    fclose(file);
}

// บันทึกข้อมูลลง CSV
void save_data(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("ไม่สามารถบันทึกไฟล์ได้\n");
        return;
    }

    fprintf(file, "EmployeeName,EvaluationResult,EvaluationDate,Feedback\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s\n",
                employees[i].name,
                employees[i].result,
                employees[i].date,
                employees[i].feedback);
    }
    fclose(file);
}

// แสดงข้อมูลทั้งหมด
void display_all() {
    printf("\n---- ข้อมูลการประเมินทั้งหมด ----\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s | %s | %s | %s\n",
               i + 1,
               employees[i].name,
               employees[i].result,
               employees[i].date,
               employees[i].feedback);
    }
    printf("-------------------------------\n");
}

// เพิ่มข้อมูลใหม่
void add_evaluation() {
    if (count >= MAX) {
        printf("จำนวนพนักงานเต็มแล้ว\n");
        return;
    }

    getchar();
    printf("ชื่อพนักงาน: ");
    fgets(employees[count].name, sizeof(employees[count].name), stdin);
    employees[count].name[strcspn(employees[count].name, "\n")] = 0;

    printf("ผลการประเมิน (Excellent/Good/Average/Poor): ");
    fgets(employees[count].result, sizeof(employees[count].result), stdin);
    employees[count].result[strcspn(employees[count].result, "\n")] = 0;

    printf("วันที่ประเมิน (YYYY-MM-DD): ");
    fgets(employees[count].date, sizeof(employees[count].date), stdin);
    employees[count].date[strcspn(employees[count].date, "\n")] = 0;

    printf("ข้อเสนอแนะ: ");
    fgets(employees[count].feedback, sizeof(employees[count].feedback), stdin);
    employees[count].feedback[strcspn(employees[count].feedback, "\n")] = 0;

    count++;
    printf("เพิ่มข้อมูลเรียบร้อย!\n");
}
void update_evaluation() {
    printf("ฟังก์ชัน update_evaluation() ยังไม่ทำ\n");
}
void delete_evaluation() {
    printf("ฟังก์ชัน delete_evaluation() ยังไม่ทำ\n");
}

// ค้นหาข้อมูล
void search_evaluation() {
    char keyword[50];
    getchar(); // กำจัด newline ค้าง
    printf("ใส่ชื่อหรือวันที่ (YYYY-MM-DD) เพื่อค้นหา: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(employees[i].name, keyword) || strstr(employees[i].date, keyword)) {
            printf("%d. %s | %s | %s | %s\n",
                   i + 1,
                   employees[i].name,
                   employees[i].result,
                   employees[i].date,
                   employees[i].feedback);
            found = 1;
        }
    }
    if (!found) printf("ไม่พบข้อมูล\n");
}

// แสดงเมนู
void display_menu(const char *filename) {
    int choice;
    do {
        printf("\n==== ระบบประเมินพนักงาน ====\n");
        printf("1. แสดงข้อมูลทั้งหมด\n");
        printf("2. เพิ่มข้อมูลใหม่\n");
        printf("3. ค้นหาข้อมูล\n");
        printf("4. อัปเดตข้อมูล\n");
        printf("5. ลบข้อมูล\n");
        printf("6. ออกจากโปรแกรม\n");
        printf("เลือก: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_all();
                break;
            case 2:
                add_evaluation();
                save_data(filename);
                break;
            case 3:
                search_evaluation();
                break;
            case 4:
                update_evaluation();
                save_data(filename);
                break;
            case 5:
                delete_evaluation();
                save_data(filename);
                break;
            case 6:
                printf("ออกจากโปรแกรม...\n");
                break;
            default:
                printf("ตัวเลือกไม่ถูกต้อง!\n");
        }
    } while (choice != 6);
}

int main() {
    const char *filename = "employee_evaluation.csv";

    load_data(filename);
    display_menu(filename);

    return 0;
}
