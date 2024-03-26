#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>


// Shader sources for vertex and fragment shaders.
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 objectColor;\n"
"void main()\n"
"{\n"
"   FragColor = objectColor;\n"
"}\0";

// Adjust viewport on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // OpenGL version and context configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Circle Approximation by n-gon", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();


    // Shader compilation and linking
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Circle parameters
    const int numVertices = 100; // Increase for smoother circle
    GLfloat circleVertices[3 * numVertices];
    const float radius = 0.5f;
    const float M_PI = 3.14159265358979323846f;

    // Generate circle vertices
    for (int i = 0; i < numVertices; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(numVertices);
        circleVertices[3 * i] = cos(angle) * radius;
        circleVertices[3 * i + 1] = sin(angle) * radius;
        circleVertices[3 * i + 2] = 0.0f;
    }

    // Circle VAO and VBO setup
    GLuint circleVAO, circleVBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // n-gon VAO and VBO setup (similar to circle, vertices updated dynamically in the loop)
    GLuint nGonVAO, nGonVBO;
    glGenVertexArrays(1, &nGonVAO);
    glGenBuffers(1, &nGonVBO);
    glBindVertexArray(nGonVAO);
    glBindBuffer(GL_ARRAY_BUFFER, nGonVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    int n = 2; // Starting with a triangle
    float lastFrame = glfwGetTime();


    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        if (currentFrame - lastFrame >= 1.0f) {
            n = n >= 12 ? 3 : n + 1; // Increment n, loop back to 3 if over 12
            lastFrame = currentFrame;

            std::vector<GLfloat> nGonVertices(3 * n);
            float angleOffset = M_PI / 2; // Offset to start from the top of the circle
            for (int i = 0; i < n; ++i) {
                float angle = angleOffset - (2.0f * M_PI * float(i) / float(n));
                nGonVertices[3 * i] = cos(angle) * radius;
                nGonVertices[3 * i + 1] = sin(angle) * radius;
                nGonVertices[3 * i + 2] = 0.0f;
            }

            // Update the nGonVBO with the new vertices
            glBindBuffer(GL_ARRAY_BUFFER, nGonVBO);
            glBufferData(GL_ARRAY_BUFFER, nGonVertices.size() * sizeof(GLfloat), nGonVertices.data(), GL_STATIC_DRAW);
        }

        // Render
        glClearColor(0.89f, 0.89f, 0.89f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        int objectColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
        glUniform4f(objectColorLocation, 0.96f, 0.76f, 0.0f, 1.0f);

        // Draw circle
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);

        glUniform4f(objectColorLocation, 0.22f, 0.51f, 0.81f, 1.0f);

        // Draw n-gon
        glBindVertexArray(nGonVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, n);

        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);
    glDeleteVertexArrays(1, &nGonVAO);
    glDeleteBuffers(1, &nGonVBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
