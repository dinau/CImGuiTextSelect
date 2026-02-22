#include <stdio.h>
#include <string.h>
#include "appImGui.h"

#include "ctextselect.h"

const char* lines[] = {
    "Line 1",
    "Line 2",
    "Line 3",
    "A longer line",
    "Text selection in Dear ImGui",
    "UTF-8 characters Ë ⑤ 三【 】┌──┐",
    "世界平和"
};

size_t getNumLines(void* userdata) {
    const char** clines = (const char**)userdata;
    size_t count = 0;
    while (clines[count] != NULL) count++;
    return count;
}

const char* getLineAtIdx(size_t idx, void* userdata, size_t* out_len){
    const char** clines = (const char**)userdata;
    if (out_len != NULL){
      *out_len = strlen(clines[idx]);
    }
    return clines[idx];
}

//------
// main
//------
void gui_main(Window *window) {

  setupFonts();

  bool showDemoWindow = true;
  ImVec4 clearColor = {.x = 0.45f, .y = 0.55f, .z = 0.60f, .w = 1.0f};
  //int showWindowDelay = 1;

  TextSelect* pTextselect = textselect_create(getLineAtIdx, getNumLines, lines, false );

  // main event loop
  while (!glfwWindowShouldClose(window->handle)) {
    glfwPollEvents();
    newFrame();
    // start imgui frame

    if (showDemoWindow)
      igShowDemoWindow(&showDemoWindow);

    // show a simple window that we created ourselves.
    igBegin("ImGuiTextSelect demo with C   2025/09", NULL, 0);
      igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
      igSeparatorText("Runing with C API");
      ImVec2 size = {.x = 0, .y = 0};
      igBeginChild_Str("text", size, 0, ImGuiWindowFlags_NoMove);
        igPushFont(NULL, 30); // Zoom font
        size_t num = getNumLines(lines);
        for (size_t i = 0; i < num; ++i) {
          const char* line = getLineAtIdx(i, lines, NULL);
          igTextUnformatted(line, NULL);
        }
        textselect_update(pTextselect);
        igPopFont();
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
        render(window);
    }
}

//------
// main
//------
int main(int argc, char *argv[]) {
  Window *window = createImGui(1024, 768);

  gui_main(window);

  destroyImGui(window);
}
