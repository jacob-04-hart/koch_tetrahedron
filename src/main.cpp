#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

#include <iostream>
#include <cmath>
#include <vector>

const std::vector<float> color1 = {1.0f, 0.0f, 0.0f}; // red
const std::vector<float> color2 = {0.0f, 1.0f, 0.0f}; // green
const std::vector<float> color3 = {0.0f, 0.0f, 1.0f}; // blue
const std::vector<float> color4 = {1.0f, 1.0f, 0.0f}; // yellow

const unsigned int maxDepth = 3;

// Face 1
const std::vector<float> f1vertex1 = {.5f, .5f, .5f};
const std::vector<float> f1vertex2 = {-.5f, -.5f, .5f};
const std::vector<float> f1vertex3 = {.5f, -.5f, -.5f};

// Face 2
const std::vector<float> f2vertex1 = {.5f, .5f, .5f};
const std::vector<float> f2vertex2 = {.5f, -.5f, -.5f};
const std::vector<float> f2vertex3 = {-.5f, .5f, -.5f};

// Face 3
const std::vector<float> f3vertex1 = {.5f, .5f, .5f};
const std::vector<float> f3vertex2 = {-.5f, .5f, -.5f};
const std::vector<float> f3vertex3 = {-.5f, -.5f, .5f};

// Face 4
const std::vector<float> f4vertex1 = {-.5f, -.5f, .5f};
const std::vector<float> f4vertex2 = {-.5f, .5f, -.5f};
const std::vector<float> f4vertex3 = {.5f, -.5f, -.5f};

std::vector<float> crossProduct(const std::vector<float>& a, const std::vector<float>& b) {
    return {
        a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]
    };
}

std::vector<float> pointsVector(const std::vector<float>& a, const std::vector<float>& b) {
    std::vector<float> ab(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        ab[i] = b[i] - a[i];
    }
    return ab;
} 

std::vector<float> normalize(const std::vector<float>& v) {
    float length = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (length == 0.0f) return {0.0f, 0.0f, 0.0f};
    return { v[0]/length, v[1]/length, v[2]/length };
}

std::vector<float> normal(const std::vector<float>& a, const std::vector<float>& b,const std::vector<float>& c) {
    std::vector<float> ab = pointsVector(a,b);
    std::vector<float> ac = pointsVector(a,c);
    return normalize(crossProduct(ab,ac));
}

std::vector<float> midpoint(std::vector<float> c1,std::vector<float> c2) {
    float x = (c1[0]+c2[0])/2;
    float y = (c1[1]+c2[1])/2;
    float z = (c1[2]+c2[2])/2;
    std::vector<float> midpoint;
    midpoint.insert(midpoint.end(),x);
    midpoint.insert(midpoint.end(),y);
    midpoint.insert(midpoint.end(),z);
    return midpoint;
}

const std::vector<float> normal1 = normal(f1vertex1,f1vertex2,f1vertex3);
const std::vector<float> normal2 = normal(f2vertex1,f2vertex2,f2vertex3);
const std::vector<float> normal3 = normal(f3vertex1,f3vertex2,f3vertex3);
const std::vector<float> normal4 = normal(f4vertex1,f4vertex2,f4vertex3);

bool vectorEquals(const std::vector<float>& v1, const std::vector<float>& v2) {
    if (v1.size() != v2.size()) return false;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (std::abs(v1[i] - v2[i]) > 1e-6) return false;
    }
    return true;
}

void drawTriangle(std::vector<float> a, std::vector<float> b, std::vector<float> c, std::vector<float> &vertices){
    std::vector<float> normalVector = normal(a,b,c);
    if(vectorEquals(normalVector, normal1)){
        vertices.insert(vertices.end(),a.begin(),a.end());
        vertices.insert(vertices.end(),color1.begin(),color1.end());
        vertices.insert(vertices.end(),b.begin(),b.end());
        vertices.insert(vertices.end(),color1.begin(),color1.end());
        vertices.insert(vertices.end(),c.begin(),c.end());
        vertices.insert(vertices.end(),color1.begin(),color1.end());
    } else if(vectorEquals(normalVector, normal2)){
        vertices.insert(vertices.end(),a.begin(),a.end());
        vertices.insert(vertices.end(),color2.begin(),color2.end());
        vertices.insert(vertices.end(),b.begin(),b.end());
        vertices.insert(vertices.end(),color2.begin(),color2.end());
        vertices.insert(vertices.end(),c.begin(),c.end());
        vertices.insert(vertices.end(),color2.begin(),color2.end());
    } else if(vectorEquals(normalVector, normal3)){
        vertices.insert(vertices.end(),a.begin(),a.end());
        vertices.insert(vertices.end(),color3.begin(),color3.end());
        vertices.insert(vertices.end(),b.begin(),b.end());
        vertices.insert(vertices.end(),color3.begin(),color3.end());
        vertices.insert(vertices.end(),c.begin(),c.end());
        vertices.insert(vertices.end(),color3.begin(),color3.end());
    } else if(vectorEquals(normalVector, normal4)){
        vertices.insert(vertices.end(),a.begin(),a.end());
        vertices.insert(vertices.end(),color4.begin(),color4.end());
        vertices.insert(vertices.end(),b.begin(),b.end());
        vertices.insert(vertices.end(),color4.begin(),color4.end());
        vertices.insert(vertices.end(),c.begin(),c.end());
        vertices.insert(vertices.end(),color4.begin(),color4.end());
    }
};

void drawKT(std::vector<float> a, std::vector<float> b, std::vector<float> c, int depth,
            std::vector<float> &vertices)
{
    if (depth < maxDepth){

    }
    else{
        drawTriangle(a,b,c,vertices);
    }
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("src/shader.vs", "src/shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // Tetrahedron vertices

    float vertices[] = {};

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render container
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}