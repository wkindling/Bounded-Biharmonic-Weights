#pragma once
#ifndef WRITEFILE_HPP
#define WRITEFILE_HPP

#include <fstream>
#include <Eigen/Dense>

void write_file(std::ofstream& out, Eigen::MatrixXd& V, Eigen::MatrixXi& F, Eigen::MatrixXd& C, Eigen::MatrixXi& E, Eigen::MatrixXd& W)
{
	using namespace std;

	out << V.rows() << " " << V.cols() << endl;
	for (int i = 0; i < V.rows(); i++)
	{
		for (int j = 0; j < V.cols(); j++)
		{
			out << V(i, j) << " ";
		}
		out << endl;
	}
	out << F.rows() << " " << F.cols() << endl;
	for (int i = 0; i < F.rows(); i++)
	{
		for (int j = 0; j < F.cols(); j++)
		{
			out << F(i, j) << " ";
		}
		out << endl;
	}
	cout << C.rows() << " " << C.cols() << endl;
	for (int i = 0; i < C.rows(); i++)
	{
		for (int j = 0; j < C.cols(); j++)
		{
			out << C(i, j) << " ";
		}
		out << endl;
	}
	cout << E.rows() << " " << E.cols() << endl;
	for (int i = 0; i < E.rows(); i++)
	{
		for (int j = 0; j < E.cols(); j++)
		{
			out << E(i, j) << " ";
		}
		out << endl;
	}
	out << W.rows() << " " << W.cols() << endl;
	for (int i = 0; i < W.rows(); i++)
	{
		for (int j = 0; j < W.cols(); j++)
		{
			out << W(i, j) << " ";
		}
		out << endl;
	}
	out.close();
}

#endif