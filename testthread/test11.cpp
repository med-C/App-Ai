#include <iostream>
#include <thread>

using namespace std;

void counter(){

    for(int i=0 ; i<10 ; i++){
        cout << "hello" << i << " ";
    }
}

int main(){

    thread a(counter);
    thread b(counter);

    b.join();
    a.detach();


    return 0;
}