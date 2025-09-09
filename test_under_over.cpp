#include <iostream> 
#include <cstdio> 
#include <cmath> 
#include <limits> 
#include <cfenv> 
#include <utility> 

using namespace std; 

//check underflow / overflow of floating-point types
template<typename FP> void find_FP_underflow_overflow(FP& min, FP& max, const long int i_max=5e7)
{
    FP test, step; 

    long int i=0; 
    test = 1.; 
    step = 1.005; 

    //find maximum
    feclearexcept(FE_ALL_EXCEPT); 

    while (!fetestexcept(FE_OVERFLOW) && i++ < i_max) { max = test; test *= step; }

    if (i >= i_max) {
        cout << "Overflow not reached by end of iterations: (" << i << ")" << endl; 
    } 

    i=0;
    test = 1.; 
    step = 0.995; 

    //find minimum
    feclearexcept(FE_ALL_EXCEPT); 

    while (!fetestexcept(FE_UNDERFLOW) && i++ < i_max) { min = test; test *= step; }

    if (i >= i_max) {
        cout << "Underflow not reached by end of iterations: (" << i << ")" << endl; 
    } 

    return; 
}  

int main(int argc, char* argv[])
{   
    //test underflow for floats
    float float_min, float_max;
    find_FP_underflow_overflow<float>(float_min, float_max); 

    printf("floats (single precision), min/max: %.8e /  %.8e\n", float_min, float_max); 

    //test underflow for doubles
    double double_min, double_max;
    find_FP_underflow_overflow<double>(double_min, double_max); 
    
    printf("doubles (single precision), min/max: %.8e /  %.8e\n", double_min, double_max); 



    return 0; 
}