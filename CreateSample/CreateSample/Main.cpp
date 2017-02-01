#include "stdafx.h"
#include "CreateSample.hpp"

template <typename List>
void split(const std::string& s, const std::string& delim, List& result)
{
	result.clear();

	using string = std::string;
	string::size_type pos = 0;

	while (pos != string::npos)
	{
		string::size_type p = s.find(delim, pos);

		if (p == string::npos)
		{
			result.push_back(s.substr(pos));
			break;
		}
		else {
			result.push_back(s.substr(pos, p - pos));
		}

		pos = p + delim.size();
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cerr << "ˆø”‚ª‘«‚è‚È‚¢‚æ" << endl;
		return EXIT_FAILURE;
	}

	string file_name = argv[1];
	ifstream ifs(file_name);
	double arg[19] = { 0 };

	vector<string> str_arg;
	string tmp;
	ifs >> tmp;
	split(tmp, ",", str_arg);
	for (int i = 0; i < 2; ++i)
	{
		arg[i] = stod(str_arg[i]);
	}
	ifs >> tmp;
	split(tmp, ",", str_arg);
	for (int i = 0; i < 2; ++i)
	{
		arg[i + 2] = stod(str_arg[i]);
	}
	ifs >> tmp;
	split(tmp, ",", str_arg);
	for (int i = 0; i < 3; ++i)
	{
		arg[i + 4] = stod(str_arg[i]);
	}
	ifs >> tmp;
	bool deviation = tmp == "true" ? true : false;
	ifs >> tmp;
	split(tmp, ",", str_arg);
	for (int i = 0; i < 6; ++i)
	{
		arg[i + 7] = 1 / stod(str_arg[i]);
	}
	ifs >> tmp;
	split(tmp, ",", str_arg);
	for (int i = 0; i < 6; ++i)
	{
		arg[i + 13] = 1 / stod(str_arg[i]);
	}

	CreateSample *cs = new CreateSample(arg[0], arg[1]);
	cs->classifyProduct(arg[2], arg[3]);
	cs->classifyCustomer(arg[4], arg[5], arg[6]);
	cs->createSample(deviation,
		arg[7], arg[8], arg[9], arg[10], arg[11], arg[12],
		arg[13], arg[14], arg[15], arg[16], arg[17], arg[18]);
	cs->dumpSample(argv[2]);

	return EXIT_SUCCESS;
}
