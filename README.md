# 🧠 AI Medical Chat Application

> A high-performance desktop AI system built with C++ (SDL2 + Dear ImGui), MongoDB, and a local automation workflow engine (n8n + Docker).

---

## 🚀 Overview

AI Medical Chat Application is a real-time desktop system that simulates an AI assistant with persistent memory and structured workflow execution.

It combines:
- Desktop UI (SDL2 + ImGui)
- Database persistence (MongoDB)
- HTTP AI communication layer
- Local workflow engine (n8n inside Docker)

The system is designed with a **strict execution pipeline and reproducible environment constraints**.

---

> ⚡

### 🎥 Full Demo
![Demo](https://raw.githubusercontent.com/med-C/App-Ai/main/other/video.mp4)

### registre Interface
![Register Interface](https://raw.githubusercontent.com/med-C/App-Ai/main/other/b.png)

### 📸 Login Interface
![Login Screen](https://raw.githubusercontent.com/med-C/App-Ai/main/other/a.png)

### 💬 Chat Interface
![Chat Screen](https://raw.githubusercontent.com/med-C/App-Ai/main/other/interface.png)

![Chat Screen](https://raw.githubusercontent.com/med-C/App-Ai/main/other/interface2.png)

---


## ⚙️ System Requirements

### 🧰 Core Tools
- g++ (C++17 support)
- Make build system

---

### 🗄️ MongoDB Driver (STRICT PATH)
The project requires MongoDB C++ driver installed at:

```
C:\mongo-cxx-driver\
    ├── include/
    ├── lib/
    └── bin/
```

---

### 🐳 Docker Runtime
- Docker must be installed
- n8n workflow engine runs locally inside Docker

---

## 🔗 Execution Model (IMPORTANT)

This system follows a **non-persistent workflow execution model**:

- Every user request triggers a workflow execution
- No caching of AI responses
- No memory persistence between requests
- If workflow fails → request is rejected

👉 This ensures full reproducibility and deterministic behavior.

---

## 🧪 Build & Run

### 🔨 Build project
```bash
make
```

### ▶ Run application
```bash
make os
```

---

## 🔗 Workflow Integration (n8n)

AI responses are generated via a local workflow engine.

### 📄 Workflow file
👉 [Open workflow.json](https://github.com/med-C/App-Ai/tree/main/other/workflow.json) <!-- TODO -->

### Example request
```json
{
  "question": "headache symptoms",
  "user_id": "123",
  "conversation_id": "abc"
}
```

---

## 📌 Key Engineering Concepts

- Real-time rendering loop (SDL2)
- GUI architecture (Dear ImGui)
- Multithreading (std::thread, mutex, atomic)
- HTTP networking layer
- MongoDB integration (C++ driver)
- Local workflow orchestration (n8n + Docker)
- Stateless execution pipeline design

---

## 🚧 Constraints

- MongoDB driver must follow strict directory structure
- Docker must be running before execution
- n8n workflow must be active
- No offline mode supported
- Workflow failure = request failure

---

## 🔮 Future Improvements

- Modular architecture (clean separation layers)
- Secure authentication (hashed credentials)
- Cloud database integration
- UI redesign (modern theme system)
- Vulkan rendering upgrade

---

## 👨‍💻 Author

Software Engineering Student  
Focused on Systems Programming, Real-Time Applications, and Graphics Engineering