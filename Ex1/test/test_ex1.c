#include "unity.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {
}

void tearDown(void) {
    remove("test_data.bin");
}

void test_record_struct_size(void) {
    char msg[100];
    sprintf(msg, "La struct Record deve essere di 36 bytes, ma è %zu. Hai dimenticato __attribute__((packed))?", sizeof(Record));
    TEST_ASSERT_EQUAL_INT_MESSAGE(36, sizeof(Record), msg);
}

void test_read_write_record(void) {
    Record r_out = { .id = 12345, .field1 = 3.14f, .field2 = -9999, .field3 = "TestString" };
    Record r_in;

    FILE *fp = fopen("test_data.bin", "wb");
    TEST_ASSERT_NOT_NULL(fp);
    int w_res = write_record(fp, &r_out);
    fclose(fp);
    TEST_ASSERT_EQUAL_INT(1, w_res);

    fp = fopen("test_data.bin", "rb");
    TEST_ASSERT_NOT_NULL(fp);
    int r_res = read_record(fp, &r_in);
    fclose(fp);

    TEST_ASSERT_EQUAL_INT(1, r_res);
    TEST_ASSERT_EQUAL_UINT64(r_out.id, r_in.id);
    TEST_ASSERT_EQUAL_FLOAT(r_out.field1, r_in.field1);
    TEST_ASSERT_EQUAL_INT64(r_out.field2, r_in.field2);
    TEST_ASSERT_EQUAL_STRING(r_out.field3, r_in.field3);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_record_struct_size);
    RUN_TEST(test_read_write_record);
    return UNITY_END();
}