
#ifndef __TEST__
#define __TEST__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//------------------------------------------------------------------------------

#define ASSERT_INIT()                                                           \
                                                                                \
    uint8_t __assert_count__ = 0x00;                                            \

#define ASSERT(condition)                                                       \
                                                                                \
    if (!(condition))                                                           \
    {                                                                           \
        return __assert_count__;                                                \
    }                                                                           \
                                                                                \
    else                                                                        \
    {                                                                           \
        __assert_count__++;                                                     \
    }                                                                           \

#define ASSERT_DATA(arr0, arr1)                                                 \
                                                                                \
    if (test_assert_data(sizeof(arr0), arr0, arr1) == 0x00)                     \
    {                                                                           \
        return __assert_count__;                                                \
    }                                                                           \
                                                                                \
    else                                                                        \
    {                                                                           \
        __assert_count__++;                                                     \
    }                                                                           \



#define TEST_XSTR(s) TEST_STR(s)
#define TEST_STR(s) #s

#define TEST_ERR_MARKUP " <"
#define TEST_INIT_INLINE(__name__)                                      \
    { .name = TEST_STR(__name__), .function = __name__, .state_code = 0 }  \

//------------------------------------------------------------------------------

typedef struct test
{
    char *name;
    
    uint8_t (*function) (void);
    uint8_t state_code;
	
}   test_t;

typedef struct test_group
{
    test_t   *list;
    uint32_t size;
    
    uint32_t passed;
    uint32_t failed;
    uint32_t other;
    
    char     *name;
    
}   test_group_t;

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

uint8_t test_assert_data(uint32_t data_size, uint8_t *data_0, uint8_t *data_1)
{
    uint32_t i;

    uint8_t return_code = 0x01;

    for (i = 0; i < data_size; ++i)
    {
        if (data_0[i] != data_1[i])
        {
            return_code = 0x00;
            break;
        }
    }

    return return_code;
}

//------------------------------------------------------------------------------

void test_init (test_t *test, uint8_t (*function)(void),
                    char name[])
{
    test->function      = function;
    test->name          = name;
    test->state_code    = 0xFF;
}

//------------------------------------------------------------------------------

void test_run (test_t *test)
{
    test->state_code = (*test->function) ();
}

//------------------------------------------------------------------------------

void test_print (test_t *test)
{
    char *flags[3];
    char *flag = NULL;
    
    flags[0] = "N/A";
    flags[1] = "PASS";
    flags[2] = "FAIL";
    
    if (test->state_code <  0) flag	= flags[0];
    if (test->state_code == 0) flag = flags[1];
    if (test->state_code >  0) flag	= flags[2];
    
    printf ("| %-35s | %-4s | %06d |",
            test->name, flag, test->state_code);

    return;
}

//------------------------------------------------------------------------------

void test_group_init (test_group_t *test_group, test_t list[],
                      uint32_t size, char name[])
{
    test_group->list      = list;
    test_group->size      = size;
    test_group->name      = name;
    
    test_group->passed    = 0;
    test_group->failed    = 0;
    test_group->other     = 0;

    return;
}

//------------------------------------------------------------------------------

void test_group_run (test_group_t *test_group)
{
    uint32_t i;
    
    for (i = 0; i < test_group->size; ++i)
    {
        test_run (&test_group->list[i]);
        
        if (test_group->list[i].state_code > 0)
        {
            test_group->failed++;
        }
        
        if (test_group->list[i].state_code < 0)
        {
            test_group->other++;
        }
        
        else if (test_group->list[i].state_code == 0)
        {
            test_group->passed++;
        }
    }

    return;
}

//------------------------------------------------------------------------------

void test_group_print (test_group_t *test_group)
{
    uint32_t i;
    
    printf ("\n- %s \n", test_group->name);
    
    for (i = 0; i < test_group->size; ++i)
    {
        test_print (test_group->list + i);
        
        if (test_group->list[i].state_code > 0 )
        {
            printf (TEST_ERR_MARKUP);
        }
        
        printf ("\n");
    }
    
    printf ("| %-35s | %4s | %6d | \n", "passed", "", test_group->passed);
    printf ("| %-35s | %4s | %6d | \n", "failed", "", test_group->failed);
    printf ("| %-35s | %4s | %6d | \n", "other ", "", test_group->other );

    return;
}

//------------------------------------------------------------------------------

uint8_t test_none(void)
{
    return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TEST__ */
