#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_impl.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>

#include <string.h>
#include "setupFonts.h"
#include "IconsFontAwesome6.h"
#include "ctextselect.h"

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igButton igButton_Str
#endif
#define igGetIO igGetIO_Nil


const char* lines[] = {
    "Line 1",
    "Line 2",
    "Line 3",
    "A longer line",
    "Text selection in Dear ImGui",
    "UTF-8 characters Ë ⑤ 三【 】┌──┐"
};

size_t getNumLines(void* userdata) {
    return sizeof(lines)/sizeof(lines[0]);
}

const char* getLineAtIdx(size_t idx, void* userdata, size_t* out_len){
    if (out_len != NULL){
      *out_len = strlen(lines[idx]);
    }
    return lines[idx];
}


//------
// main
//------
int main(int argc, char *argv[]) {
  if (!glfwInit())
    return -1;

  // Decide GL+GLSL versions
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  const char *glsl_version = "#version 330";

  // just an extra window hint for resize
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  GLFWwindow *window = glfwCreateWindow(1024, 600, "ImGuiTextSelect with C", NULL, NULL);
  if (!window) {
    printf("Failed to create window! Terminating!\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // enable vsync
  glfwSwapInterval(1);

  // check opengl version sdl uses
  printf("opengl version: %s\n", (char *)glGetString(GL_VERSION));

  // setup imgui
  igCreateContext(NULL);

  // set docking
  ImGuiIO *ioptr = igGetIO();
  ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
                                                              //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
  ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
  //ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
#endif

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  //igStyleColorsDark(NULL);
  igStyleColorsClassic(NULL);
  //igStyleColorsLight(NULL);

  setupFonts();

  bool showDemoWindow = true;
  ImVec4 clearColor = {.x = 0.45f, .y = 0.55f, .z = 0.60f, .w = 1.0f};
  //int showWindowDelay = 1;

  TextSelect* pTextselect = textselect_create(getLineAtIdx, getNumLines, lines, false );

  // main event loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    if (showDemoWindow)
      igShowDemoWindow(&showDemoWindow);

    // show a simple window that we created ourselves.
    igBegin("ImGuiTextSelect demo with C   2025/09", NULL, 0);
      igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
      igSeparatorText("Runing with C API");
      ImVec2 size = {.x = 0, .y = 0};
      igBeginChild_Str("text", size, 0, ImGuiWindowFlags_NoMove);
        size_t num = getNumLines(lines);
          for (size_t i = 0; i < num; ++i) {
              const char* line = getLineAtIdx(i, lines, NULL);
              igTextUnformatted(line, NULL);
          }
        textselect_update(pTextselect);
        if (igBeginPopupContextWindow(NULL, 1)) {
            igBeginDisabled(!textselect_has_selection(pTextselect));
            if (igMenuItem_Bool("Copy", "Ctrl+C", false, true)) {
                textselect_copy(pTextselect);
            }
            igEndDisabled();

            if (igMenuItem_Bool("Select all", "Ctrl+A", false, true)) {
                textselect_select_all(pTextselect);
            }
            if (igMenuItem_Bool("Clear selection", NULL, false, true)) {
                textselect_clear_selection(pTextselect);
            }
            igEndPopup();
        }
      igEndChild();
    igEnd();

    // render
    igRender();
    glfwMakeContextCurrent(window);
    glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
#ifdef IMGUI_HAS_DOCK
    if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_window = glfwGetCurrentContext();
      igUpdatePlatformWindows();
      igRenderPlatformWindowsDefault(NULL, NULL);
      glfwMakeContextCurrent(backup_current_window);
    }
#endif
    glfwSwapBuffers(window);
  }

  // clean up
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(NULL);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
