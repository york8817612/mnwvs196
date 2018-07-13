#pragma once
#include "exprtk.hpp"
#include <vector>
#include <string>


class Evaluator
{
	exprtk::parser<double> m_parser;
	exprtk::expression<double> m_expression;
	exprtk::symbol_table<double> m_symbol_table;

public:
	Evaluator(double& ref, const std::string& evalStr);
	~Evaluator();

	//���U�ɪ��ܼƧ��ܤF�A���o�s��
	double Eval();

	//��ӭ���
	static double Eval(const std::string& evalStr, const std::string& variableName, double& var);
};

