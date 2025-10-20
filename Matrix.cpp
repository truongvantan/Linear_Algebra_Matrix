#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>

using Matrix = std::vector<std::vector<double>>;

void printMatrix(const Matrix& matrix)
{
	size_t rows = matrix.size();
	size_t cols = matrix[0].size();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << std::setw(10) << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

bool isSquareMatrix(const Matrix& matrix)
{
	size_t rows = matrix.size();
	size_t cols = matrix[0].size();

	return rows == cols;
}

Matrix findSubMatrix(const Matrix& inputMatrix, int idxRow, int idxCol)
{
	size_t rows = inputMatrix.size();
	size_t cols = inputMatrix[0].size();
	Matrix result(rows - 1, std::vector<double>(rows - 1));
	int i = 0, j = 0;

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (row != idxRow && col != idxCol)
			{
				result[i][j] = inputMatrix[row][col];
				j++;

				if (j == rows - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}


	return result;
}

// Khai triển định thức theo cột đầu tiên
double findDeterminant(const Matrix& inputMatrix, size_t sizeMatrix)
{
	double determinant = 0.0;

	if (isSquareMatrix(inputMatrix))
	{
		double sign = 1.0;
		Matrix subMatrix;
		int idxCol = 0;

		if (sizeMatrix == 1)
		{
			return inputMatrix[0][0];
		}

		if (sizeMatrix == 2)
		{
			return inputMatrix[0][0] * inputMatrix[1][1] - inputMatrix[1][0] * inputMatrix[0][1];
		}

		for (int row = 0; row < sizeMatrix; row++)
		{
			subMatrix = findSubMatrix(inputMatrix, row, idxCol);
			determinant += sign * inputMatrix[row][idxCol] * findDeterminant(subMatrix, sizeMatrix - 1);
			sign = -sign;
		}
	}
	else
	{
		throw std::runtime_error("Matrix is not square!!!");
	}

	return determinant;
}

// Tìm ma trận phụ hợp
Matrix findAdjointMatrix(const Matrix& inputMatrix)
{
	size_t sizeMatrix = inputMatrix.size();
	Matrix adjointMatrix(sizeMatrix, std::vector<double>(sizeMatrix));

	if (sizeMatrix == 1)
	{
		adjointMatrix[0][0] = 1;
		return adjointMatrix;
	}

	double sign = 1.0;
	Matrix subMatrix;

	for (int i = 0; i < sizeMatrix; i++)
	{
		for (int j = 0; j < sizeMatrix; j++)
		{
			subMatrix = findSubMatrix(inputMatrix, i, j);
			sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
			adjointMatrix[j][i] = sign * findDeterminant(subMatrix, sizeMatrix - 1);
		}
	}

	return adjointMatrix;
}

// Tìm ma trận nghịch đảo
Matrix findInverseMatrix(const Matrix& inputMatrix)
{
	size_t sizeMatrix = inputMatrix.size();
	Matrix inverseMatrix(sizeMatrix, std::vector<double>(sizeMatrix));

	double determinant = findDeterminant(inputMatrix, sizeMatrix);

	if (determinant == 0.0 || determinant <= 1e-9)
	{
		throw std::runtime_error("Singluar Matrix. Cannot find inverse!!!");
	}

	Matrix adjointMatrix = findAdjointMatrix(inputMatrix);

	for (int i = 0; i < sizeMatrix; i++)
	{
		for (int j = 0; j < sizeMatrix; j++)
		{
			inverseMatrix[i][j] = adjointMatrix[i][j] / determinant;
		}
	}


	return inverseMatrix;
}

int main()
{
	Matrix matrix = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{4, 1, 6, 3},
		{2, 7, 1, 9}
	};

	Matrix matrix2 = {
		{1, 2},
		{3, 4}
	};

	Matrix matrix3 = {
		{4, 5, 6, 7, 8},
		{9, 1, 5, 7, 3},
		{2, 0, 6, 7, 9},
		{1, 2, 8, 6, 1},
		{1, 8, 0, 5, 6}
	};

	Matrix matrix4 = {
		{1, 2, 3},
		{-2, 4, 0},
		{4, -5, 7}
	};

	Matrix matrix5 = {
		{5, -2, 2, 7},
		{1, 0, 0, 3},
		{-3, 1, 5, 0},
		{3, -1, -9, 4}
	};

	std::cout << "adjoint matrix: " << std::endl;
	printMatrix(findAdjointMatrix(matrix5));

	std::cout << "inverse matrix: " << std::endl;
	printMatrix(findInverseMatrix(matrix5));

	std::cin.get();

	return 0;
}