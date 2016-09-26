#include "tsne_core.cpp"

extern "C" {
    int run_tSNE_float64(double *inputData, double *outputData, int Nsamples, int in_dims, int out_dims, int max_iter, double theta, double perplexity, int rand_seed, bool verbose) {
    	return run_tSNE<double>(inputData, outputData, Nsamples, in_dims, out_dims, max_iter, theta, perplexity, rand_seed, verbose);
    }

    int run_tSNE_float32(float *inputData, float *outputData, int Nsamples, int in_dims, int out_dims, int max_iter, float theta, float perplexity, int rand_seed, bool verbose) {
    	return run_tSNE<float>(inputData, outputData, Nsamples, in_dims, out_dims, max_iter, theta, perplexity, rand_seed, verbose);
    }
}
