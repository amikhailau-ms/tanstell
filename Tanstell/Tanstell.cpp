// Tanstell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdio.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <math.h>

using namespace std;

struct element
{
	double probability;
	vector <element*> sons{};
	string code;
	element(float p, string c) {
		this->probability = p;
		this->code = c;
	}
	element(float p, string c, element* sons...) {
		this->probability = p;
		this->code = c;
		this->sons.push_back(sons);
	}
};


class ComparePairs
{
public:
	int operator()(const element* p1, const element* p2)
	{
		return p2->probability > p1->probability;
	}
};

struct DFSdata {
	double averageWordLength;
	double entropy;
	int codeResult;
	FILE* fout;
};

void dfs(element* node, DFSdata* data);
string convertToCode(int decimal);
string symbols[] = { "0","1","2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
int D = 0;
int L = 0;
int n = 0;

int main()
{
	FILE* fin = fopen("input.txt", "r");
	FILE* fout = fopen("output.txt", "w");
	fscanf(fin, "%d %d %d\n", &D, &n, &L);
	if (L > D) {
		printf("L > D, wrong input data.\n");
	}
	if (L < 2) {
		printf("L < 2, wrong input data.\n");
	}
	priority_queue <element*, vector<element*>, ComparePairs> queue;
	vector <double> probabilities;
	for (int i = 0; i < D; i++) {
		float probability = 0;
		fscanf(fin, "%f\n", &probability);
		probabilities.push_back(double(probability));
	}
	int times_to_split = floor((pow(D, n) - double(L)) / (double(L) - 1.0));
	element* root = new element(1.0, "");
	queue.push(root);
	for (;times_to_split >= 0; times_to_split--) {
		element* node0 = queue.top();
		queue.pop();
		for (int i = 0; i < L; i++) {
			element* newNode = new element(node0->probability * probabilities[i], node0->code + symbols[i]);
			node0->sons.push_back(newNode);
			queue.push(newNode);
		}
	}

	DFSdata data;
	data.fout = fout;
	data.averageWordLength = 0.0f;
	data.entropy = 0.0f;
	data.codeResult = 0;

	dfs(root, &data);

	fprintf(fout, "\nAverage word length: %.3f\nW: %.3f\nEntropy: %.3f\n", data.averageWordLength, double(n) / data.averageWordLength , data.entropy);
}

void dfs(element* node, DFSdata* data) {
	int sonsNumber = node->sons.size();
	if (sonsNumber == 0) {
		data->averageWordLength += node->code.length() * node->probability;
		data->entropy -= node->probability * log2f(node->probability);
		fprintf(data->fout, "Result code: %s | Source code: %s\n", convertToCode(data->codeResult).c_str(), node->code.c_str());
		data->codeResult++;
	}
	else {
		for (int i = 0; i < sonsNumber; i++) {
			dfs(node->sons[i], data);
		}
	}
}

string convertToCode(int decimal) {
	string result = "";
	for (int i = n - 1; i >= 0; i--) {
		int rest = decimal % int(pow(D, i));
		result += symbols[int((decimal - rest) / int(pow(D, i)))];
		decimal = rest;
	}
	return result;
}
