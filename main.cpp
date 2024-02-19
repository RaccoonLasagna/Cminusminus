#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>


using namespace std;

// Function for trilinear interpolation
double trilinearInterpolate(double x, double y, double z,
                            const vector<vector<vector<double>>> &data) {
  int x0 = int(x);
  int y0 = int(y);
  int z0 = int(z);
  int x1 = x0 + 1;
  int y1 = y0 + 1;
  int z1 = z0 + 1;

  double xd = x - x0;
  double yd = y - y0;
  double zd = z - z0;

  double c00 = data[x0][y0][z0] * (1 - xd) + data[x1][y0][z0] * xd;
  double c10 = data[x0][y1][z0] * (1 - xd) + data[x1][y1][z0] * xd;
  double c01 = data[x0][y0][z1] * (1 - xd) + data[x1][y0][z1] * xd;
  double c11 = data[x0][y1][z1] * (1 - xd) + data[x1][y1][z1] * xd;

  double c0 = c00 * (1 - yd) + c10 * yd;
  double c1 = c01 * (1 - yd) + c11 * yd;

  return c0 * (1 - zd) + c1 * zd;
}

// Function to generate random 3D data
vector<vector<vector<double>>> generateRandomData() {
  srand(time(NULL));
  vector<vector<vector<double>>> data(
      101, vector<vector<double>>(101, vector<double>(101, 0.0)));
  for (int x = 0; x <= 100; ++x) {
    for (int y = 0; y <= 100; ++y) {
      for (int z = 0; z <= 100; ++z) {
        // Generate random data in the range of 0 to 1
        data[x][y][z] = static_cast<double>(rand()) / RAND_MAX;
      }
    }
  }
  return data;
}

// Function to save data to a CSV file
void saveToCSV(const vector<vector<vector<double>>> &result) {
  ofstream outputFile("result.csv");
  for (int x = 0; x < result.size(); ++x) {
    for (int y = 0; y < result[0].size(); ++y) {
      for (int z = 0; z < result[0][0].size(); ++z) {
        outputFile << x << "," << y << "," << z << " is equal to " << fixed
                   << setprecision(6) << result[x][y][z] << endl;
      }
    }
  }
  outputFile.close();
}

int main() {
  // Generate random 3D data
  vector<vector<vector<double>>> data = generateRandomData();

  cout << "Random data generated" << endl;

  // Interpolate and generate results
  vector<vector<vector<double>>> result(
      101, vector<vector<double>>(101, vector<double>(101, 0.0)));
  for (int x = 0; x <= 100; ++x) {
    cout << "Interpolating at x = " << x << endl;
    for (int y = 0; y <= 100; ++y) {
      // cout << "Interpolating at y = " << y << endl;
      for (int z = 0; z < 100; ++z) {
        // cout << "Interpolating at z = " << z << endl;
        double interpolated_value = trilinearInterpolate(x, y, z, data);
        // cout << "Interpolated value at (" << x << ", " << y << ", " << z
            //  << ") is " << interpolated_value << endl;
        result[x][y][z] = interpolated_value;
        // cout << "Interpolation at z = " << z << " complete" << endl;
      }
      // cout << "Interpolation at y = " << y << " complete" << endl;
    }
    // cout << "Interpolation at x = " << x << " complete" << endl;
  }
  cout << "Interpolation complete" << endl;
  // Save results to CSV file
  saveToCSV(result);

  // Output confirmation message
  cout << "Result saved to result.csv" << endl;

  return 0;
}