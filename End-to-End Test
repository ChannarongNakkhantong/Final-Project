#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 100
#define TEST_FILENAME "e2e_test_data.csv"


typedef struct {
    char name[50];
    char result[20];
    char date[12];
    char feedback[100];
} EmployeeEvaluation;
 
EmployeeEvaluation employees[MAX];
int count = 0;

// อ่านข้อมูลจาก CSV
void load_data(const char *filename) {
    count = 0;
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char line[256];
    if (fgets(line, sizeof(line), file) == NULL) { // ข้าม header
        fclose(file);
        return;
    } 

    while (fgets(line, sizeof(line), file)) {
        if (count >= MAX) break;
        
        char temp_line[256];
        strcpy(temp_line, line);
        
        char *token = strtok(temp_line, ",");
        if (token) strcpy(employees[count].name, token); else continue;

        token = strtok(NULL, ",");
        if (token) strcpy(employees[count].result, token); else continue;

        token = strtok(NULL, ",");
        if (token) strcpy(employees[count].date, token); else continue;

        token = strtok(NULL, ",");
        if (token) {
            token[strcspn(token, "\n")] = 0;
            strcpy(employees[count].feedback, token);
        } else {
            strcpy(employees[count].feedback, "");
        }
        count++;
    }
    fclose(file);
}

// บันทึกข้อมูลลง CSV
void save_data(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

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

// ฟังก์ชันหลักในการลบ (สำหรับ Test)
int delete_evaluation_core(const char *keyword) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].name, keyword) == 0) {
            found = 1;
            for (int j = i; j < count - 1; j++) {
                employees[j] = employees[j + 1];
            }
            count--;
            return 1;
        }
    }
    return 0;
}

// TEST UTILITIES & ASSERTIONS
void reset_test_state() {
    count = 0;
    remove(TEST_FILENAME); 
}

void assert_equal(const char *test_name, int expected, int actual) {
    if (expected == actual) {
        printf("  ✅ PASS: %s\n", test_name);
    } else {
        printf("  ❌ FAIL: %s (Expected: %d, Got: %d)\n", test_name, expected, actual);
    }
}

void assert_string_equal(const char *test_name, const char *expected, const char *actual) {
    if (strcmp(expected, actual) == 0) {
        printf("  ✅ PASS: %s\n", test_name);
    } else {
        printf("  ❌ FAIL: %s (Expected: '%s', Got: '%s')\n", test_name, expected, actual);
    }
}


// E2E TEST SCENARIO
void run_e2e_test() {
    printf("\n====================================\n");
    printf("     RUNNING E2E TEST SCENARIO         \n");
    printf("====================================\n");
    
    reset_test_state();
    const char *filename = TEST_FILENAME;
    
    printf("START: เริ่มต้น E2E Test (ไฟล์ '%s' ถูกลบแล้ว)\n", filename);

    //CREATE
    //เพิ่มข้อมูลในหน่วยความจำ
    strcpy(employees[0].name, "E2E Tester A");
    strcpy(employees[0].result, "Poor");
    strcpy(employees[0].date, "2025-10-09");
    strcpy(employees[0].feedback, "Initial entry");
    count = 1;
    //บันทึกลงไฟล์
    save_data(filename);
    printf("Step 1: Added 'E2E Tester A' (Poor) and saved.\n");

    //READ & VERIFY
    //โหลดข้อมูลจากไฟล์
    load_data(filename);
    //ตรวจสอบ
    assert_equal("E2E Step 2: Verify Load Count", 1, count);
    assert_string_equal("E2E Step 2: Verify Name", "E2E Tester A", employees[0].name);

    //UPDATE
    //แก้ไขในหน่วยความจำ (สมมติว่าหาเจอแล้ว)
    strcpy(employees[0].result, "Excellent");
    //บันทึกทับไฟล์
    save_data(filename);
    printf("Step 3: Updated 'E2E Tester A' result to Excellent and saved.\n");

    //VERIFY UPDATE
    //โหลดข้อมูลใหม่จากไฟล์
    load_data(filename);
    //ตรวจสอบผลลัพธ์
    assert_string_equal("E2E Step 4: Verify Update Result", "Excellent", employees[0].result);

    //DELETE
    //ลบจากหน่วยความจำ
    delete_evaluation_core("E2E Tester A");
    //บันทึกการเปลี่ยนแปลงลงไฟล์
    save_data(filename);
    printf("Step 5: Deleted 'E2E Tester A' and saved.\n");

    //VERIFY DELETE
    //โหลดข้อมูลใหม่จากไฟล์
    load_data(filename);
    //ตรวจสอบ Count
    assert_equal("E2E Step 6: Verify Final Count (Should be 0)", 0, count);
    
    printf("\nE2E TEST SCENARIO COMPLETED.\n");
}

//E2E Test
int main() {
    run_e2e_test();
    remove(TEST_FILENAME); // ทำความสะอาดไฟล์ทดสอบ
    return 0;
}
