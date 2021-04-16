#define MY_VOXEL_APP
// #define MY_RAYTRACING_APP


#ifdef MY_VOXEL_APP
#include <OpenGLApp.h>
#include <iostream>

int main()
{
    IApp *app = OpenGLApp::Instance();
    app->Initialize();
    app->Run();
    return 0;
}
#endif

#ifdef MY_RAYTRACING_APP

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using Color = glm::vec3;
using Point = glm::vec3;

// Color.h

void WritePixelColor(std::ostream &os, Color pixelColor)
{
    int ir = static_cast<int>(pixelColor.x * 255 + 0.5);
    int ig = static_cast<int>(pixelColor.y * 255 + 0.5);
    int ib = static_cast<int>(pixelColor.z * 255 + 0.5);

    os << ir << " " << ig << " " << ib << "\n" << std::endl;
}

// Ray.h

class Ray
{
public:
    Ray() { }
    Ray(const Point& origin, const glm::vec3& direction)
        : origin(origin), direction(direction)
    {}

    Point Origin() const  { return origin; }
    glm::vec3 Direction() const { return direction; }

    Point at(float t) const {
        return origin + t * direction;
    }
private:
    Point origin;
    glm::vec3 direction;
};

float HitSphere(const Point& center, float radius, const Ray& r) {
    glm::vec3 oc = r.Origin() - center;
    auto a = glm::dot(r.Direction(), r.Direction());
    auto b = 2.0 * glm::dot(oc, r.Direction());
    auto c = glm::dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}


Color RayColor(Ray &ray)
{
    auto t = HitSphere(Point(0,0,-1), 0.5, ray);
    if (t > 0.0) {
        glm::vec3 N = glm::normalize(ray.at(t) - glm::vec3(0, 0, -1));
        return (float)0.5 * Color(N.x + 1, N.y + 1, N.z + 1);
    }

    glm::vec3 unitDirection = glm::normalize(ray.Direction());
    t = 0.5 * (unitDirection.y + 1.0);
    
    return ((float)1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}


int main()
{
    // image base info
    const float aspectRatio = 16.0 / 9.0;
    const int imgWidth = 400;
    const int imgHeight = static_cast<int>(imgWidth / aspectRatio);

    // Camera
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = Point(0, 0, 0);
    auto horizontal = glm::vec3(viewportWidth, 0, 0);
    auto vertical = glm::vec3(0, viewportHeight, 0);
    auto lowerLeftCorner = origin - (float)0.5 * horizontal - (float)0.5 * vertical - glm::vec3(0, 0, focalLength);
    
    std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

    for (int row = imgHeight - 1; row >= 0; --row) {
        std::cerr << "Scanning line: " << row << std::endl;;
        for (int col = 0; col < imgWidth; ++col) {
            float u = (float)(col) / (imgWidth - 1);
            float v = (float)(row) / (imgHeight - 1);

            Ray ray = Ray(origin, lowerLeftCorner + horizontal * u + vertical * v - origin);
            Color pixelColor = RayColor(ray);
            WritePixelColor(std::cout, pixelColor);
        }
    }
    std::cerr << "Done. " << std::endl;
    return 0;
}
#endif

