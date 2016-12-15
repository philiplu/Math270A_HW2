#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
 
vector<double> load_phi_i(ifstream& file, int N) {
	vector<double> phi;
	for (int it = 0; it < N; ++it) {
		double value;
		file >> value;
		phi.push_back(value);
	}
	return phi;
}

vector<double> derivatives_of_phi(vector<double> phi, double dX) {
	int N = phi.size();
	vector<double> dev(N,0.0);
	for (int i = 0; i < N - 1; ++i){
		 double dev_i = (phi[i + 1] - phi[i]) / (2*dX);
		 dev[i] += dev_i; 
		 dev[i + 1] += dev_i;
	}
	dev[0] = 2 * dev[0];
	dev[N - 1] = 2 * dev[N-1];
	return dev;
}

int main(){
	int N = 20;
	double e = 0.05; 
	int steps = 481;
	double dX = .05;
	
	string filename_prefix = "output//matlab//particle_x_";
	string filename_suffix = ".dat";
	string outfile_prefix = "output//matlab//particle_x_";
	string outfile_suffix = ".obj";
	for (int i = 0; i < steps; ++i) {
		
		vector<double> phi_i, phiy_i, phiz_i;
		vector<double> phi_der;
		double a; 
		
		string number = to_string(i);
		string file = filename_prefix+number+filename_suffix;
		ifstream particleFile(file);
		phi_i = load_phi_i(particleFile, N);
		string outfilepath = outfile_prefix + number + outfile_suffix;
		ofstream outfile(outfilepath); 
		outfile << "g" << endl; 
		phi_der = derivatives_of_phi(phi_i, dX);
		for (int n = 0; n < phi_i.size(); ++n) {
			a = 1 / sqrt(phi_der[n]);
			outfile << "v" << ' ' << phi_i[n] << ' ' << 0 << ' ' << e*a << endl; 
			outfile << "v" << ' ' << phi_i[n] << ' ' << e*a << ' ' << 0 << endl;
			outfile << "v" << ' ' << phi_i[n] << ' ' << 0 << ' ' << -e*a << endl;
			outfile << "v" << ' ' << phi_i[n] << ' ' << -e*a << ' ' << 0 << endl;
		}
		int y = 4 * (N - 1) + 1;
		outfile << "f" << ' ' << y << ' ' << y+1 << ' ' << y+2 << ' ' << y+3 << endl;
		for (int n = 0; n < phi_i.size()-1; ++n){
			int x = 4 * n + 1;
			outfile << "f" << ' ' << 0+x << ' ' << 1+x << ' ' << 2+x << ' ' << 3+x << endl;
			outfile << "f" << ' ' << 0+x << ' ' << 1+x << ' ' << 5+x << ' ' << 4+x << endl;
			outfile << "f" << ' ' << 1+x << ' ' << 2+x << ' ' << 6+x << ' ' << 5+x << endl;
			outfile << "f" << ' ' << 2+x << ' ' << 3+x << ' ' << 7+x << ' ' << 6+x << endl;
			outfile << "f" << ' ' << 3+x << ' ' << 0+x << ' ' << 4+x << ' ' << 7+x << endl;
		}

	}

	return 0;
}