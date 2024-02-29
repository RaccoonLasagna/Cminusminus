#include <algorithm>
#include <iostream>
#include <vector>
#include <array>

using namespace std;

#ifndef INTERPOLATE_H
#define INTERPOLATE_H

class Var3DGraph {
protected:
    vector<array<int, 4>> exampleData;

public:
    Var3DGraph(const vector<array<int, 4>>& exampleData) {
        // กำหนด exampleData โดยเรียงจากน้อยไปมากตาม x, y, z
        this->exampleData = exampleData;
        sort(this->exampleData.begin(), this->exampleData.end(), [](const auto& a, const auto& b) {
            if (a[0] != b[0]) // เรียงตาม x ถ้าไม่เท่ากัน
                return a[0] < b[0];
            else if (a[1] != b[1]) // เรียงตาม y ถ้า x เท่ากันและ y ไม่เท่ากัน
                return a[1] < b[1];
            else // เรียงตาม z ถ้า x และ y เท่ากัน
                return a[2] < b[2];
        });
    }

    int interpolate(int x, int y, int z) {
        // ตรวจสอบว่าจุดอยู่นอกขอบเขตข้อมูลหรือไม่
        if (x < exampleData.front()[0] || x > exampleData.back()[0] ||
            y < exampleData.front()[1] || y > exampleData.back()[1] ||
            z < exampleData.front()[2] || z > exampleData.back()[2]) {
            return 0;
        }

        // หาตำแหน่งข้อมูลที่อยู่ระหว่าง x, y, z
        size_t i = 0;
        while (i < exampleData.size() - 1 && exampleData[i][0] < x) {
            ++i;
        }

        // ทำ interpolation แยกตามแต่ละมิติ
        int x1 = exampleData[i][0], y1 = exampleData[i][1], z1 = exampleData[i][2], val1 = exampleData[i][3];
        int x2 = exampleData[i + 1][0], y2 = exampleData[i + 1][1], z2 = exampleData[i + 1][2], val2 = exampleData[i + 1][3];

        // คำนวณอัตราส่วนของ interpolation สำหรับแต่ละมิติ
        double alpha_x = static_cast<double>(x - x1) / (x2 - x1);
        double alpha_y = static_cast<double>(y - y1) / (y2 - y1);
        double alpha_z = static_cast<double>(z - z1) / (z2 - z1);

        // ทำ interpolation แยกตามแต่ละมิติ
        int val_x1 = val1 + alpha_x * (val2 - val1);
        int val_x2 = val1 + alpha_x * (val2 - val1);
        int val_y1 = val1 + alpha_y * (val2 - val1);
        int val_y2 = val1 + alpha_y * (val2 - val1);
        int val_z1 = val1 + alpha_z * (val2 - val1);
        int val_z2 = val1 + alpha_z * (val2 - val1);

        // ทำ interpolation สุดท้าย
        int val_xy1 = val_x1 + alpha_y * (val_x2 - val_x1);
        int val_xy2 = val_y1 + alpha_y * (val_y2 - val_y1);
        int val_xyz1 = val_xy1 + alpha_z * (val_xy2 - val_xy1);

        return val_xyz1;
    }
};

#endif