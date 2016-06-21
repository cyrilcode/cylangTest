#include <cyfw/main.h>
#include <cyfw/Gui.h>
#include <Resource.h>
#include <Cyril/CyrilParser.h>

using namespace std;
using namespace cy;

class MyApp : public cy::App
{
    Gui gui;
    bool guiOpen;
    char text[1024*16];
    bool escPressed;
    bool shouldQuit;
    ImFont* font1;
public:
    MyApp() : escPressed{false}, shouldQuit{false} {
        char * t = "rotate\nbox\0";
        memcpy(text, t, 11);
        Resource fontdata = LOAD_RESOURCE(a_scp_r_ttf);
        auto font_cfg_template = ImFontConfig();
        font_cfg_template.FontDataOwnedByAtlas = false;
        font1 = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
                (void*)fontdata.begin(), fontdata.size(), 32, &font_cfg_template);
    }

    void setup()
    {
        window->closeOnEscapeKey(false);
        window->setClearColor({0,0,0,1});
        gui.init(window);
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0;
        style.WindowPadding = {0, 0};
        style.Colors[ImGuiCol_Text] = {1,1,1,1};
        style.Colors[ImGuiCol_WindowBg] = {0,0,0,0};
        style.Colors[ImGuiCol_FrameBg] = {0,0,0,0};
    }

    void draw()
    {
        window->clear();
        gui.clear();

        doGui();

        gui.draw();
    }

    void doGui()
    {
        ImGui::SetNextWindowPos(ImVec2(10,10));
        ImGuiWindowFlags windowFlags =
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
        if (!ImGui::Begin("Buffer", &guiOpen, windowFlags))
        {
            ImGui::End();
            return;
        }
        ImGui::PushFont(font1);
        vec2i dim = window->getWindowSize();
        ImGui::SetWindowSize(ImVec2(dim.x() - 20, dim.y() - 20));

        ImGui::InputTextMultiline("source", text, sizeof(text), ImVec2(-1.0f, -1.0f), 0);

        if (escPressed) ImGui::OpenPopup("Quit?");
        if (ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove))
        {
            ImGui::SetWindowFontScale(0.5f);
            ImGui::Text("Are you sure you want to quit?\n\n");
            ImGui::Separator();

            if (ImGui::Button("Quit", ImVec2(120,0))) {
                escPressed = false;
                shouldQuit = true;
                window->quit();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120,0))) {
                escPressed = false;
                window->quit(false);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::PopFont();
        ImGui::End();
    }

    bool quit()
    {
        escPressed = true;
        return shouldQuit;
    }

    void key(window::KeyEvent e)
    {
        if (e.key == window::key::R && e.action == window::action::PRESSED && e.mods == window::mods::SUPER)
        {
            cout << "Program:" << endl << text << endl;
            CyrilParser parser;
            Cyril* prog = parser.parseString(text);
            cout << "Compiled:" << endl;
            prog->print();
        }
        if (e.key == window::key::ESCAPE && e.action == window::action::PRESSED)
        {
            escPressed = true;
        }
        gui.key(e);
    }
    void scroll(window::ScrollEvent e) { gui.scroll(e); }
    void textInput(window::CharEvent e) { gui.character(e); }
    void mouseButton(window::MouseButtonEvent e ) { gui.mouse(e); }
};

int main() {
    cy::run<MyApp,ConsoleLogger>(640, 480, "testing editor");
}
