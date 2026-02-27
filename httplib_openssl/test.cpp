#define _WIN32_WINNT 0x0A00  // Windows 10 ou supérieur _ http
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "include/httplib.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json; 

int main() {
    std::string token = "";

    httplib::SSLClient cli("router.huggingface.co", 443);
    cli.set_follow_location(true);

    httplib::Headers headers = {
        {"Authorization", "Bearer " + token},
        {"Content-Type", "application/json"}
    };

    std::string body = R"({
        "model": "Qwen/Qwen3.5-397B-A17B:novita",
        "messages": [
            {"role": "user", "content": "who is sans from undertale."}
        ]
    })";

    auto res = cli.Post("/v1/chat/completions", headers, body, "application/json");

    if (res) {
    std::string body = res->body;
    try {
        json j = json::parse(body);  // parse le string en objet JSON

        // maintenant tu peux accéder aux éléments
        // par exemple, si tu veux le premier message du modèle
        std::string reply = j["choices"][0]["message"]["content"];
        std::cout << "Model reply: " << reply << std::endl;

    } catch (json::parse_error& e) {
        std::cerr << "Erreur parse JSON: " << e.what() << std::endl;
    }
    } else {
        std::cout << "Request failed\n";
    }

    return 0;
}