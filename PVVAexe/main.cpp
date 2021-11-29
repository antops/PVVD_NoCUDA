/**/
#pragma once
#include "../FFTPropagatorDLL/FFTPropagator.h"
#include "../PVVADll/PVVADll.h"
#include "../PVVADllAS/PVVADllAS.h"
#include "../Util/GraphTrans.h"
#include "../Util/FieldBase.h"
#include "../Util/FieldFile.h"
#include <fstream>

using namespace std;

enum CUDAMode
{
	FFTPro = 0,	//场传播计算
	PVVA = 1,			//PVVA镜子反射计算
	PVVAAS = 2,			//PVVA-AS镜子反射计算
};

enum FILEMode
{
	ASCII = 0,	//ASCII文件格式 txt
	BINARY = 1, //二进制文件格式，dat
};

int main() {
	cout << "this is an integrated Computation Core for Beam Propagation" << endl;
	string fieldinfile;
	string fieldoutfile;
	string mirrorfile;
	fstream in;
	in.open("./input.txt", ios::in);
	int RunMode;	int FileMode;
	if (!in.is_open()) { 
		cout << "can't open inputfile, Enter to exit." << endl; 
		getchar(); 
		return -1; 
	}
	in >> RunMode >> FileMode;	in.ignore(100, '\n');
	if (RunMode == FFTPro) {
		cout << "Run Beam Propagation Between Aerptures" << endl;
		in >> fieldinfile; in.ignore(100, '\n');
		in >> fieldoutfile; in.ignore(100, '\n');
		in.close();
		double freq;
		FieldBase fin;	FieldBase fout;
		FFTPropagator FFTrun;
		if (FileMode == ASCII) {
			freq = fromFile(fieldinfile,fin);
			fromFile(fieldoutfile, fout);
		}
		else if (FileMode == BINARY) {
			freq = fromFileBinary(fieldinfile, fin);
			fromFileBinary(fieldoutfile, fout);
		}
		cout << "field read done!" << endl;
		cout << "calculating freq: " << freq << "Hz, Size: " << fin.N_width << "x" << fout.M_depth << "Points" << endl;
		cout << fin.graphTransField.getTrans_x() << " "
			<< fin.graphTransField.getTrans_y() << " "
			<< fin.graphTransField.getTrans_z() << " "
			<< fin.graphTransField.getRotate_x() << " "
			<< fin.graphTransField.getRotate_y() << " "
			<< fin.graphTransField.getRotate_z() << " "
			<< fin.graphTransField.getRotate_theta() << endl;
		cout << fout.graphTransField.getTrans_x() << " "
			<< fout.graphTransField.getTrans_y() << " "
			<< fout.graphTransField.getTrans_z() << " "
			<< fout.graphTransField.getRotate_x() << " "
			<< fout.graphTransField.getRotate_y() << " "
			<< fout.graphTransField.getRotate_z() << " "
			<< fout.graphTransField.getRotate_theta() << endl;
		FFTrun.setInput(&fin);
		cout << "calculating" << endl;
		FFTrun.calculate(freq, fout.graphTransField, 0);
		cout << "calculated" << endl;
		FFTrun.getCalculatedFieldBase(fout);

		if (FileMode == ASCII) {
			toFile(fieldoutfile, fout, freq);
		}
		else if (FileMode == BINARY) {
			toFileBinary(fieldoutfile, fout, freq);
		}
		cout << "outputdone" << endl;	
	}
	else if (RunMode == PVVA) {
		PVVADll runPVVA;
		double freq;
		FieldBase fin; FieldBase fout;
		cout << "Run PVVA for mirror reflection" << endl;
		in >> fieldinfile; in.ignore(100, '\n');
		in >> fieldoutfile; in.ignore(100, '\n');
		int MIRRORMODE;
		in >> MIRRORMODE;	in.ignore(100, '\n');
		in >> mirrorfile;	in.ignore(100, '\n');
		in.close();
		if (FileMode == ASCII) {
			freq = fromFile(fieldinfile, fin);
			fromFile(fieldoutfile, fout);
		}
		else if (FileMode == BINARY) {
			freq = fromFileBinary(fieldinfile, fin);
			fromFileBinary(fieldoutfile, fout);
		}
		cout << "field read done!" << endl;
		cout << "calculating freq: " << freq << "Hz, Size: " << fin.N_width << "x" << fout.M_depth << "Points" << endl;

		if (MIRRORMODE == 0) {//STL
			std::cout << "MIRRORMODE == 0" << std::endl;
			runPVVA.setSTLModelFile(mirrorfile);
			runPVVA.setInField(&fin);
			runPVVA.setOutField(&fout);
			cout << "calculating" << endl;
			runPVVA.calculate(freq);
			cout << "calculated" << endl;
			fout = runPVVA.getFieldout();
		}
		else if (MIRRORMODE == 1) {//
			std::cout << "MIRRORMODE == 1" << std::endl;
			runPVVA.setAnalyticalModelFile(mirrorfile);
			runPVVA.setInField(&fin);
			runPVVA.setOutField(&fout);
			cout << "calculating" << endl;
			runPVVA.calculate(freq);
			cout << "calculated" << endl;
			fout = runPVVA.getFieldout();
		}
		if (FileMode == ASCII) {
			toFile(fieldoutfile, fout, freq);
		}
		else if (FileMode == BINARY) {
			toFileBinary(fieldoutfile, fout, freq);
		}
		cout << "outputdone" << endl;
	}
	else if (RunMode == PVVAAS) {
		PVVADllAS runPVVAAS;
		double freq;
		FieldBase fin; FieldBase fout;
		cout << " Run PVVAAS for mirror reflection" << endl;
		in >> fieldinfile; in.ignore(100, '\n');
		in >> fieldoutfile; in.ignore(100, '\n');
		int MIRRORMODE;
		in >> MIRRORMODE;	in.ignore(100, '\n');
		in >> mirrorfile;	in.ignore(100, '\n');
		in.close();
		if (FileMode == ASCII) {
			freq = fromFile(fieldinfile, fin);
			fromFile(fieldoutfile, fout);
		}
		else if (FileMode == BINARY) {
			freq = fromFileBinary(fieldinfile, fin);
			fromFileBinary(fieldoutfile, fout);
		}
		cout << "field read done!" << endl;
		cout << "calculating freq: " << freq << "Hz, Size: " << fin.N_width << "x" << fout.M_depth << "Points" << endl;

		if (MIRRORMODE == 0) {//STL
			runPVVAAS.setSTLModelFile(mirrorfile);
			runPVVAAS.setInField(&fin);
			runPVVAAS.setOutField(&fout);
			cout << "calculating" << endl;
			runPVVAAS.calculate(freq);
			cout << "calculated" << endl;
			fout = runPVVAAS.getFieldout();
		}
		else if (MIRRORMODE == 1) {//
			runPVVAAS.setAnalyticalModelFile(mirrorfile);
			runPVVAAS.setInField(&fin);
			runPVVAAS.setOutField(&fout);
			cout << "calculating" << endl;
			runPVVAAS.calculate(freq);
			cout << "calculated" << endl;
			fout = runPVVAAS.getFieldout();
		}
		if (FileMode == ASCII) {
			toFile(fieldoutfile, fout, freq);
		}
		else if (FileMode == BINARY) {
			toFileBinary(fieldoutfile, fout, freq);
		}
		cout << "outputdone" << endl;
	}
	//char op;
	//std::cout << "input any character to exit" << std::endl;
	//op = getchar();
	//op = getchar();
	return 0;
}