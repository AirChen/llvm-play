int callee(const int* X)
{
    return *X + 1;
}

int main() 
{
    int T = 4;
    return callee(&T);
}