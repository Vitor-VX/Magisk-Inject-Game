#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <asm-generic/mman.h>
#include <sys/mman.h>
#include "Roboto-Regular.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"
#include "backends/android_native_app_glue.h"
#include "Dobby/dobby.h"
#include "Utils.h"
#include <cstring>
#include "../Funcoes.h"
#include "../Request/HttpRequest.h"
//#include "../Request/HttpRequest.h"
//#include "obfuscate.h"

#ifndef ZYGISK_DUMP_ORIGINAL_MENUINJECTOR_H
#define ZYGISK_DUMP_ORIGINAL_MENUINJECTOR_H

bool isInitialized = false;
using inject_event_orig = int (*)(JNIEnv *env, jobject thiz, jobject inputEvent);
inject_event_orig o_inject_event = nullptr;

int glWidth = 440;
int glHeight = 450;

void createButtonImgui(std::string Text, Funcoes &funcoes, bool Funcoes::*field, float buttonWidth) {
    bool fieldValue = funcoes.*field;

    // Ajusta a cor do botão e da borda de acordo com o estado
    if (fieldValue) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f)); // Verde mais forte quando ativado
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.8f, 0.0f, 1.0f)); // Borda verde quando ativado
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Preto mais claro quando desligado
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Borda preta quando desligado
    }

    if (ImGui::Button((fieldValue) ? (Text + " ON").c_str() : (Text + " OFF").c_str(),
                      ImVec2(buttonWidth, 0))) {
        funcoes.*field = !fieldValue;
    }

    ImGui::PopStyleColor(2);
}

void SetupImgui() {
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = ImVec2((float) glWidth, (float) glHeight);

    // Configurar estilos
    ImGuiStyle *style = &ImGui::GetStyle();
    style->WindowBorderSize = 0;
    style->WindowRounding = 10.0f;
    style->WindowTitleAlign = ImVec2(0.5, 0.5);
    style->WindowPadding = ImVec2(20, 20); // Ajuste o padding horizontal e vertical aqui
    style->WindowMinSize = ImVec2(100, 100);

    // Ajustar a altura dos botões
    style->ItemSpacing.y = 10; // Ajuste a altura dos botões aqui

    // Configurar cores
    ImVec4 *colors = style->Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Preto para o fundo do menu
    colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Preto mais claro para botões desligados
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Preto mais claro para botões desligados quando hover
    colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.8f, 0.0f, 1.0f); // Verde mais escuro para botões ligados
    colors[ImGuiCol_Border] = ImVec4(0.0f, 0.8f, 0.0f, 1.0f); // Borda verde
    colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Texto branco

    ImGui_ImplOpenGL3_Init("#version 100");

    // Adicionar fonte
    ImFontConfig fontConfig;
    fontConfig.SizePixels = 26.0f;
    fontConfig.FontDataOwnedByAtlas = false;
    ImFont *font = io.Fonts->AddFontFromMemoryTTF((void *) Roboto_Regular, 168260, 24.0f, &fontConfig,
                                                  io.Fonts->GetGlyphRangesChineseFull());
    IM_ASSERT(font != NULL);

    ImGui::GetStyle().ScaleAllSizes(2.0f);
}

void Menu() {
    ImGui::SetNextWindowSize(ImVec2(550, 550), ImGuiCond_Appearing);
    ImGui::Begin("Vx Team Plugin", 0, ImGuiWindowFlags_NoResize);

    float buttonWidth = ImGui::GetWindowWidth() - ImGui::GetStyle().FramePadding.x * 2;

    ImGui::Text("Funções Online - Test");

    //createButtonImgui("LIMPAR PERSON", funcoes, &Funcoes::clearPerson, buttonWidth);

    createButtonImgui("TEST FUNCTION", funcoes, &Funcoes::vida, buttonWidth);

    ImGui::Text("Funções Game Normal");

    createButtonImgui("AUTO KILL", funcoes, &Funcoes::hpZero, buttonWidth);

    createButtonImgui("VIDA INFINITA TEST", funcoes, &Funcoes::maxHp, buttonWidth);

    createButtonImgui("DANO AUMENTADO TEST", funcoes, &Funcoes::increasedDamage, buttonWidth);

    createButtonImgui("SPEED", funcoes, &Funcoes::setSpeed, buttonWidth);

    ImGui::End();
}

void *(*orig_Input)(void *, void *, const void *);
void *hook_Input(void *inst, void *motionEvent, const void *inputMessage) {
    void *result = orig_Input(inst, motionEvent, inputMessage);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *) inst);
    return result;
}

EGLBoolean (*o_swapbuffers)(EGLDisplay dpy, EGLSurface surface);

EGLBoolean swapbuffers_hook(EGLDisplay dpy, EGLSurface surf) {
    eglQuerySurface(dpy, surf, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surf, EGL_HEIGHT, &glHeight);

    if (!isInitialized) {
        SetupImgui();
        isInitialized = true;
    }

    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();

    Menu();

    ImGui::Render();
    ImGui::EndFrame();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
    glClearColor(0.0, 0.0, 0, 0.0);

    return o_swapbuffers(dpy, surf);
}

int inject_event_hook(JNIEnv *env, jobject __this, jobject input_event) {
    jclass motion_event = env->FindClass(("android/view/MotionEvent"));

    if (!motion_event) {
        LOGI("Can't find MotionEvent!");
        return o_inject_event(env, __this, input_event);
    }

    if (env->IsInstanceOf(input_event, motion_event)) {
        jmethodID get_action = env->GetMethodID(motion_event, ("getActionMasked"), ("()I"));
        jmethodID get_x = env->GetMethodID(motion_event, ("getX"), ("()F"));
        jmethodID get_y = env->GetMethodID(motion_event, ("getY"), ("()F"));
        jmethodID get_pointer_count = env->GetMethodID(motion_event, ("getPointerCount"), ("()I"));

        ImGui_ImplAndroid_HandleInputEvent(env->CallIntMethod(input_event, get_action),
                                           env->CallFloatMethod(input_event, get_x),
                                           env->CallFloatMethod(input_event, get_y),
                                           env->CallIntMethod(input_event, get_pointer_count));

        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse) {
            return true;
        }
    }

    jclass key_event = env->FindClass("android/view/KeyEvent");

    if (!key_event) {
        LOGI("Can't find KeyEvent!");
        return o_inject_event(env, __this, input_event);
    }

    if (env->IsInstanceOf(input_event, key_event)) {
        jmethodID get_unicode_char = env->GetMethodID(key_event, ("getUnicodeChar"), ("()I"));
        jmethodID get_meta_state = env->GetMethodID(key_event, ("getMetaState"), ("()I"));

        ImGuiIO &io = ImGui::GetIO();
        io.AddInputCharacter(env->CallIntMethod(input_event, get_unicode_char,
                                                env->CallIntMethod(input_event, get_meta_state)));
    }
    return o_inject_event(env, __this, input_event);
}

#endif //ZYGISK_DUMP_ORIGINAL_MENUINJECTOR_H