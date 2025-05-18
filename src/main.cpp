#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

// Rotates 'point' around 'axis' passing through 'center' by 'angle' radians
std::vector<float> rotateAroundAxis(
    const std::vector<float>& point,
    const std::vector<float>& center,
    const std::vector<float>& axis,
    float angle)
{
    glm::vec3 p(point[0] - center[0], point[1] - center[1], point[2] - center[2]);
    glm::vec3 ax(axis[0], axis[1], axis[2]);
    glm::vec3 rotated = glm::rotate(p, angle, glm::normalize(ax));
    return {rotated.x + center[0], rotated.y + center[1], rotated.z + center[2]};
}

std::vector<float> crossProduct(const std::vector<float> &a, const std::vector<float> &b)
{
    return {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]};
}

std::vector<float> pointsVector(const std::vector<float> &a, const std::vector<float> &b)
{
    std::vector<float> ab(a.size());
    for (size_t i = 0; i < a.size(); ++i)
    {
        ab[i] = b[i] - a[i];
    }
    return ab;
}

std::vector<float> normalize(const std::vector<float> &v)
{
    float length = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length == 0.0f)
        return {0.0f, 0.0f, 0.0f};
    return {v[0] / length, v[1] / length, v[2] / length};
}

std::vector<float> normal(const std::vector<float> &a, const std::vector<float> &b, const std::vector<float> &c)
{
    std::vector<float> ab = pointsVector(a, b);
    std::vector<float> ac = pointsVector(a, c);
    return normalize(crossProduct(ab, ac));
}

std::vector<float> midpoint(std::vector<float> c1, std::vector<float> c2)
{
    float x = (c1[0] + c2[0]) / 2;
    float y = (c1[1] + c2[1]) / 2;
    float z = (c1[2] + c2[2]) / 2;
    std::vector<float> midpoint;
    midpoint.insert(midpoint.end(), x);
    midpoint.insert(midpoint.end(), y);
    midpoint.insert(midpoint.end(), z);
    return midpoint;
}

bool vectorEquals(const std::vector<float> &v1, const std::vector<float> &v2)
{
    if (v1.size() != v2.size())
        return false;
    for (size_t i = 0; i < v1.size(); ++i)
    {
        if (std::abs(v1[i] - v2[i]) > 1e-1)
            return false;
    }
    return true;
}

bool normalsEqual(const std::vector<float> &n1, const std::vector<float> &n2, float epsilon = 1e-1)
{
    float dot = n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2];
    // For normalized vectors, dot == 1 means same direction, -1 means opposite
    return std::abs(std::abs(dot) - 1.0f) < epsilon;
}

const std::vector<float> normal1 = normal(f1vertex1, f1vertex2, f1vertex3);
const std::vector<float> normal2 = normal(f2vertex1, f2vertex2, f2vertex3);
const std::vector<float> normal3 = normal(f3vertex1, f3vertex2, f3vertex3);
const std::vector<float> normal4 = normal(f4vertex1, f4vertex2, f4vertex3);

void drawTriangle(std::vector<float> a, std::vector<float> b, std::vector<float> c, std::vector<float> &vertices)
{
    std::vector<float> n = normal(a, b, c);

    // List of face normals and colors
    const std::vector<std::vector<float>> faceNormals = {normal1, normal2, normal3, normal4};
    const std::vector<std::vector<float>> faceColors = {color1, color2, color3, color4};

    // Find the face normal with the largest absolute dot product
    float maxDot = -1.0f;
    int bestFace = 0;
    for (int i = 0; i < 4; ++i)
    {
        float dot = std::abs(n[0] * faceNormals[i][0] + n[1] * faceNormals[i][1] + n[2] * faceNormals[i][2]);
        if (dot > maxDot)
        {
            maxDot = dot;
            bestFace = i;
        }
    }

    // Use the color of the closest face
    const std::vector<float> &color = faceColors[bestFace];
    vertices.insert(vertices.end(), a.begin(), a.end());
    vertices.insert(vertices.end(), color.begin(), color.end());
    vertices.insert(vertices.end(), b.begin(), b.end());
    vertices.insert(vertices.end(), color.begin(), color.end());
    vertices.insert(vertices.end(), c.begin(), c.end());
    vertices.insert(vertices.end(), color.begin(), color.end());
}

void drawST(std::vector<float> a, std::vector<float> b, std::vector<float> c, int depth,
            std::vector<float> &vertices)
{
    if (depth < maxDepth)
    {
        std::vector<float> mid1 = midpoint(a, b);
        std::vector<float> mid2 = midpoint(b, c);
        std::vector<float> mid3 = midpoint(c, a);

        drawST(a, mid1, mid3, depth + 1, vertices);
        drawST(mid1, b, mid2, depth + 1, vertices);
        drawST(mid3, mid2, c, depth + 1, vertices);

        std::vector<float> center = {
            (mid1[0] + mid2[0] + mid3[0]) / 3.0f,
            (mid1[1] + mid2[1] + mid3[1]) / 3.0f,
            (mid1[2] + mid2[2] + mid3[2]) / 3.0f};
        std::vector<float> n = normal(mid1, mid2, mid3);

        float edgeLength = std::sqrt(
            (mid1[0] - mid2[0]) * (mid1[0] - mid2[0]) +
            (mid1[1] - mid2[1]) * (mid1[1] - mid2[1]) +
            (mid1[2] - mid2[2]) * (mid1[2] - mid2[2]));
        float height = std::sqrt(2.0f / 3.0f) * edgeLength;

        std::vector<float> inner1 = {mid1[0] - n[0] * height, mid1[1] - n[1] * height, mid1[2] - n[2] * height};
        std::vector<float> inner2 = {mid2[0] - n[0] * height, mid2[1] - n[1] * height, mid2[2] - n[2] * height};
        std::vector<float> inner3 = {mid3[0] - n[0] * height, mid3[1] - n[1] * height, mid3[2] - n[2] * height};

        float angle = glm::radians(60.0f);
        std::vector<float> rotated1 = rotateAroundAxis(inner1, center, n, angle);
        std::vector<float> rotated2 = rotateAroundAxis(inner2, center, n, angle);
        std::vector<float> rotated3 = rotateAroundAxis(inner3, center, n, angle);

        drawST(rotated1, rotated2, rotated3, depth + 1, vertices);
    }
    else
    {
        drawTriangle(a, b, c, vertices);
    }
}

void drawInverseST(std::vector<float> a, std::vector<float> b, std::vector<float> c, int depth,
            std::vector<float> &vertices)
{
    std::vector<float> mid1 = midpoint(a, b);
    std::vector<float> mid2 = midpoint(b, c);
    std::vector<float> mid3 = midpoint(c, a);
    std::vector<float> center = {
        (mid1[0] + mid2[0] + mid3[0]) / 3.0f,
        (mid1[1] + mid2[1] + mid3[1]) / 3.0f,
        (mid1[2] + mid2[2] + mid3[2]) / 3.0f};
    std::vector<float> n = normal(mid1, mid2, mid3);

    float edgeLength = std::sqrt(
        (mid1[0] - mid2[0]) * (mid1[0] - mid2[0]) +
        (mid1[1] - mid2[1]) * (mid1[1] - mid2[1]) +
        (mid1[2] - mid2[2]) * (mid1[2] - mid2[2]));
    float height = std::sqrt(2.0f / 3.0f) * edgeLength;

    std::vector<float> inner1 = {mid1[0] - n[0] * height, mid1[1] - n[1] * height, mid1[2] - n[2] * height};
    std::vector<float> inner2 = {mid2[0] - n[0] * height, mid2[1] - n[1] * height, mid2[2] - n[2] * height};
    std::vector<float> inner3 = {mid3[0] - n[0] * height, mid3[1] - n[1] * height, mid3[2] - n[2] * height};

    float angle = glm::radians(60.0f);
    std::vector<float> rotated1 = rotateAroundAxis(inner1, center, n, angle);
    std::vector<float> rotated2 = rotateAroundAxis(inner2, center, n, angle);
    std::vector<float> rotated3 = rotateAroundAxis(inner3, center, n, angle);

    drawTriangle(mid1, mid2, mid3, vertices);
    drawTriangle(rotated1, rotated2, rotated3, vertices);
    if (depth < maxDepth)
    {
        drawInverseST(a, mid1, mid3, depth + 1, vertices);
        drawInverseST(mid1, b, mid2, depth + 1, vertices);
        drawInverseST(mid3, mid2, c, depth + 1, vertices);
        drawInverseST(rotated1, rotated2, rotated3, depth + 1, vertices);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("src/shader.vs", "src/shader.fs"); 

    // Tetrahedron vertices
    std::vector<float> vertices;

    //Sierpinski
    drawST(f1vertex1, f1vertex2, f1vertex3, 0, vertices);
    drawST(f2vertex1, f2vertex2, f2vertex3, 0, vertices);
    drawST(f3vertex1, f3vertex2, f3vertex3, 0, vertices);
    drawST(f4vertex1, f4vertex2, f4vertex3, 0, vertices);

    //Inverse Sierpinski
    //drawInverseST(f1vertex1, f1vertex2, f1vertex3, 0, vertices);
    //drawInverseST(f2vertex1, f2vertex2, f2vertex3, 0, vertices);
    //drawInverseST(f3vertex1, f3vertex2, f3vertex3, 0, vertices);
    //drawInverseST(f4vertex1, f4vertex2, f4vertex3, 0, vertices);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        
        ourShader.setMat4("projection", projection);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}