static int a;
extern int b;
static void bar();
extern void ext();

static void bar()
{
    a=1;
    b=2;
}
void foo()
{
    bar();
    ext();
}

