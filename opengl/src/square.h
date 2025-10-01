#ifndef Square_H
#define Square_H

class Square{
    public:
    float *pPositions;
    unsigned int squareVAO, squareVBO, squareIBO;  // Add VBO member

    void compute();
    void draw();
    Square(float positions[8]);  // Fixed to match implementation needs
    ~Square();
};

#endif