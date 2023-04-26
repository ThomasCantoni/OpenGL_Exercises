#pragma once
#include <string>

typedef struct GLFWwindow GLFWwindow;

class OGLWindow 
{
public:
    OGLWindow(int InWidth, int InHeight, const std::string& InTitle);
    ~OGLWindow();
    
    void PrintInfo();
    bool IsOpened();
    float GetDeltaTime();
    void Update();
    void SetTitle(const std::string& InTitle);

private:
    int Width;
    int Height;
    std::string Title;
    GLFWwindow* RawWindow;
    float DeltaTime;
    float LastTime;
};