//Importing Libraries
#include <iostream>
#include <iomanip>
#include <cstdlib> 
using namespace std;

//Function Prototypes & Template Declaration
void print_real(float number, int fieldspace, int precision);
void update_scale(T& m1, T& m2, T m3 = 10);
template <typename T>

//Main Function
int main(void){
    float  a=0;
    float b=0;
    cout<<" Enter first numbers: ";
    cin>>a;
    cout<<" Enter second numbers: ";
    cin>>b;

    //Calling the function print_real to print user-inputted numbers
    cout << "Numbers Inputted:\n";
    print_real(a,7,3);
    print_real(b,7,3);

    //Calling the function update_scale
    update_scale(a, b);

    //Calling the functions print_real after updating the values
    cout << "Updated Numbers:\n";
    print_real(a,7,3);
    print_real(b,7,3);
}


//Print Real Numbers Function
void print_real(float number, int fieldspace, int precision){
    cout << fixed                     // Print fixed-point notation
         << setprecision(precision)   // Print the precision number of decimal places
         << setw(fieldspace)          // Print the width of the output field
         << number                    // Print the number
         << endl;                     // Print a newline
}

//Update Scale Function
template <typename T>
void update_scale(T& m1, T& m2, T m3){
    T m1_1=m1; 
    T m2_1=m2;

    m1=(m1_1+m2_1)*m3;
    m2=(m1_1-m2_1)*m3;
}