//
// Created by piotr on 18.03.2021.
//
#include <iostream>
#include <omp.h>

using namespace std;
int main(){
#pragma omp parallel for
    for(int i = 0; i < 10; i++){
        cout << i << endl;
    }
    return 0;
}