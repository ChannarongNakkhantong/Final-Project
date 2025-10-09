#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define TEST_FILENAME "unit_test_data.csv" 

// โครงสร้างข้อมูล
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

// บันทึกข้อมูลลง CSV (จำเป็นสำหรับ E2E Test แต่ยังคงไว้สำหรับ Unit Test ที่มีการสร้างไฟล์)
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

//Test
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

void test_load_data() {
    printf("\n--- UNIT TEST 1: load_data ---\n");
    reset_test_state();
    
    // 1.1 สร้างไฟล์จำลอง
    FILE *f = fopen(TEST_FILENAME, "w");
    if (f) {
        fprintf(f, "EmployeeName,EvaluationResult,EvaluationDate,Feedback\n");
        fprintf(f, "Test Alice,Excellent,2024-01-01,Great\n");
        fprintf(f, "Test Bob,Good,2024-02-01,Keep going\n");
        fclose(f);
    }

    load_data(TEST_FILENAME);

    assert_equal("Count after loading 2 items", 2, count);
    assert_string_equal("Item 1 Name check", "Test Alice", employees[0].name);

    reset_test_state();
    load_data(TEST_FILENAME); // ทดสอบการโหลดไฟล์ที่ไม่มีอยู่
    assert_equal("Count after loading non-existent file", 0, count);
}

void test_delete_evaluation() {
    printf("\n--- UNIT TEST 2: delete_evaluation_core ---\n");
    reset_test_state();

    // 2.1 เตรียมข้อมูล
    strcpy(employees[0].name, "Target A");
    strcpy(employees[1].name, "Keeper B");
    strcpy(employees[2].name, "Target C");
    count = 3;

    // 2.2 ลบรายการ "Target A"
    int result = delete_evaluation_core("Target A"); 
    assert_equal("Delete Target A success code", 1, result); 
    assert_equal("Count after deleting A", 2, count);

    // 2.3 ลบรายการที่ไม่มีอยู่
    result = delete_evaluation_core("NotFound"); 
    assert_equal("Delete not found result code", 0, result);
}


void run_unit_tests() {
    printf("\n==================================\n");
    printf("         RUNNING UNIT TESTS         \n");
    printf("==================================\n");
    test_load_data();
    test_delete_evaluation();
    printf("\nUNIT TESTS COMPLETED. (2 functions tested)\n");
}

int main() {
    run_unit_tests();
    remove(TEST_FILENAME); // ทำความสะอาดไฟล์ทดสอบ
    return 0;
}
