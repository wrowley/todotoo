/* todotoo main function, including
 * - managing a window and 'dear imgui' context
 * - defining and managing the set of active to-do lists
 *
 * Much code here originated in ../imgui/examples/example_glfw_opengl2/main.cpp
 */

/* We use OpenGL renderer and GLFW platform bindings */
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define TDT_TITLE_LEN (128)


class TDTToDoList
{
    char title[TDT_TITLE_LEN];


public:
    inline TDTToDoList()        { memset(title, 0, TDT_TITLE_LEN); }
    inline ~TDTToDoList()       { }
    inline TDTToDoList(char *s) { memcpy(title, s, TDT_TITLE_LEN); }

    inline char* getTitle()       { return title; }
};

class TDTToDoListSet
{
    int          size;
    TDTToDoList* data;

public:
    inline TDTToDoListSet()        { size = 0; data = NULL; }
    inline ~TDTToDoListSet()       { if (data != NULL) free(data); }

    inline int getSize()                   { return size; }
    inline TDTToDoList* getList(int index) { return &data[index]; }

    void addList(char *title)
    {
        data       = (TDTToDoList*)realloc((TDTToDoList*)data, (size + 1) * sizeof(TDTToDoList));
        data[size] = TDTToDoList(title);
        size++;
    }

};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "todotoo", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Setup Style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    /* All of the lists we have */
    TDTToDoListSet to_do_lists = TDTToDoListSet();

    /* Dialog for creating new to-do list */
    bool dialog_creating_new_list = false;
    bool dialog_creating_new_list_first_focus = true;
    char buf_new_list_name[128];

    /* Actually creating the list */
    bool create_new_list = false;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New"))
                {
                    dialog_creating_new_list = true;
                    dialog_creating_new_list_first_focus = true;
                    memset(buf_new_list_name, 0, IM_ARRAYSIZE(buf_new_list_name));
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (dialog_creating_new_list)
        {
            ImGui::Begin("CREATE", &dialog_creating_new_list);
            if (dialog_creating_new_list_first_focus) ImGui::SetKeyboardFocusHere();
            ImGui::InputText("input text", buf_new_list_name, IM_ARRAYSIZE(buf_new_list_name));
            if (!ImGui::IsItemActive())
            {
                if (strlen(buf_new_list_name) > 0)
                {
                    TDTToDoList el = TDTToDoList(buf_new_list_name);
                    dialog_creating_new_list = false;
                    printf("New list called '%s' will be created\n", buf_new_list_name);
                    to_do_lists.addList(buf_new_list_name);
//                    create_new_list = true;
                }
            }
            dialog_creating_new_list_first_focus = false;
            ImGui::End();
        }

        for (int i = 0; i < to_do_lists.getSize(); i++)
        {
            TDTToDoList *el = to_do_lists.getList(i);

            char buf[128];
            bool window_bool = true;
            ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
            ImGui::PushID(i);
            ImGui::Begin(el->getTitle(), &window_bool);
            ImGui::Checkbox("##0", &window_bool); ImGui::SameLine(); ImGui::InputText("##1", buf, IM_ARRAYSIZE(buf));
            ImGui::End();
            ImGui::PopID();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
