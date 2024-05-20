#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "server.h"
#include "cve.h"
#include "apt_package.h"
#include "db.h"

void glfw_error_callback(int error, const char* description);
void server_details(Server server);

int main() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        return -1;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "NetPatch-Master", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    create_db("../NetPatch.db");

    bool b_add_server = false;
    int b_servers = -1;

    char name[128];
    char hostname[128];
    char username[128];
    char password[128];
    char port_b[6];

    std::vector<Server> servers = get_servers("../NetPatch.db");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Servers", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos(ImVec2(0, 20));
        ImGui::SetWindowSize(ImVec2(200, ImGui::GetIO().DisplaySize.y - 20));
        ImGui::Text("Servers");

        if (ImGui::Button("Add Server") || b_add_server) {
            if (!b_add_server) {
                name[0] = '\0';
                hostname[0] = '\0';
                username[0] = '\0';
                password[0] = '\0';
                port_b[0] = '\0';
                b_add_server = true;
            }

            ImGui::Begin("Add Server", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::SetWindowPos(ImVec2(450, 300));
            ImGui::SetWindowSize(ImVec2(500, 180));

            ImGui::InputText("Server name", name, IM_ARRAYSIZE(name));
            ImGui::InputText("Hostname", hostname, IM_ARRAYSIZE(hostname));
            ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
            ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
            ImGui::InputText("Port", port_b, IM_ARRAYSIZE(port_b));

            if (ImGui::Button("Save")) {
                b_add_server = false;
                int i = 0;

                int port = 0;

                while (port_b[i] != '\0') {
                    port = (port + (port_b[i] - '0')) * 10;
                    ++i;
                }

                port /= 10;

                Server server(name, hostname, username, password, port);
                servers.push_back(server);

                add_server("../NetPatch.db", name, hostname, username, password, port);
            }

            ImGui::End();
        }

        ImGui::SameLine(93);
        ImGui::Button("Delete Server");

        ImGui::Separator();

        ImGui::Spacing();
        for (int i = 0; i < servers.size(); ++i) {
            ImGui::PushID(i);
            if (ImGui::Button(servers[i].get_name()) || b_servers == i) {
                b_servers = i;
                server_details(servers[b_servers]);
            }
            ImGui::PopID();
        }

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.11f, 0.15f, 0.21f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void server_details(Server server) {
    ImGui::SetNextWindowPos(ImVec2(200, 20));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - 200, ImGui::GetIO().DisplaySize.y - 20));
    ImGui::Begin(server.get_name(), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Columns(2, NULL, false);
    ImGui::Text(server.get_name());
    ImGui::NextColumn();
    ImGui::SetColumnOffset(1, ImGui::GetIO().DisplaySize.x - 250);
    ImGui::Button("Scan Now");

    ImGui::Columns(1);

    ImGui::Spacing();
    
    ImGui::Text("Hostname: ");
    ImGui::SameLine();
    ImGui::Text(server.get_hostname());

    ImGui::Spacing();
    ImGui::Text("Vulnerabilities");

    ImGui::BeginChild("Vulnerabilities List", ImVec2(0, ImGui::GetIO().DisplaySize.y - 200), true);

    ImGui::Text("Critical Vulnerability 1");
    ImGui::Text("A description of the critical vulnerability and its potential impact.");

    ImGui::Separator();

    ImGui::EndChild();

    ImGui::End();
}