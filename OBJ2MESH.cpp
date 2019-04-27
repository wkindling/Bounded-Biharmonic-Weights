#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

vector<vector<double>> V;
vector<vector<int>> F;


int main()
{
	string filename = "C:/Users/dell/Desktop/singlerod.obj";
	string line;
	fstream f;
	f.open(filename, ios::in);

	while (!f.eof())
	{
		getline(f, line);
		if (line == "") continue;
		vector<string> stream;
		string end_mark = " ";
		string token = "";
		if ((line[0] == 'v'&& line[1] == ' ') || (line[0] == 'f'))
		{
			line = line.append(end_mark);
			int flag = 0;

			for (unsigned int i = 0; i < line.length(); i++)
			{
				char ch = line[i];
				if (ch != ' ')
				{
					token += ch;
					flag = 1;
				}
				else
				{
					if (flag) stream.push_back(token);
					flag = 0;
					token = "";
				}
			}
			if (stream[0] == "v")
			{
				vector<double> position;
				for (int i = 1; i <= 3; i++)
				{
					double num = atof(stream[i].c_str());
					position.push_back(num);
				}
				V.push_back(position);
			}
			else if (stream[0] == "f")
			{
				vector<int>  vertex_index;
				for (int i = 1; i <= 3; i++)
				{
					string stem = stream[i];
					string index = "";
					for (unsigned int j = 0; j < stem.length(); j++)
					{
						char ch = stem[j];
						if (ch != '/') index += ch;
						else break;
					}
					int index_num = atof(index.c_str());
					vertex_index.push_back(index_num);
				}
				F.push_back(vertex_index);
			}
		}
	}
	f.close();

	ofstream out("out.mesh");
	out << "MeshVersionFormatted 1" << endl;
	out << "Dimension 3" << endl;
	out << "Vertices" << endl;
	out << V.size() << endl;
	for (unsigned int i = 0; i < V.size(); i++)
	{
		out << V[i][0] << " " << V[i][1] << " " << V[i][2] << " 0" << endl;
	}
	out << "Triangles" << endl;
	out << F.size() << endl;

	for (unsigned int i = 0; i < F.size(); i++)
	{
		out << F[i][0] << " " << F[i][1] << " " << F[i][2] << " 0" << endl;
	}
	out << "Tetrahedra" << endl;
	out << "0" << endl;
	out << "Edges" << endl;
	out << "0" << endl;
	out << " End";

	out.close();
}