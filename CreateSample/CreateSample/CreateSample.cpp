// CreateSample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "CreateSample.hpp"
#include "Shuffle.hpp"

void CreateSample::classifyProduct(double gp_per_p, double bp_per_p)
{
	if (gp_per_p + bp_per_p > 1.0)
	{
		cout << "error: over 100%" << endl;
		return;
	}
	else if (gp_per_p == 0 || bp_per_p == 0)
	{
		cout << "error: under 1%" << endl;
		return;
	}
	
	vector<int> all_product(this->product_num);
	iota(all_product.begin(), all_product.end(), 0);

	shuffle(all_product.begin(), all_product.end(), mt19937());

	this->good_product_num = (int)(this->product_num * gp_per_p);
	vector<int> good_product(good_product_num);
	copy(all_product.begin(), all_product.begin() + good_product_num, good_product.begin());
	sort(good_product.begin(), good_product.end());
	this->good_product = good_product;

	this->bad_product_num = (int)(this->product_num * bp_per_p);
	vector<int> bad_product(bad_product_num);
	copy(all_product.begin() + good_product_num, all_product.begin() + good_product_num + bad_product_num, bad_product.begin());
	sort(bad_product.begin(), bad_product.end());
	this->bad_product = bad_product;
}

void CreateSample::classifyCustomer(double gc_per_c, double bc_per_c, double c_per_c)
{
	if (gc_per_c + bc_per_c + c_per_c > 1.0)
	{
		cout << "error: over 100%" << endl;
		return;
	}
	else if (gc_per_c <= 0 || bc_per_c <= 0 || c_per_c <= 0)
	{
		cout << "error: under 1%" << endl;
		return;
	}

	vector<int> all_customer(this->customer_num);
	iota(all_customer.begin(), all_customer.end(), 0);

	shuffle(all_customer.begin(), all_customer.end(), mt19937());

	this->good_customer_num = (int)(this->customer_num * gc_per_c);
	vector<int> good_customer(good_customer_num);
	copy(all_customer.begin(), all_customer.begin() + good_customer_num, good_customer.begin());
	sort(good_customer.begin(), good_customer.end());
	this->good_customer = good_customer;

	this->bad_customer_num = (int)(this->customer_num * bc_per_c);
	vector<int> bad_customer(bad_customer_num);
	copy(all_customer.begin() + good_customer_num, all_customer.begin() + good_customer_num + bad_customer_num, bad_customer.begin());
	sort(bad_customer.begin(), bad_customer.end());
	this->bad_customer = bad_customer;

	this->complainer_num = (int)(this->customer_num * c_per_c);
	vector<int> complainer(complainer_num);
	copy(all_customer.begin() + good_customer_num + bad_customer_num, all_customer.begin() + good_customer_num + bad_customer_num + complainer_num, complainer.begin());
	sort(complainer.begin(), complainer.end());
	this->complainer = complainer;
}

void CreateSample::createSample(bool deviation,
	double g_gg_per, double g_bb_per, double g_gc_per, double g_bc_per, double g_bg_per, double g_gb_per,
	double b_bg_per, double b_gb_per, double b_bc_per, double b_gc_per, double b_gg_per, double b_bb_per)
{
	if (false && deviation) {
		this->createSampleD(g_gg_per, g_bb_per, g_gc_per, g_bc_per, g_bg_per, g_gb_per, b_bg_per, b_gb_per, b_gg_per, b_bb_per, b_gc_per, b_bc_per);
		return;
	}

	random_device seed_gen;
	default_random_engine engine(seed_gen());
	vector<int>& shuffle_data = make_nonrepeat_rand_array_select_with_hash(0, 0, this->good_product_num);

	cout << "good_review" << endl;
	cout << "good_good" << endl;
	poisson_distribution<int> dist_good_good_good(g_gg_per);
	vector<int> ggg_reviews(this->good_customer_num);
	vector<pair<int, int>> good_good_good;
	for (vector<int>::iterator c = this->good_customer.begin(), num = ggg_reviews.begin(), e1 = this->good_customer.end(), e2 = ggg_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
		*num = dist_good_good_good(engine);
		if (*num < this->good_product_num)
		{
			shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
			for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
			{
				good_good_good.push_back(make_pair(*c, *i));
			}
		}
		else
		{
			for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
			{
				good_good_good.push_back(make_pair(*c, *i));
			}
		}
	}
	this->test_good.reserve(this->test_good.size() + good_good_good.size());
	copy(good_good_good.begin(), good_good_good.end(), back_inserter(this->test_good));

	cout << "bad_good" << endl;
	poisson_distribution<int> dist_good_bad_good(g_bg_per);
	vector<pair<int, int>> good_bad_good;
	for (vector<int>::iterator c = this->good_customer.begin(), num = ggg_reviews.begin(), e1 = this->good_customer.end(), e2 = ggg_reviews.end(); c != e1 || num != e2;)
	{
		int tmp = dist_good_bad_good(engine);
		if (tmp <= 0.2 * *num)
		{
			if (tmp < this->bad_product_num)
			{
				shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
				for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
				{
					good_good_good.push_back(make_pair(*c, *i));
				}
			}
			else
			{
				for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
				{
					good_good_good.push_back(make_pair(*c, *i));
				}
			}

			++c;
			++num;
		}
	}
	this->test_good.reserve(this->test_good.size() + good_bad_good.size());
	copy(good_bad_good.begin(), good_bad_good.end(), back_inserter(this->test_good));

	cout << "bad_bad" << endl;
	poisson_distribution<int> dist_good_bad_bad(g_bb_per);
	vector<int> gbb_reviews(this->bad_customer_num);
	vector<pair<int, int>> good_bad_bad;
	for (vector<int>::iterator c = this->bad_customer.begin(), num = gbb_reviews.begin(), e1 = this->bad_customer.end(), e2 = gbb_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
		*num = dist_good_bad_bad(engine);
		if (*num < this->bad_product_num)
		{
			shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
			for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
			{
				good_good_good.push_back(make_pair(*c, *i));
			}
		}
		else
		{
			for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
			{
				good_good_good.push_back(make_pair(*c, *i));
			}
		}
	}
	this->test_good.reserve(this->test_good.size() + good_bad_bad.size());
	copy(good_bad_bad.begin(), good_bad_bad.end(), back_inserter(this->test_good));

	cout << "good_bad" << endl;
	poisson_distribution<int> dist_good_good_bad(g_gb_per);
	vector<pair<int, int>> good_good_bad;
	for (vector<int>::iterator c = this->bad_customer.begin(), num = gbb_reviews.begin(), e1 = this->bad_customer.end(), e2 = gbb_reviews.end(); c != e1 || num != e2;)
	{
		int tmp = dist_good_good_bad(engine);
		if (tmp <= 0.2 * *num)
		{
			if (tmp < this->good_product_num)
			{
				shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
				for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
				{
					good_good_good.push_back(make_pair(*c, *i));
				}
			}
			else
			{
				for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
				{
					good_good_good.push_back(make_pair(*c, *i));
				}
			}

			++c;
			++num;
		}
	}
	this->test_good.reserve(this->test_good.size() + good_good_bad.size());
	copy(good_good_bad.begin(), good_good_bad.end(), back_inserter(this->test_good));

	cout << "bad_complainer" << endl;
	poisson_distribution<int> dist_good_bad_complainer(g_bc_per);
	vector<int> gbc_reviews(this->complainer_num);
	vector<pair<int, int>> good_bad_complainer;
	for (vector<int>::iterator c = this->complainer.begin(), num = gbc_reviews.begin(), e1 = this->complainer.end(), e2 = gbc_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
		*num = dist_good_bad_complainer(engine);
		if (*num < this->bad_product_num)
		{
			shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
			for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
			{
				good_good_good.push_back(make_pair(*c, *i));
			}
		}
		else
		{
			for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
			{
				good_good_good.push_back(make_pair(*c, *i));
			}
		}
	}
	this->test_good.reserve(this->test_good.size() + good_bad_complainer.size());
	copy(good_bad_complainer.begin(), good_bad_complainer.end(), back_inserter(this->test_good));

	cout << "good_complainer" << endl;
	poisson_distribution<int> dist_good_good_complainer(g_gc_per);
	vector<pair<int, int>> good_good_complainer;
	for (vector<int>::iterator c = this->complainer.begin(), num = gbc_reviews.begin(), e1 = this->complainer.end(), e2 = gbc_reviews.end(); c != e1 || num != e2;)
	{
		int tmp = dist_good_good_complainer(engine);
		if (0.6 * tmp <= 0.4 * *num)
		{
			if (tmp < this->good_product_num)
			{
				shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
				for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
				{
					good_good_good.push_back(make_pair(*c, *i));
				}
			}
			else
			{
				for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
				{
					good_good_good.push_back(make_pair(*c, *i));
				}
			}

			++c;
			++num;
		}
	}
	this->test_good.reserve(this->test_good.size() + good_good_complainer.size());
	copy(good_good_complainer.begin(), good_good_complainer.end(), back_inserter(this->test_good));


	cout << "bad_review" << endl;
	cout << "bad_good" << endl;
	poisson_distribution<int> dist_bad_bad_good(b_bg_per);
	vector<int> bbg_reviews(this->good_customer_num);
	vector<pair<int, int>> bad_bad_good;
	for (vector<int>::iterator c = this->good_customer.begin(), num = bbg_reviews.begin(), e1 = this->good_customer.end(), e2 = bbg_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
		*num = dist_bad_bad_good(engine);
		if (*num < this->bad_product_num)
		{
			shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
			for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
			{
				bad_bad_good.push_back(make_pair(*c, *i));
			}
		}
		else
		{
			for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
			{
				bad_bad_good.push_back(make_pair(*c, *i));
			}
		}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_bad_good.size());
	copy(bad_bad_good.begin(), bad_bad_good.end(), back_inserter(this->test_bad));

	cout << "good_good" << endl;
	poisson_distribution<int> dist_bad_good_good(b_gg_per);
	vector<pair<int, int>> bad_good_good;
	for (vector<int>::iterator c = this->good_customer.begin(), num = bbg_reviews.begin(), e1 = this->good_customer.end(), e2 = bbg_reviews.end(); c != e1 || num != e2;)
	{
		int tmp = dist_bad_good_good(engine);
		if (tmp <= 0.2 * *num)
		{
			if (tmp < this->good_product_num)
			{
				shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
				for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
				{
					bad_good_good.push_back(make_pair(*c, *i));
				}
			}
			else
			{
				for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
				{
					bad_good_good.push_back(make_pair(*c, *i));
				}
			}

			++c;
			++num;
		}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_good_good.size());
	copy(bad_good_good.begin(), bad_good_good.end(), back_inserter(this->test_bad));

	cout << "good_bad" << endl;
	poisson_distribution<int> dist_bad_good_bad(b_gb_per);
	vector<int> bgb_reviews(this->bad_customer_num);
	vector<pair<int, int>> bad_good_bad;
	for (vector<int>::iterator c = this->bad_customer.begin(), num = bgb_reviews.begin(), e1 = this->bad_customer.end(), e2 = bgb_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
		*num = dist_bad_good_bad(engine);
		if (*num < this->good_product_num)
		{
			shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
			for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
			{
				bad_good_bad.push_back(make_pair(*c, *i));
			}
		}
		else
		{
			for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
			{
				bad_good_bad.push_back(make_pair(*c, *i));
			}
		}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_good_bad.size());
	copy(bad_good_bad.begin(), bad_good_bad.end(), back_inserter(this->test_bad));

	cout << "bad_bad" << endl;
	poisson_distribution<int> dist_bad_bad_bad(b_bb_per);
	vector<pair<int, int>> bad_bad_bad;
	for (vector<int>::iterator c = this->bad_customer.begin(), num = bgb_reviews.begin(), e1 = this->bad_customer.end(), e2 = bgb_reviews.end(); c != e1 || num != e2;)
	{
		int tmp = dist_bad_bad_bad(engine);
		if (tmp <= 0.2 * *num)
		{
			if (tmp < this->bad_product_num)
			{
				shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
				for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
				{
					bad_bad_bad.push_back(make_pair(*c, *i));
				}
			}
			else
			{
				for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
				{
					bad_bad_bad.push_back(make_pair(*c, *i));
				}
			}

			++c;
			++num;
		}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_bad_bad.size());
	copy(bad_bad_bad.begin(), bad_bad_bad.end(), back_inserter(this->test_bad));

	cout << "good_complainer" << endl;
	poisson_distribution<int> dist_bad_good_complainer(b_gc_per);
	vector<int> bgc_reviews(this->complainer_num);
	vector<pair<int, int>> bad_good_complainer;
	for (vector<int>::iterator c = this->complainer.begin(), num = bgc_reviews.begin(), e1 = this->complainer.end(), e2 = bgc_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
		*num = dist_bad_good_complainer(engine);
		if (*num < this->good_product_num)
		{
			shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
			for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
			{
				bad_good_complainer.push_back(make_pair(*c, *i));
			}
		}
		else
		{
			for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
			{
				bad_good_complainer.push_back(make_pair(*c, *i));
			}
		}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_good_complainer.size());
	copy(bad_good_complainer.begin(), bad_good_complainer.end(), back_inserter(this->test_bad));

	cout << "bad_complainer" << endl;
	poisson_distribution<int> dist_bad_bad_complainer(b_bc_per);
	vector<pair<int, int>> bad_bad_complainer;
	for (vector<int>::iterator c = this->complainer.begin(), num = bgc_reviews.begin(), e1 = this->complainer.end(), e2 = bgc_reviews.end(); c != e1 || num != e2;)
	{
		int tmp = dist_bad_bad_complainer(engine);
		if (0.6 * tmp <= 0.4 * *num)
		{
			if (tmp < this->bad_product_num)
			{
				shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
				for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
				{
					bad_bad_complainer.push_back(make_pair(*c, *i));
				}
			}
			else
			{
				for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
				{
					bad_bad_complainer.push_back(make_pair(*c, *i));
				}
			}

			++c;
			++num;
		}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_bad_complainer.size());
	copy(bad_bad_complainer.begin(), bad_bad_complainer.end(), back_inserter(this->test_bad));
}

void CreateSample::createSampleD(
	double g_gg_per, double g_bb_per, double g_gc_per, double g_bc_per, double g_bg_per, double g_gb_per,
	double b_bg_per, double b_gb_per, double b_gg_per, double b_bb_per, double b_gc_per, double b_bc_per)
{
	random_device seed_gen;
	default_random_engine engine(seed_gen());
	vector<int>& shuffle_data = make_nonrepeat_rand_array_select_with_hash(0, 0, this->good_product_num);

	cout << "good_review" << endl;
	cout << "good_good" << endl;
	poisson_distribution<int> dist_good_good_good(g_gg_per);
	vector<int> ggg_reviews(this->good_customer_num);
	vector<pair<int, int>> good_good_good;
	for (vector<int>::iterator c = this->good_customer.begin(), num = ggg_reviews.begin(), e1 = this->good_customer.end(), e2 = ggg_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
		*num = dist_good_good_good(engine);
		if (*num < this->good_product_num)
		{
			shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
			for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
			{
				good_good_good.push_back(make_pair(*c, *i));
			}
		}
		else
		{
			for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
			{
				good_good_good.push_back(make_pair(*c, *i));
			}
		}
	}
	this->test_good.reserve(this->test_good.size() + good_good_good.size());
	copy(good_good_good.begin(), good_good_good.end(), back_inserter(this->test_good));

	cout << "bad_good" << endl;
	poisson_distribution<int> dist_good_bad_good(g_bg_per);
	vector<pair<int, int>> good_bad_good;
	for (vector<int>::iterator c = this->good_customer.begin(), num = ggg_reviews.begin(), e1 = this->good_customer.end(), e2 = ggg_reviews.end(); c != e1 || num != e2;)
	{
	int tmp = dist_good_bad_good(engine);
	if (tmp <= 0.2 * *num)
	{
	if (tmp < this->bad_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}

	++c;
	++num;
	}
	}
	this->test_good.reserve(this->test_good.size() + good_bad_good.size());
	copy(good_bad_good.begin(), good_bad_good.end(), back_inserter(this->test_good));

	cout << "bad_bad" << endl;
	poisson_distribution<int> dist_good_bad_bad(g_bb_per);
	vector<int> gbb_reviews(this->bad_customer_num);
	vector<pair<int, int>> good_bad_bad;
	for (vector<int>::iterator c = this->bad_customer.begin(), num = gbb_reviews.begin(), e1 = this->bad_customer.end(), e2 = gbb_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
	*num = dist_good_bad_bad(engine);
	if (*num < this->bad_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	}
	this->test_good.reserve(this->test_good.size() + good_bad_bad.size());
	copy(good_bad_bad.begin(), good_bad_bad.end(), back_inserter(this->test_good));

	cout << "good_bad" << endl;
	poisson_distribution<int> dist_good_good_bad(g_gb_per);
	vector<pair<int, int>> good_good_bad;
	for (vector<int>::iterator c = this->bad_customer.begin(), num = gbb_reviews.begin(), e1 = this->bad_customer.end(), e2 = gbb_reviews.end(); c != e1 || num != e2;)
	{
	int tmp = dist_good_good_bad(engine);
	if (tmp <= 0.2 * *num)
	{
	if (tmp < this->good_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}

	++c;
	++num;
	}
	}
	this->test_good.reserve(this->test_good.size() + good_good_bad.size());
	copy(good_good_bad.begin(), good_good_bad.end(), back_inserter(this->test_good));

	cout << "bad_complainer" << endl;
	poisson_distribution<int> dist_good_bad_complainer(g_bc_per);
	vector<int> gbc_reviews(this->complainer_num);
	vector<pair<int, int>> good_bad_complainer;
	for (vector<int>::iterator c = this->complainer.begin(), num = gbc_reviews.begin(), e1 = this->complainer.end(), e2 = gbc_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
	*num = dist_good_bad_complainer(engine);
	if (*num < this->bad_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	}
	this->test_good.reserve(this->test_good.size() + good_bad_complainer.size());
	copy(good_bad_complainer.begin(), good_bad_complainer.end(), back_inserter(this->test_good));

	cout << "good_complainer" << endl;
	poisson_distribution<int> dist_good_good_complainer(g_gc_per);
	vector<pair<int, int>> good_good_complainer;
	for (vector<int>::iterator c = this->complainer.begin(), num = gbc_reviews.begin(), e1 = this->complainer.end(), e2 = gbc_reviews.end(); c != e1 || num != e2;)
	{
	int tmp = dist_good_good_complainer(engine);
	if (0.6 * tmp <= 0.4 * *num)
	{
	if (tmp < this->good_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}

	++c;
	++num;
	}
	}
	this->test_good.reserve(this->test_good.size() + good_good_complainer.size());
	copy(good_good_complainer.begin(), good_good_complainer.end(), back_inserter(this->test_good));


	cout << "bad_review" << endl;
	cout << "bad_good" << endl;
	poisson_distribution<int> dist_bad_bad_good(b_bg_per);
	vector<int> bbg_reviews(this->good_customer_num);
	vector<pair<int, int>> bad_bad_good;
	for (vector<int>::iterator c = this->good_customer.begin(), num = bbg_reviews.begin(), e1 = this->good_customer.end(), e2 = bbg_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
	*num = dist_bad_bad_good(engine);
	if (*num < this->bad_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_bad_good.size());
	copy(bad_bad_good.begin(), bad_bad_good.end(), back_inserter(this->test_bad));

	cout << "good_good" << endl;
	poisson_distribution<int> dist_bad_good_good(b_gg_per);
	vector<pair<int, int>> bad_good_good;
	for (vector<int>::iterator c = this->good_customer.begin(), num = bbg_reviews.begin(), e1 = this->good_customer.end(), e2 = bbg_reviews.end(); c != e1 || num != e2;)
	{
	int tmp = dist_bad_good_good(engine);
	if (tmp <= 0.2 * *num)
	{
	if (tmp < this->good_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}

	++c;
	++num;
	}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_good_good.size());
	copy(bad_good_good.begin(), bad_good_good.end(), back_inserter(this->test_bad));

	cout << "good_bad" << endl;
	poisson_distribution<int> dist_bad_good_bad(b_gb_per);
	vector<int> bgb_reviews(this->bad_customer_num);
	vector<pair<int, int>> bad_good_bad;
	for (vector<int>::iterator c = this->bad_customer.begin(), num = bgb_reviews.begin(), e1 = this->bad_customer.end(), e2 = bgb_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
	*num = dist_bad_good_bad(engine);
	if (*num < this->good_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_good_bad.size());
	copy(bad_good_bad.begin(), bad_good_bad.end(), back_inserter(this->test_bad));

	cout << "bad_bad" << endl;
	poisson_distribution<int> dist_bad_bad_bad(b_bb_per);
	vector<pair<int, int>> bad_bad_bad;
	for (vector<int>::iterator c = this->bad_customer.begin(), num = bgb_reviews.begin(), e1 = this->bad_customer.end(), e2 = bgb_reviews.end(); c != e1 || num != e2;)
	{
	int tmp = dist_bad_bad_bad(engine);
	if (tmp <= 0.2 * *num)
	{
	if (tmp < this->bad_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}

	++c;
	++num;
	}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_bad_bad.size());
	copy(bad_bad_bad.begin(), bad_bad_bad.end(), back_inserter(this->test_bad));

	cout << "good_complainer" << endl;
	poisson_distribution<int> dist_bad_good_complainer(b_gc_per);
	vector<int> bgc_reviews(this->complainer_num);
	vector<pair<int, int>> bad_good_complainer;
	for (vector<int>::iterator c = this->complainer.begin(), num = bgc_reviews.begin(), e1 = this->complainer.end(), e2 = bgc_reviews.end(); c != e1 || num != e2; ++c, ++num)
	{
	*num = dist_bad_good_complainer(engine);
	if (*num < this->good_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->good_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->good_product.begin(), e = this->good_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_good_complainer.size());
	copy(bad_good_complainer.begin(), bad_good_complainer.end(), back_inserter(this->test_bad));

	cout << "bad_complainer" << endl;
	poisson_distribution<int> dist_bad_bad_complainer(b_bc_per);
	vector<pair<int, int>> bad_bad_complainer;
	for (vector<int>::iterator c = this->complainer.begin(), num = bgc_reviews.begin(), e1 = this->complainer.end(), e2 = bgc_reviews.end(); c != e1 || num != e2;)
	{
	int tmp = dist_bad_bad_complainer(engine);
	if (0.6 * tmp <= 0.4 * *num)
	{
	if (tmp < this->bad_product_num)
	{
	shuffle_data = make_nonrepeat_rand_array_select_with_hash(*num, 0, this->bad_product_num);
	for (vector<int>::iterator i = shuffle_data.begin(), e = shuffle_data.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}
	else
	{
	for (vector<int>::iterator i = this->bad_product.begin(), e = this->bad_product.end(); i != e; ++i)
	{
	good_good_good.push_back(make_pair(*c, *i));
	}
	}

	++c;
	++num;
	}
	}
	this->test_bad.reserve(this->test_bad.size() + bad_bad_complainer.size());
	copy(bad_bad_complainer.begin(), bad_bad_complainer.end(), back_inserter(this->test_bad));
}

void CreateSample::dumpSample()
{
	ofstream product_data("product_data.txt", ios::out);
	copy(this->good_product.begin(), this->good_product.end(), ostream_iterator<int>(product_data, ","));
	product_data << endl;
	copy(this->bad_product.begin(), this->bad_product.end(), ostream_iterator<int>(product_data, ","));
	product_data << endl;

	ofstream customer_data("customer_data.txt", ios::out);
	copy(this->good_customer.begin(), this->good_customer.end(), ostream_iterator<int>(customer_data, ","));
	customer_data << endl;
	copy(this->bad_customer.begin(), this->bad_customer.end(), ostream_iterator<int>(customer_data, ","));
	customer_data << endl;
	copy(this->complainer.begin(), this->complainer.end(), ostream_iterator<int>(customer_data, ","));
	customer_data << endl;
	
	ofstream test_good("test_good.txt", ios::out);
	for (vector<pair<int, int>>::iterator i = this->test_good.begin(), e = this->test_good.end(); i != e; ++i)
	{
		test_good << (*i).first << " " << (*i).second << endl;
	}
	test_good << endl;
	
	ofstream test_bad("test_bad.txt", ios::out);
	for (vector<pair<int, int>>::iterator i = this->test_bad.begin(), e = this->test_bad.end(); i != e; ++i)
	{
		test_bad << (*i).first << " " << (*i).second << endl;
	}
	test_bad << endl;

	ofstream train("train.txt", ios::out);
	copy(this->bad_product.begin(), this->bad_product.end(), ostream_iterator<int>(train, ","));
	train << endl;
	copy(this->bad_customer.begin(), this->bad_customer.end(), ostream_iterator<int>(train, ","));
	copy(this->complainer.begin(), this->complainer.end(), ostream_iterator<int>(train, ","));
	train << endl;
}
