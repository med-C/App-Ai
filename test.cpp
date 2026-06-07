// #include <SDL.h>
// #include "imgui.h"
// #include "backends/imgui_impl_sdl2.h"
// #include "backends/imgui_impl_sdlrenderer2.h"

// int main(int argc, char* argv[])
// {
//     if (SDL_Init(SDL_INIT_VIDEO) != 0)
//         return -1;

//     SDL_Window* window = SDL_CreateWindow(
//         "SDL2 + ImGui Test",
//         SDL_WINDOWPOS_CENTERED,
//         SDL_WINDOWPOS_CENTERED,
//         800, 600,
//         SDL_WINDOW_SHOWN
//     );

//     SDL_Renderer* renderer = SDL_CreateRenderer(
//         window, -1,
//         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
//     );

//     // ----- ImGui Init -----
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO();
//     (void)io;

//     ImGui::StyleColorsDark();

//     ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
//     ImGui_ImplSDLRenderer2_Init(renderer);

//     bool quit = false;
//     float slider_value = 0.0f;
//     int counter = 0;

//     while (!quit)
//     {
//         SDL_Event event;
//         while (SDL_PollEvent(&event))
//         {
//             ImGui_ImplSDL2_ProcessEvent(&event);

//             if (event.type == SDL_QUIT)
//                 quit = true;
//         }

//         // ----- Start Frame -----
//         ImGui_ImplSDLRenderer2_NewFrame();
//         ImGui_ImplSDL2_NewFrame();
//         ImGui::NewFrame();

//         // ----- UI -----
//         ImGui::Begin("My First Window");

//         ImGui::Text("Hello bro");
//         ImGui::SliderFloat("Float", &slider_value, 0.0f, 1.0f);

//         if (ImGui::Button("Click me"))
//             counter++;

//         ImGui::Text("Counter = %d", counter);

//         ImGui::End();

//         // ----- Render -----
//         ImGui::Render();

//         SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
//         SDL_RenderClear(renderer);

//         // 🔥 FIX ICI
//         ImGui_ImplSDLRenderer2_RenderDrawData(
//             ImGui::GetDrawData(),
//             renderer
//         );

//         SDL_RenderPresent(renderer);
//     }

//     // ----- Cleanup -----
//     ImGui_ImplSDLRenderer2_Shutdown();
//     ImGui_ImplSDL2_Shutdown();
//     ImGui::DestroyContext();

//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }

















/////////////////// le code de AI


// #define _WIN32_WINNT 0x0A00
// #define CPPHTTPLIB_OPENSSL_SUPPORT

// #include <SDL.h>
// #include "imgui.h"
// #include "backends/imgui_impl_smadl2.h"
// #include "backends/imgui_impl_sdlrenderer2.h"

// #include "httplib_openssl/include/httplib.h"
// #include "json.hpp"
// #include <string>
// #include <thread>
// #include <mutex>
// #include <iostream>
// #include <atomic>
// #include <chrono>

// #include <config.h>

// using json = nlohmann::json;

// // ---------------- Variables globales ----------------
// std::string ai_response = "Salut ! Pose ta question.";
// std::string question_pending = "";
// std::mutex mtx;
// std::atomic<bool> run_ai_thread(true);
// std::string token = HF_TOKEN;

// // ---------------- Fonction simple AI ----------------
// void ask_ai(const std::string& question) {
//     try {
//         httplib::SSLClient cli("router.huggingface.co", 443);
//         cli.set_follow_location(true);

//         httplib::Headers headers = {
//             {"Authorization", "Bearer " + token},
//             {"Content-Type", "application/json"}
//         };

//         std::string body = R"({
//             "model": "Qwen/Qwen3.5-397B-A17B:novita",
//             "messages": [{"role": "user", "content": ")" + question + R"("}]
//         })";

//         auto res = cli.Post("/v1/chat/completions", headers, body, "application/json");

//         std::string reply = "Request failed";
//         if (res) {
//             try {
//                 json j = json::parse(res->body);
//                 reply = j["choices"][0]["message"]["content"];
//             } catch (...) {
//                 reply = "Erreur parse JSON";
//             }
//         }

//         std::lock_guard<std::mutex> lock(mtx);
//         ai_response = reply;

//     } catch (...) {
//         std::lock_guard<std::mutex> lock(mtx);
//         ai_response = "Erreur requête AI";
//     }
// }

// // ---------------- Thread permanent AI ----------------
// void ai_thread_func() {
//     while (run_ai_thread) {
//         std::string question;
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             question = question_pending;
//             question_pending.clear(); // on consomme la question
//         }

//         if (!question.empty()) {
//             ask_ai(question); // met à jour ai_response
//         } else {
//             std::this_thread::sleep_for(std::chrono::milliseconds(50));
//         }
//     }
// }

// // ---------------- Main ----------------
// int SDL_main(int, char**) {
//     SDL_Init(SDL_INIT_VIDEO);
//     SDL_Window* window = SDL_CreateWindow("Chat AI Simple",
//                                           SDL_WINDOWPOS_CENTERED,
//                                           SDL_WINDOWPOS_CENTERED,
//                                           800, 600,
//                                           SDL_WINDOW_ALLOW_HIGHDPI);
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

//     ImGui::CreateContext();
//     ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
//     ImGui_ImplSDLRenderer2_Init(renderer);

//     char input_text[256] = "";
//     bool run = true;

//     // Lancer le thread permanent
//     std::thread ai_thread(ai_thread_func);

//     while (run) {
//         SDL_Event event;
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT) run = false;
//             ImGui_ImplSDL2_ProcessEvent(&event);
//         }

//         ImGui_ImplSDLRenderer2_NewFrame();
//         ImGui_ImplSDL2_NewFrame();
//         ImGui::NewFrame();

//         ImGui::Begin("Chat AI Simple");
//         ImGui::TextWrapped("Pose ta question et clique sur Envoyer.");

//         ImGui::InputText("##input", input_text, sizeof(input_text));

//         if (ImGui::Button("Envoyer")) {
//             if (strlen(input_text) > 0) {
//                 std::lock_guard<std::mutex> lock(mtx);
//                 question_pending = input_text;
//                 input_text[0] = '\0';
//             }
//         }

//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             ImGui::TextWrapped("%s", ai_response.c_str());
//         }

//         ImGui::End();

//         ImGui::Render();
//         SDL_SetRenderDrawColor(renderer, 25, 25, 40, 255);
//         SDL_RenderClear(renderer);
//         ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
//         SDL_RenderPresent(renderer);
//     }

//     // Terminer le thread AI
//     run_ai_thread = false;
//     ai_thread.join();

//     ImGui_ImplSDLRenderer2_Shutdown();
//     ImGui_ImplSDL2_Shutdown();
//     ImGui::DestroyContext();
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }

#define _WIN32_WINNT 0x0A00
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "httplib_openssl/include/httplib.h"
#include "json.hpp"

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <atomic>
#include <chrono>
#include <vector>
#include <cstring>

using json = nlohmann::json;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

enum AppState {
    LOGIN,
    REGISTER,
    CHAT
};

struct Message {
    std::string role;
    std::string content;
};

struct Conversation {
    std::string id;
    std::string title;
};

mongocxx::instance instance{};
mongocxx::client client{mongocxx::uri{"mongodb://127.0.0.1:27017/?serverSelectionTimeoutMS=3000"}};

auto db = client["chat_app"];
auto users_col = db["users"];
auto conv_col = db["conversations"];
auto msg_col  = db["messages"];

std::mutex msg_mutex;
std::mutex conv_mutex;

std::vector<Message> messages;
std::vector<Conversation> conversations;

std::string current_user;
std::string current_conversation;

std::atomic<bool> loading_ai(false);
std::string error_text;

// --- DB Functions ---
bool create_user(const std::string& user, const std::string& pass) {
    try {
        auto result = users_col.find_one(document{} << "username" << user << finalize);
        if (result) return false;
        users_col.insert_one(document{}
            << "username" << user
            << "password" << pass
            << "created_at" << (long long)time(0)
            << finalize);
        return true;
    } catch(...) { return false; }
}

bool login_user(const std::string& user, const std::string& pass) {
    try {
        auto result = users_col.find_one(document{}
            << "username" << user
            << "password" << pass
            << finalize);
        return result.has_value();
    } catch(...) { return false; }
}

void load_conversations(const std::string& username) {
    std::lock_guard<std::mutex> lock(conv_mutex);
    conversations.clear();
    try {
        auto cursor = conv_col.find(document{} << "username" << username << finalize);
        for (auto&& doc : cursor) {
            Conversation c;
            c.id    = std::string(doc["conversation_id"].get_string().value);
            c.title = std::string(doc["title"].get_string().value);
            conversations.push_back(c);
        }
    } catch(...) {}
}

void load_messages(const std::string& conv_id) {
    std::lock_guard<std::mutex> lock(msg_mutex);
    messages.clear();
    try {
        auto cursor = msg_col.find(document{} << "conversation_id" << conv_id << finalize);
        for (auto&& doc : cursor) {
            Message m;
            m.role    = std::string(doc["role"].get_string().value);
            m.content = std::string(doc["content"].get_string().value);
            messages.push_back(m);
        }
    } catch(...) {}
}

void create_conversation() {
    std::string id = std::to_string(time(0));
    try {
        conv_col.insert_one(document{}
            << "username" << current_user
            << "conversation_id" << id
            << "title" << "Conv " + std::to_string(conversations.size() + 1)
            << "created_at" << (long long)time(0)
            << finalize);
        current_conversation = id;
        load_conversations(current_user);
        load_messages(current_conversation);
    } catch(...) {}
}

void save_message(const std::string& role, const std::string& content) {
    try {
        msg_col.insert_one(document{}
            << "conversation_id" << current_conversation
            << "role" << role
            << "content" << content
            << "timestamp" << (long long)time(0)
            << finalize);
    } catch(...) {}
}

void delete_conversation(const std::string& conv_id) {
    try {
        msg_col.delete_many(document{} << "conversation_id" << conv_id << finalize);
        conv_col.delete_one(document{} << "conversation_id" << conv_id << finalize);
        load_conversations(current_user);
        if (conv_id == current_conversation) {
            if (!conversations.empty()) {
                current_conversation = conversations[0].id;
                load_messages(current_conversation);
            } else {
                create_conversation();
            }
        }
    } catch(...) {}
}

// --- AI Thread ---
void send_question_async(const std::string& question) {
    loading_ai = true;
    std::thread([question]() {
        std::string ai_response;
        try {
            httplib::Client cli("http://localhost:5678");
            cli.set_connection_timeout(5);
            cli.set_read_timeout(30);
            json body;
            body["question"] = question;
            auto res = cli.Post("/webhook-test/medical", body.dump(), "application/json");
            if (res && res->status == 200) {
                try {
                    auto data = json::parse(res->body);
                    auto& root = data.is_array() ? data[0] : data;
                    if (root.contains("data")) {
                        auto& d = root["data"];
                        ai_response =
                            "Conseil: " + d.value("conseil", "") + "\n\n" +
                            "Medicament: " + d.value("medicament_recommande", "") + "\n\n" +
                            "Posologie: " + d.value("posologie", "") + "\n\n" +
                            "Avertissement: " + d.value("avertissement", "");
                    } else {
                        ai_response = root.dump(2);
                    }
                } catch(...) {
                    ai_response = "Erreur parsing reponse.";
                }
            } else {
                ai_response = "Erreur n8n (code " + std::to_string(res ? res->status : 0) + ")";
            }
        } catch(...) {
            ai_response = "Connexion n8n impossible.";
        }
        {
            std::lock_guard<std::mutex> lock(msg_mutex);
            messages.push_back({"ai", ai_response});
        }
        save_message("ai", ai_response);
        loading_ai = false;
    }).detach();
}

// --- Main ---
int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Medical Chat",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool running = true;
    AppState state = LOGIN;

    char username[128] = "";
    char password[128] = "";
    char input_msg[2048] = "";

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        
        ImGui::Begin("Main", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // --- Bouton X en haut à droite ---
        ImGui::SetCursorPos(ImVec2(io.DisplaySize.x - 40, 5));
        if (ImGui::Button("X", ImVec2(30, 30))) {
            running = false;
        }

        if (state == LOGIN || state == REGISTER) {
            float w = 400, h = 380;
            ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - w)*0.5f, (io.DisplaySize.y - h)*0.4f));
            ImGui::BeginChild("Auth", ImVec2(w, h), true);
            ImGui::Text("Medical AI Chat");
            ImGui::Separator();
            ImGui::InputText("Username", username, 128);
            ImGui::InputText("Password", password, 128, ImGuiInputTextFlags_Password);
            if (state == LOGIN) {
                if (ImGui::Button("Login", ImVec2(-1, 40))) {
                    if (login_user(username, password)) {
                        current_user = username;
                        load_conversations(current_user);
                        if (conversations.empty()) create_conversation();
                        else {
                            current_conversation = conversations[0].id;
                            load_messages(current_conversation);
                        }
                        state = CHAT;
                        error_text.clear();
                    } else error_text = "Identifiants invalides.";
                }
                if (ImGui::Button("Creer un compte", ImVec2(-1, 30))) {
                    state = REGISTER;
                    error_text.clear();
                }
            } else {
                if (ImGui::Button("Register", ImVec2(-1, 40))) {
                    if (create_user(username, password))
                        error_text = "Compte cree ! Connectez-vous.";
                    else error_text = "Nom d'utilisateur existe deja.";
                }
                if (ImGui::Button("Retour au Login", ImVec2(-1, 30))) {
                    state = LOGIN;
                    error_text.clear();
                }
            }
            if (!error_text.empty())
                ImGui::TextColored(ImVec4(1,0.3f,0.3f,1), "%s", error_text.c_str());
            ImGui::EndChild();
        }
        else if (state == CHAT) {
            ImVec2 size = ImGui::GetContentRegionAvail();
            float left_w = size.x * 0.28f;

            ImGui::BeginChild("sidebar", ImVec2(left_w, size.y), true);
            ImGui::Text("Connecte : %s", current_user.c_str());
            ImGui::Separator();
            if (ImGui::Button("+ Nouvelle Conversation", ImVec2(-1, 25)))
                create_conversation();
            ImGui::Spacing();
            ImGui::Text("Conversations :");
            ImGui::Separator();
            {
                std::lock_guard<std::mutex> lock(conv_mutex);
                for (auto& c : conversations) {
                    if (ImGui::Selectable(c.title.c_str(), c.id == current_conversation)) {
                        current_conversation = c.id;
                        load_messages(c.id);
                    }
                }
            }
            ImGui::Spacing();
            if (!current_conversation.empty()) {
                if (ImGui::Button("Supprimer la conversation", ImVec2(-1, 25))) {
                    delete_conversation(current_conversation);
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("chat", ImVec2(0, size.y), true);
            ImGui::BeginChild("msg_area", ImVec2(0, -45), false);
            {
                std::lock_guard<std::mutex> lock(msg_mutex);
                for (auto& m : messages) {
                    if (m.role == "user")
                        ImGui::TextColored(ImVec4(0.4f,1,0.4f,1), "You");
                    else
                        ImGui::TextColored(ImVec4(0.4f,0.7f,1,1), "AI");
                    ImGui::TextWrapped("%s", m.content.c_str());
                    ImGui::Separator();
                }
            }
            if (loading_ai) ImGui::Text("AI reflechit...");
            ImGui::EndChild();

            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 90);
            ImGui::InputText("##msg", input_msg, 2048);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Envoyer", ImVec2(80, 0))) {
                std::string q = input_msg;
                if (!q.empty() && !current_conversation.empty()) {
                    {
                        std::lock_guard<std::mutex> lock(msg_mutex);
                        messages.push_back({"user", q});
                    }
                    save_message("user", q);
                    send_question_async(q);
                    memset(input_msg, 0, sizeof(input_msg));
                }
            }
            ImGui::EndChild();
        }

        ImGui::End();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}





// #include <SDL.h>
// #include "imgui.h"
// #include "backends/imgui_impl_sdl2.h"
// #include "backends/imgui_impl_sdlrenderer2.h"

// #include <string>
// #include <thread>
// #include <mutex>
// #include <atomic>
// #include <chrono>

// // ================== AI SIMPLIFIÉ ==================
// std::string ai_response = "AI prêt.";
// std::string question_pending;
// std::mutex mtx;
// std::atomic<bool> run_ai(true);

// void fake_ai(const std::string& q)
// {
//     std::this_thread::sleep_for(std::chrono::milliseconds(400));

//     std::lock_guard<std::mutex> lock(mtx);
//     ai_response = "AI: " + q;
// }

// void ai_thread_func()
// {
//     while (run_ai)
//     {
//         std::string q;
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             q = question_pending;
//             question_pending.clear();
//         }

//         if (!q.empty())
//             fake_ai(q);
//         else
//             std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     }
// }

// // ================== SCREENS ==================
// enum Screen { LOGIN, DB, AI };

// // ================== MAIN ==================
// int SDL_main(int, char**)
// {
//     SDL_Init(SDL_INIT_VIDEO);

//     SDL_Window* window = SDL_CreateWindow(
//         "Pro UI",
//         SDL_WINDOWPOS_CENTERED,
//         SDL_WINDOWPOS_CENTERED,
//         900, 600,
//         SDL_WINDOW_SHOWN
//     );

//     SDL_Renderer* renderer = SDL_CreateRenderer(
//         window, -1,
//         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
//     );

//     // ================== IMGUI INIT ==================
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();

//     ImGuiIO& io = ImGui::GetIO();

//     ImGui::StyleColorsDark();

//     // ===== STYLE PRO =====
//     ImGuiStyle& style = ImGui::GetStyle();
//     style.WindowRounding = 10.0f;
//     style.FrameRounding = 6.0f;
//     style.GrabRounding = 6.0f;
//     style.ScrollbarRounding = 8.0f;

//     style.WindowPadding = ImVec2(15, 15);
//     style.FramePadding = ImVec2(10, 6);

//     ImVec4* colors = style.Colors;
//     colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
//     colors[ImGuiCol_Button] = ImVec4(0.20f, 0.40f, 0.80f, 1.00f);
//     colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.50f, 0.95f, 1.00f);
//     colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.30f, 0.60f, 1.00f);

//     ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
//     ImGui_ImplSDLRenderer2_Init(renderer);

//     // ================== DATA ==================
//     char username[64] = "";
//     char password[64] = "";
//     char input[256] = "";

//     Screen screen = LOGIN;

//     std::thread ai_thread(ai_thread_func);

//     bool run = true;

//     // ================== LOOP ==================
//     while (run)
//     {
//         SDL_Event event;
//         while (SDL_PollEvent(&event))
//         {
//             if (event.type == SDL_QUIT)
//                 run = false;

//             ImGui_ImplSDL2_ProcessEvent(&event);
//         }

//         ImGui_ImplSDLRenderer2_NewFrame();
//         ImGui_ImplSDL2_NewFrame();
//         ImGui::NewFrame();

//         // ================== LOGIN ==================
//         if (screen == LOGIN)
//         {
//             ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

//             ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
//             ImGui::SetNextWindowSize(ImVec2(360, 260));

//             ImGui::Begin("Login", nullptr,
//                 ImGuiWindowFlags_NoResize |
//                 ImGuiWindowFlags_NoCollapse);

//             ImGui::Text("Welcome");
//             ImGui::Spacing();

//             ImGui::InputText("Nom", username, 64);
//             ImGui::InputText("Mot de passe", password, 64,
//                 ImGuiInputTextFlags_Password);

//             ImGui::Spacing();
//             ImGui::Separator();
//             ImGui::Spacing();

//             if (ImGui::Button("Base de donnees", ImVec2(-1, 40)))
//                 screen = DB;

//             if (ImGui::Button("AI Chat", ImVec2(-1, 40)))
//                 screen = AI;

//             ImGui::End();
//         }

//         // ================== DB ==================
//         else if (screen == DB)
//         {
//             ImGui::Begin("Database");

//             ImGui::Text("Users list");
//             ImGui::Separator();

//             ImGui::BulletText("admin / 1234");
//             ImGui::BulletText("test / 0000");
//             ImGui::BulletText("guest / guest");

//             ImGui::Spacing();

//             if (ImGui::Button("Back", ImVec2(120, 35)))
//                 screen = LOGIN;

//             ImGui::End();
//         }

//         // ================== AI ==================
//         else if (screen == AI)
//         {
//             ImGui::Begin("AI Chat");

//             ImGui::BeginChild("chat", ImVec2(0, -60), true);

//             ImGui::TextWrapped("%s", ai_response.c_str());

//             ImGui::EndChild();

//             ImGui::InputText("##input", input, 256);

//             ImGui::SameLine();

//             if (ImGui::Button("Send", ImVec2(100, 25)))
//             {
//                 std::lock_guard<std::mutex> lock(mtx);
//                 question_pending = input;
//                 input[0] = '\0';
//             }

//             ImGui::Spacing();

//             if (ImGui::Button("Back"))
//                 screen = LOGIN;

//             ImGui::End();
//         }

//         // ================== RENDER ==================
//         ImGui::Render();

//         SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
//         SDL_RenderClear(renderer);

//         ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
//         SDL_RenderPresent(renderer);
//     }

//     // ================== CLEANUP ==================
//     run_ai = false;
//     ai_thread.join();

//     ImGui_ImplSDLRenderer2_Shutdown();
//     ImGui_ImplSDL2_Shutdown();
//     ImGui::DestroyContext();

//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }