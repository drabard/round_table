#include "gui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "window.h"

void gui_init(struct window* window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  ImGui_ImplGlfw_InitForOpenGL(window->glfw_win, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  ImGui::StyleColorsDark();

  unsigned char* tex_pixels = NULL;
  int tex_w, tex_h;
  io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);
}

bool gui_wants_input() {
  ImGuiIO& io = ImGui::GetIO();
  return io.WantCaptureMouse || io.WantCaptureKeyboard;
}

void gui_shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void gui_prepare(struct window* window) {
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2(window->width, window->height);
  io.DeltaTime = 1.0f / 60.0f;
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void gui_draw() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}