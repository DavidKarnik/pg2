#include "App.h"

float App::GetHeightmapY(float position_x, float position_z) const
{
    float X = position_x + HEIGHTMAP_SHIFT;
    float Z = position_z + HEIGHTMAP_SHIFT;
    float Y = 0.0f;

    float X_floor = std::floor(X);
    float Z_floor = std::floor(Z);

    float X_ceil = std::ceil(X);
    float Z_ceil = std::ceil(Z);

    if (X - X_floor < 0.5f && Z - Z_floor < 0.5f) {
        // In the lower-left triangle
        float x_fraction = X - X_floor;
        float y_fraction = Z - Z_floor;
        float common_height = (*_heights)[{X_floor, Z_floor}];
        float x_difference = (*_heights)[{X_ceil, Z_floor}] - common_height;
        float y_difference = (*_heights)[{X_floor, Z_ceil}] - common_height;
        Y = common_height + x_fraction * x_difference + y_fraction * y_difference;
    }
    else {
        // In the upper-right triangle
        float x_fraction = X_ceil - X;
        float y_fraction = Z_ceil - Z;
        float common_height = (*_heights)[{X_ceil, Z_ceil}];
        float x_difference = common_height - (*_heights)[{X_floor, Z_ceil}];
        float y_difference = common_height - (*_heights)[{X_ceil, Z_floor}];
        Y = common_height - x_fraction * x_difference - y_fraction * y_difference;
    }
    return Y * HEGHTMAP_SCALE;
}