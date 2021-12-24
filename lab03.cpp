#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>


struct Solution
{
	double F;
	std::vector<double> vars = { 0, 0, 0 };
};


void brutforce()
{
	for (int x1 = 0; x1 < 10; ++x1)
	{
		for (int x2 = 0; x2 < 10; ++x2)
		{
			for (int x3 = 0; x3 < 10; ++x3)
			{
				if (x1 + x2 + x3 <= 7 && x1 + x2 <= 2 && 0.5 *
					x2 + 2 * x3 <= 4)
					std::cout << x1 << " " << x2 << " " << x3 << " " << 1 *
					x1 + 3 * x2 + 8 * x3 << std::endl;
			}
		}
	}
}
void simplex(double** s_matrix, int flag, unsigned int
	matrix_length, std::vector<Solution>& sols) {
	char variable = 'x';
	const unsigned short int basis_size = 3;
	const unsigned short int matrix_width = 4;
	double** ad_matrix = new double* [matrix_length];
	for (int i = 0; i < matrix_length; ++i)
		ad_matrix[i] = new double[matrix_width];
	double** matrix_left = new double* [matrix_length + 1];
	for (int i = 0; i < matrix_length + 1; ++i)
		matrix_left[i] = new double[matrix_width];
	double** matrix_right = new double* [matrix_length + 1];
	for (int i = 0; i < matrix_length + 1; ++i)
		matrix_right[i] = new double[matrix_width];
	for (int i = 0; i < matrix_length; ++i)
	{
		for (int j = 0; j < matrix_width; ++j)
		{
			matrix_left[i][j] = s_matrix[i][j];
			matrix_right[i][j] = s_matrix[i][j];
		}
	}
	int basis[basis_size] = { 1 , 2, 3 };
	int* free = new int[matrix_length - 1];
	for (int i = 0; i < matrix_length - 1; ++i)
		free[i] = i + 4;
	double min = -1;
	std::cout << std::fixed;
	std::cout << " s0 ";
	for (int i = 0; i < basis_size; i++)
	{
		std::cout << variable << basis[i] << " ";
	}
	std::cout << "\n";
	for (int i = 0; i < matrix_length; ++i)
	{
		if (i < matrix_length - 1) std::cout << variable << free[i] << " ";
		else std::cout << "F ";
		for (int j = 0; j < matrix_width; ++j)
		{
			std::cout << std::setw(8) << std::setprecision(3) <<
				s_matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	bool f = true;
	int perm_col, perm_str;
	bool exist = true;
	while (f && exist)
	{
		perm_str = -1;
		perm_col = -1;
		for (int i = 0; i < matrix_length - 1; ++i)
		{
			if (s_matrix[i][0] < 0)
			{
				perm_str = i;
				break;
			}
		}
		if (perm_str >= 0)
		{
			exist = false;
			for (int i = 1; i < matrix_width; ++i)
			{
				if (s_matrix[perm_str][i] < 0)
				{
					perm_col = i;
					exist = true;
					break;
				}
			}
				if (exist) {
					for (int i = 0; i < matrix_length - 1; ++i)
					{
						if (((s_matrix[i][0] / s_matrix[i][perm_col]) >= 0) &&
							((min < 0) || (s_matrix[i][0] / s_matrix[i][perm_col] < min)))
						{
							min = s_matrix[i][0] / s_matrix[i][perm_col];
							perm_str = i;
						}
					}
					for (int i = 0; i < matrix_length; i++)
					{
						for (int j = 0; j < matrix_width; j++)
						{
							if (i != perm_str && j != perm_col)
								ad_matrix[i][j] = s_matrix[i][j] -
								s_matrix[i][perm_col] * s_matrix[perm_str][j] /
								s_matrix[perm_str][perm_col];
							else if (i != perm_str)
								ad_matrix[i][j] = -1 * s_matrix[i][j] /
								s_matrix[perm_str][perm_col];
							else if (j != perm_col)
								ad_matrix[i][j] = s_matrix[i][j] /
								s_matrix[perm_str][perm_col];
							else
								ad_matrix[i][j] = 1 / s_matrix[i][j];
							if (abs(ad_matrix[i][j]) <= pow(10, -12))
								ad_matrix[i][j] = abs(ad_matrix[i][j]);
						}
					}
					for (int i = 0; i < matrix_length; ++i)
						for (int j = 0; j < matrix_width; ++j)
							s_matrix[i][j] = ad_matrix[i][j];
					std::swap(basis[perm_col - 1], free[perm_str]);
					std::cout << " s0 ";
					for (int i = 0; i < basis_size; i++)
					{
							std::cout << variable << basis[i] << " ";
					}
					std::cout << "\n";
					for (int i = 0; i < matrix_length; ++i)
					{
						if (i < matrix_length - 1) std::cout << variable << free[i]
							<< " ";
						else std::cout << "F ";
						for (int j = 0; j < matrix_width; ++j)
						{
							std::cout << std::setw(8) << std::setprecision(3) <<
								s_matrix[i][j] << " ";
						}
						std::cout << std::endl;
					}
					std::cout << std::endl;
				}
		}
		else f = false;
	}
	if (exist) f = true;
	else std::cout << "There is no solution\n";
	bool unlim = false;
	while (f && !unlim) {
		perm_col = -1;
		min = -1;
		for (int i = 1; i < matrix_width; ++i)
		{
			if (s_matrix[matrix_length - 1][i] > 0)
			{
				perm_col = i;
				break;
			}
		}
		if (perm_col > 0)
		{
				unlim = true;
			for (int i = 0; i < matrix_length - 1; ++i)
				if (s_matrix[i][perm_col] > 0)
				{
					unlim = false;
					break;
				}
			if (!unlim)
			{
				for (int i = 0; i < matrix_length - 1; ++i)
				{
					if (((s_matrix[i][0] / s_matrix[i][perm_col]) >= 0) &&
						((min < 0) || (s_matrix[i][0] / s_matrix[i][perm_col] < min)))
					{
						min = s_matrix[i][0] / s_matrix[i][perm_col];
						perm_str = i;
					}
				}
				for (int i = 0; i < matrix_length; i++)
				{
					for (int j = 0; j < matrix_width; j++)
					{
						if (i != perm_str && j != perm_col)
							ad_matrix[i][j] = s_matrix[i][j] -
							s_matrix[i][perm_col] * s_matrix[perm_str][j] /
							s_matrix[perm_str][perm_col];
						else if (i != perm_str)
							ad_matrix[i][j] = -1 * s_matrix[i][j] /
							s_matrix[perm_str][perm_col];
						else if (j != perm_col)
							ad_matrix[i][j] = s_matrix[i][j] /
							s_matrix[perm_str][perm_col];
						else
							ad_matrix[i][j] = 1 / s_matrix[i][j];
						if (abs(ad_matrix[i][j]) <= pow(10, -12))
							ad_matrix[i][j] = abs(ad_matrix[i][j]);
					}
				}
				for (int i = 0; i < matrix_length; ++i)
					for (int j = 0; j < matrix_width; ++j)
						s_matrix[i][j] = ad_matrix[i][j];
				std::swap(basis[perm_col - 1], free[perm_str]);
				std::cout << " s0 ";
				for (int i = 0; i < basis_size; i++)
				{
					std::cout << variable << basis[i] << " ";
				}
				std::cout << "\n";
				for (int i = 0; i < matrix_length; ++i)
				{
					if (i < matrix_length - 1) std::cout << variable << free[i]
						<< " ";
					else std::cout << "F ";
					for (int j = 0; j < matrix_width; ++j)
					{
						std::cout << std::setw(8) << std::setprecision(3) <<
							s_matrix[i][j] << " ";
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
		else f = false;
	}
	std::cout << std::endl;
	if (unlim)
	{
		std::cout << "Function is unlimitted\n";
	}
	else
	{
		Solution temp;
		std::cout << "\nSolution:\n";
			std::cout << "F = " << s_matrix[matrix_length - 1][0] * -1 <<
			"\n";
		temp.F = s_matrix[matrix_length - 1][0] * -1;
		for (int i = 0; i < basis_size; ++i)
		{
			if (basis[i] <= basis_size)
				std::cout << variable << basis[i] << " = 0\n";
		}
		for (int i = 0; i < matrix_length - 1; ++i)
		{
			if (free[i] <= basis_size)
			{
				std::cout << variable << free[i] << " = " << s_matrix[i][0]
					<< "\n";
				temp.vars[free[i] - 1] = s_matrix[i][0];
			}
		}
		bool branch = false;
		int branch_var = 0;
		for (int i = 0; i < basis_size; ++i) {
			if (temp.vars[i] != int(temp.vars[i]))
			{
				branch_var = i + 1;
				break;
			}
		}
		if (branch_var > 0) branch = true;
		if (branch) {

			for (int i = 0; i < matrix_width; ++i)
			{
				matrix_left[matrix_length][i] = matrix_left[matrix_length- 1][i];
				matrix_left[matrix_length - 1][i] = 0;
			}
			matrix_left[matrix_length - 1][0] =
			floor(temp.vars[branch_var - 1]);
			matrix_left[matrix_length - 1][branch_var] = 1;
			std::cout << "------------------branch----------------------\n\n";
			simplex(matrix_left, -1, matrix_length + 1, sols);
			std::cout << "----------------------------------------------\n\n";
			for (int i = 0; i < matrix_width; ++i)
			{
				matrix_right[matrix_length][i] =
					matrix_right[matrix_length - 1][i];
				matrix_right[matrix_length - 1][i] = 0;
			}
			matrix_right[matrix_length - 1][0] = -1 *
				ceil(temp.vars[branch_var - 1]);
			matrix_right[matrix_length - 1][branch_var] = -1;
			std::cout << "------------------branch----------------------\n\n";
			simplex(matrix_right, -1, matrix_length + 1, sols);
			std::cout << "----------------------------------------------\n\n";
		}
		else
		{
			sols.push_back(temp);
		}
	}
	for (int i = 0; i < matrix_length; ++i)
		delete[] ad_matrix[i];
	delete[] ad_matrix;
	for (int i = 0; i < matrix_length + 1; ++i)
		delete[] matrix_left[i];
	delete[] matrix_left;
	for (int i = 0; i < matrix_length + 1; ++i)
		delete[] matrix_right[i];
	delete[] matrix_right;
}

void bnb_method() {
	const unsigned short int basis_size = 3;
	const unsigned short int matrix_length = 4;
	const unsigned short int matrix_width = 4;
	double** s_matrix = new double* [matrix_length];
	for (int i = 0; i < matrix_length; ++i)
		s_matrix[i] = new double[matrix_width];
	std::ifstream in("D:\\GitHub\\MO\\lab03\\lptask3.txt");
	for (int i = 0; i < matrix_width - 1; ++i)
		in >> s_matrix[matrix_length - 1][i + 1];
	for (int i = 0; i < matrix_length - 1; ++i)
		for (int j = 0; j < matrix_width - 1; ++j)
		{
			in >> s_matrix[i][j + 1];
		}
	for (int i = 0; i < matrix_length - 1; ++i)
		in >> s_matrix[i][0];
	s_matrix[matrix_length - 1][0] = 0;
	in.close();
	std::cout << std::endl;

	std::vector<Solution> good_sols;
	std::cout << "--------------------Direct-task---------------\n\n";
	simplex(s_matrix, -1, matrix_length, good_sols);
	std::cout << "-----------------------End--------------------\n\n";
	double max = 9223372036854775806 * (-1);
	int max_index = 0;
	for (int i = 0; i < good_sols.size(); ++i)
	{
		if (good_sols[i].F > max)
		{
			max = good_sols[i].F;
			max_index = i;
		}
	}
	std::cout << "F = " << good_sols[max_index].F << "\n";
	for (int i = 0; i < good_sols[max_index].vars.size(); ++i)
	{
		std::cout << "x" << i + 1 << " = " <<
			good_sols[max_index].vars[i] << "\n";
	}
	for (int i = 0; i < matrix_length; ++i)
		delete[] s_matrix[i];
	delete[] s_matrix;
}

int main()
{
	brutforce();
	bnb_method();
	return 0;
}