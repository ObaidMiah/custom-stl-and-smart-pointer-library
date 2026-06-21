#include "../include/vector.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <iomanip>

// Benchmark function for our custom Vector
double benchmark_custom_vector(size_t num_elements) {
    Vector<int> vec;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < num_elements; ++i) {
        vec.push_back(i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    return duration.count() / 1000.0; // Convert to milliseconds
}

double benchmark_constant_vector(size_t num_elements) {
    Vector<int> vec;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < num_elements; ++i) {
       // vec.push_back_constant(i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    return duration.count() / 1000.0; // Convert to milliseconds
}

// Benchmark function for std::vector (for comparison)
double benchmark_std_vector(size_t num_elements) {
    std::vector<int> vec;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < num_elements; ++i) {
        vec.push_back(i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    return duration.count() / 1000.0; // Convert to milliseconds
}

// Generate test sizes with exponential growth
std::vector<size_t> generate_test_sizes() {
    std::vector<size_t> sizes;
    
    // Start with small sizes and grow exponentially
    for (size_t i = 100; i <= 1000000; i *= 2) {
        sizes.push_back(i);
    }
    
    // Add some intermediate sizes for better granularity
    std::vector<size_t> intermediate = {500, 1500, 3000, 7000, 15000, 30000, 70000, 150000, 300000, 700000};
    sizes.insert(sizes.end(), intermediate.begin(), intermediate.end());
    
    // Sort and remove duplicates
    std::sort(sizes.begin(), sizes.end());
    sizes.erase(std::unique(sizes.begin(), sizes.end()), sizes.end());
    
    return sizes;
}

int main() {
    std::cout << "Vector Performance Benchmark\n";
    std::cout << "==========================\n\n";
    
    auto test_sizes = generate_test_sizes();
    std::vector<double> custom_times;
    std::vector<double> std_times;
    std::vector<double> const_times;

    std::cout << std::setw(12) << "Size" << std::setw(20) << "Custom Vector (ms)" 
              << std::setw(20) << "STD Vector (ms)" << std::setw(20) << "Constant Vector(ms)\n";
    std::cout << std::string(72, '-') << "\n";
    
    for (size_t i = 0; i < test_sizes.size(); ++i) {
        size_t size = test_sizes[i];
        
        // Run benchmarks multiple times and take average
        const int num_runs = 5;
        double custom_total = 0.0;
        double std_total = 0.0;
        double const_total = 0.0;

        for (int run = 0; run < num_runs; ++run) {
            custom_total += benchmark_custom_vector(size);
            std_total += benchmark_std_vector(size);
            const_total += benchmark_constant_vector(size);
        }
        
        double custom_avg = custom_total / num_runs;
        double std_avg = std_total / num_runs;
        double const_avg = const_total / num_runs;
        
        custom_times.push_back(custom_avg);
        std_times.push_back(std_avg);
        const_times.push_back(const_avg);

        std::cout << std::setw(12) << size 
                  << std::setw(20) << std::fixed << std::setprecision(2) << custom_avg
                  << std::setw(20) << std::fixed << std::setprecision(2) << std_avg
                  << std::setw(20) << std::fixed << std::setprecision(2) << const_avg << "\n";
    }
    
    // Write JSON file
    std::ofstream data_file("../data/benchmark_data.json");
    data_file << "{\n";
    
    // Write test sizes
    data_file << "  \"test_sizes\": [";
    for (size_t i = 0; i < test_sizes.size(); ++i) {
        if (i > 0) data_file << ", ";
        data_file << test_sizes[i];
    }
    data_file << "],\n";
    
    // Write custom vector times
    data_file << "  \"custom_vector_times\": [";
    for (size_t i = 0; i < custom_times.size(); ++i) {
        if (i > 0) data_file << ", ";
        data_file << custom_times[i];
    }
    data_file << "],\n";
    
    // Write std vector times
    data_file << "  \"std_vector_times\": [";
    for (size_t i = 0; i < std_times.size(); ++i) {
        if (i > 0) data_file << ", ";
        data_file << std_times[i];
    }
    data_file << "],\n";

    // Write constant vector times
    data_file << "  \"const_vector_times\": [";
    for (size_t i = 0; i < const_times.size(); ++i) {
        if (i > 0) data_file << ", ";
        data_file << const_times[i];
    }
    data_file << "]\n";
    
    data_file << "}\n";
    data_file.close();
    
    std::cout << "\nBenchmark data saved to 'benchmark_data.json'\n";
    
    return 0;
}
