#pragma once

class Sprite {
public:
    struct Position {
        int x;
        int y;
    };

    explicit Sprite(const char* path);
    ~Sprite();

    void draw();

    [[nodiscard]] Position getPosition() const;
    void setPosition(int x, int y);

private:
    unsigned int textureID{};
    unsigned int vao{};
    unsigned int vbo{};
    unsigned int shader{};

    Position position{};
};
