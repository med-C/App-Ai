#include <iostream>
#include <thread>
#include <mutex>
#include <future>

// using namespace std;
std::mutex mtx;
std::string shared_data;

std::string ask_ai()
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    // throw std::runtime_error("Erreur IA !"); // simulate exception
    std::lock_guard<std::mutex> lock(mtx);
    shared_data = "AI response";
    return shared_data;
}
//throw run a exception in f.get()
int main()
{
    std::future<std::string> f =
        std::async(std::launch::async, ask_ai);

    try {
        std::string result = f.get(); // récupère le résultat
        std::cout << "Résultat : " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Exception capturée : " << e.what() << std::endl;
    }
}