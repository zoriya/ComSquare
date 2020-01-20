#include <criterion/criterion.h>

int HelloWorld(void);

Test(HelloWorld, hello)
{
    int result = HelloWorld();

    cr_assert_eq(result, 0);
}