int main()
{
    char * p = (char*)malloc(1000);
    /*use p as an array of size 1000*/
    free(p);
}
