#pragma once

#include "stdafx.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <unordered_map>

using namespace std;

class CreateSample
{
public:
	CreateSample(int n, int m) :product_num(n), customer_num(m) {}
	void classifyProduct(double gp_per_p, double bp_per_p);
	void classifyCustomer(double gc_per_c, double bc_per_c, double c_per_c);
	void createSample(bool deviation,
		double g_gg_per, double g_bb_per, double g_gc_per, double g_bc_per, double g_bg_per, double g_gb_per,
		double b_bg_per, double b_gb_per, double b_gg_per, double b_bb_per, double b_gc_per, double b_bc_per);
	void createSampleD(
		double g_gg_per, double g_bb_per, double g_gc_per, double g_bc_per, double g_bg_per, double g_gb_per,
		double b_bg_per, double b_gb_per, double b_gg_per, double b_bb_per, double b_gc_per, double b_bc_per);
	void dumpSample();

private:
	int product_num;
	int customer_num;

	int good_product_num;
	int bad_product_num;

	int good_customer_num;
	int bad_customer_num;
	int complainer_num;

	vector<int> good_product;
	vector<int> bad_product;

	vector<int> good_customer;
	vector<int> bad_customer;
	vector<int> complainer;

	vector<int> test_good;
	vector<int> test_bad;
};
