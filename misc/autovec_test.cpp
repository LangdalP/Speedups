#include <iostream>
#include <string>

#include <cstdlib> 
#include <ctime>

using std::cout;
using std::endl;

int test_gcc_auto_vectorization(const int* const b, const int* const c)
{
    int a[256];
    int i;
    
    // Want this part to be auto-vectorized, as per https://gcc.gnu.org/projects/tree-ssa/vectorization.html
    for (i = 0; i < 256; i++) {
        a[i] = b[i] + c[i];
    }

    int a_sum = 0;
    for (i = 0; i < 256; i++) {
        a_sum += a[i];
    }
    return a_sum;
}

int main(int argc, char const *argv[])
{
    srand((unsigned)time(0)); 
    int random_num = (rand()%1000)+1; 

    int b[256], c[256];
    for (int i = 0; i < 256; i++) {
        random_num = (rand()%1000)+1;
        b[i] = i + random_num;
        c[i] = i + random_num;
    }
    cout << "Testing auto-vectorization: " << test_gcc_auto_vectorization(b, c) << endl;
	return 0;
}