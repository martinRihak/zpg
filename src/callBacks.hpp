static void error_callback(int error, const char *description) { fputs(description, stderr); }

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == 265 && action == GLFW_PRESS) // L key
        rotateLeft = !rotateLeft;
    printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

static void window_size_callback(GLFWwindow *window, int width, int height)
{
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
}

static void button_callback(GLFWwindow *window, int button, int action, int mode)
{
    if (action == GLFW_PRESS)
        printf("button_callback [%d,%d,%d]\n", button, action, mode);
}