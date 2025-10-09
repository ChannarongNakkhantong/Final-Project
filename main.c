#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 100
#define TEST_FILENAME "unit_test_data.csv" //Unit Test และ E2E Test

typedef struct {
    char name[50];
    char result[20];
    char date[12];    // YYYY-MM-DD
    char feedback[100];
} EmployeeEvaluation;

EmployeeEvaluation employees[MAX];
int count = 0;


void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
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
            token[strcspn(token, "\n")] = 0; // ตัด \n
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
    if (count == 0) {
        printf("ไม่มีข้อมูล\n");
        printf("-------------------------------\n");
        return;
    }
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

    clear_input_buffer(); 
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

//อัปเดต
void update_evaluation_core(const char *keyword, int is_interactive) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].name, keyword) == 0) {
            found = 1;
            
            if (is_interactive) {
                printf("ผลการประเมินใหม่: ");
                fgets(employees[i].result, sizeof(employees[i].result), stdin);
                employees[i].result[strcspn(employees[i].result, "\n")] = 0;

                printf("ข้อเสนอแนะใหม่: ");
                fgets(employees[i].feedback, sizeof(employees[i].feedback), stdin);
                employees[i].feedback[strcspn(employees[i].feedback, "\n")] = 0;

                printf("อัปเดตเรียบร้อย!\n");
            }
            break; 
        }
    }
    if (!found && is_interactive) printf("ไม่พบพนักงานชื่อดังกล่าว\n");
}

// อัปเดตข้อมูล
void update_evaluation() {
    char keyword[50];
    clear_input_buffer();
    printf("ใส่ชื่อพนักงานที่ต้องการอัปเดต: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    update_evaluation_core(keyword, 1); // 1 คือ Interactive
}


//ลบ
int delete_evaluation_core(const char *keyword) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].name, keyword) == 0) {
            found = 1;
            for (int j = i; j < count - 1; j++) {
                employees[j] = employees[j + 1];
            }
            count--;
            return 1; // ลบสำเร็จ
        }
    }
    return 0; // ลบไม่สำเร็จ
}

//ลบข้อมูล
void delete_evaluation() {
    char keyword[50];
    clear_input_buffer();
    printf("ใส่ชื่อพนักงานที่ต้องการลบ: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    if (delete_evaluation_core(keyword)) {
        printf("ลบเรียบร้อย!\n"); 
    } else {
        printf("ไม่พบพนักงานชื่อดังกล่าว\n");
    }
}

//ค้นหาข้อมูล
void search_evaluation() {
    char keyword[50];
    clear_input_buffer();
    printf("ใส่ชื่อหรือวันที่ (YYYY-MM-DD) เพื่อค้นหา: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    int found = 0;
    printf("\n---- ผลการค้นหาสำหรับ '%s' ----\n", keyword);
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
    printf("-------------------------------\n");
}

// UNIT TESTS 
void test_load_data() {
    printf("\n--- UNIT TEST 1: load_data ---\n");
    reset_test_state();
    
    
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
    load_data(TEST_FILENAME);
    assert_equal("Count after loading non-existent file", 0, count);
}

void test_delete_evaluation() {
    printf("\n--- UNIT TEST 2: delete_evaluation_core ---\n");
    reset_test_state();

    
    strcpy(employees[0].name, "Target A");
    strcpy(employees[1].name, "Keeper B");
    strcpy(employees[2].name, "Target C");
    count = 3;

    
    int result = delete_evaluation_core("Target A"); 
    assert_equal("Delete Target A success code", 1, result); 
    assert_equal("Count after deleting A", 2, count);

    
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
    remove(TEST_FILENAME); 
}

// E2E TEST SCENARIO
void run_e2e_test() {
    printf("\n====================================\n");
    printf("         RUNNING E2E TEST SCENARIO         \n");
    printf("====================================\n");
    
    reset_test_state();
    const char *filename = TEST_FILENAME;
    
    //CREATE
    strcpy(employees[0].name, "E2E Tester A");
    strcpy(employees[0].result, "Poor");
    count = 1;
    save_data(filename);

    //READ & VERIFY
    count = 0; 
    load_data(filename);
    assert_equal("E2E Step 2: Verify Load Count", 1, count);
    
    //UPDATE & SAVE
    strcpy(employees[0].result, "Excellent");
    save_data(filename);

    //VERIFY UPDATE
    count = 0;
    load_data(filename);
    assert_string_equal("E2E Step 4: Verify Update Result", "Excellent", employees[0].result);

    //DELETE & SAVE
    delete_evaluation_core("E2E Tester A");
    save_data(filename);

    //VERIFY DELETE 
    count = 0;
    load_data(filename);
    assert_equal("E2E Step 6: Verify Final Count (Should be 0)", 0, count);
    
    printf("\nE2E TEST SCENARIO COMPLETED.\n");
    remove(filename);
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
        printf("-------------------------------\n");
        printf("7. รัน Unit Tests\n");
        printf("8. รัน E2E Test\n");
        printf("เลือก: ");
        
        if (scanf("%d", &choice) != 1) {
            choice = 0; 
        }
        clear_input_buffer(); 

        switch (choice) {
            case 1: display_all(); break;
            case 2: add_evaluation(); save_data(filename); break;
            case 3: search_evaluation(); break;
            case 4: update_evaluation(); save_data(filename); break;
            case 5: delete_evaluation(); save_data(filename); break;
            case 6: printf("ออกจากโปรแกรม...\n"); break;
            case 7: run_unit_tests(); break;
            case 8: run_e2e_test(); break;
            default: printf("ตัวเลือกไม่ถูกต้อง! กรุณาเลือก 1-8\n");
        }
    } while (choice != 6);
}

int main() {
    const char *filename = "employee_evaluation.csv";
    load_data(filename);
    display_menu(filename);

    return 0;
}
