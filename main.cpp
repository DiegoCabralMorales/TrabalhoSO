#include <iostream>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

std::mutex ataque_p1;
std::mutex ataque_p2;

void ataqueP1(int tipoAtaque){
    ataque_p1.lock();
    //executa comando de ataque(regiao critica)
    ataque_p1.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void ataqueP2(int tipoAtaque){
    ataque_p2.lock();
    //executa comando de ataque(regiao critica)
    ataque_p2.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main(){
    int tipoAtaqueP1;
    int tipoAtaqueP2;
    std::thread funcoesP1(ataqueP1, std::ref(tipoAtaqueP1));
    std::thread funcoesP2(ataqueP2, std::ref(tipoAtaqueP2));
    funcoesP1.join();
    funcoesP2.join();

    std::cout << std::endl;
    return 0;
}