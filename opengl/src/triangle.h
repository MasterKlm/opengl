#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle{
    public:
    float *pPositions;
    unsigned int triangleVAO, triangleVBO;  // Make these member variables

    void compute();
    void draw();
    Triangle(float positions[6]);
    ~Triangle();  // Add destructor for cleanup
};

#endif