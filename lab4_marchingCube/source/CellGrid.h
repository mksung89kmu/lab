#ifndef CELLGRID_H
#define CELLGRID_H
#include <iostream>
#include <FastNoise.h>

using namespace std;


class CellGrid
{
    public:
        float ***cells;
        int width = 0;
        int height = 0;
        int depth = 0;

        CellGrid();
        CellGrid(int _width, int _height, int _depth);

        // fill the grid with 3D noise values
        void fillGrid(FastNoise& noise, int octaves, float lacunarity, float persistance, float scale);
        virtual ~CellGrid();

        void print() 
        {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    for (int k = 0; k < depth; k++) {
                        std::cout << cells[i][j][k] << " ";
                    }
                    std::cout << endl;
                }
                std::cout << endl;
            }

        }
    protected:

    private:
};

#endif // CELLGRID_H
