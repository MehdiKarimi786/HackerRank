#include <vector>

using namespace std;

void matrixRotation(vector<vector<int>> matrix, int r) 
{
    auto rows = matrix.size();
    auto cols = matrix[0].size();

    auto getLayer = [&](int row, int col) {
        auto layerVertical = std::min<int>(row, rows - 1 - row);
        auto layerHorizontal = std::min<int>(col, cols - 1 - col);
        return std::min<int>(layerVertical, layerHorizontal);
    };

    auto getValue = [&](int row, int col) {
        auto layer = getLayer(row, col);
        auto count = 2 * rows + 2 * cols - 8 * layer - 4;
        auto routate = r % count;
        while (routate > 0)
        {
            if (col == layer)
            {
                if (row > layer)
                {
                    --row;
                }
                else
                {
                    ++col;
                }
            }
            else if (row == layer)
            {
                if (col < (cols - layer - 1))
                {
                    ++col;
                }
                else
                {
                    ++row;
                }
            }
            else if (col == (cols - layer - 1))
            {
                if (row < (rows - layer - 1))
                {
                    ++row;
                }
                else
                {
                    --col;
                }
            }
            else if (row == (rows - layer - 1))
            {
                if (col > layer)
                {
                    --col;
                }
                else
                {
                    --row;
                }
            }
            --routate;
        }
        return matrix[row][col];
    };

    for (auto row(0); row < rows; ++row)
    {
        for (auto col(0); col < cols; ++col)
        {
            printf("%d ", getValue(row, col));
        }
        printf("\n");
    }

}

int main()
{
    matrixRotation({ {1,2,3,4},
        {7, 8, 9, 10},
        {13, 14 ,15 ,16},
        {19, 20 ,21 ,22},
        {25, 26 ,27, 28 } }, 7);
    return 0;
}