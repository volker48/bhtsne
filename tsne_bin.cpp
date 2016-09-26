#include "tsne_core.cpp"


// Function that loads data from a t-SNE file
// Note: this function does a malloc that should be freed elsewhere
template<typename T>
bool load_data(T** data, int* n, int* d, int* no_dims, int* max_iter, T* theta, T* perplexity, int* rand_seed) {

	// Open file, read first 2 integers, allocate memory, and read the data
    FILE *h;
	if((h = fopen("data.dat", "r+b")) == NULL) {
		printf("Error: could not open data file.\n");
		return false;
	}
	fread(n, sizeof(int), 1, h);											// number of datapoints
	fread(d, sizeof(int), 1, h);											// original dimensionality
    fread(theta, sizeof(T), 1, h);										// gradient accuracy
	fread(perplexity, sizeof(T), 1, h);								// perplexity
	fread(no_dims, sizeof(int), 1, h);                                      // output dimensionality
        fread(max_iter, sizeof(int), 1, h);
	*data = (T*) malloc(*d * *n * sizeof(T));
    if(*data == NULL) { printf("Memory allocation failed!\n"); exit(1); }
    fread(*data, sizeof(T), *n * *d, h);                               // the data

    *rand_seed = 0;
    if(!feof(h)) fread(rand_seed, sizeof(int), 1, h);                       // random seed
	fclose(h);
    printf("Read the %i x %i data matrix successfully!\n", *n, *d);
	return true;
}

// Function that saves map to a t-SNE file
template<typename T>
void save_data(T* data, int* landmarks, T* costs, int n, int d) {

	// Open file, write first 2 integers and then the data
	FILE *h;
	if((h = fopen("result.dat", "w+b")) == NULL) {
		printf("Error: could not open data file.\n");
		return;
	}
	fwrite(&n, sizeof(int), 1, h);
	fwrite(&d, sizeof(int), 1, h);
    fwrite(data, sizeof(T), n * d, h);
	fwrite(landmarks, sizeof(int), n, h);
    fwrite(costs, sizeof(T), n, h);
    fclose(h);
	printf("Wrote the %i x %i data matrix successfully!\n", n, d);
}


template<typename T>
void run_tSNE_andSave(T *inputData, int N, int D, int no_dims, int max_iter, T theta, T perplexity, int rand_seed) {
	// Allocate memory for the output
	T* Y = (T*) malloc(N * no_dims * sizeof(T));
	if(Y == NULL) { printf("Memory allocation failed!\n"); exit(1); }


    int res = run_tSNE(inputData, Y, N, D, no_dims, max_iter, theta, perplexity, rand_seed, true);

    if (res > 0)
        exit(res);


    // Make dummy landmarks and costs
    int* landmarks = (int*) malloc(N * sizeof(int));
    if(landmarks == NULL) { printf("Memory allocation failed!\n"); exit(1); }
    for(int n = 0; n < N; n++) landmarks[n] = n;
    T* costs = (T*) calloc(N, sizeof(T));
    if(costs == NULL) { printf("Memory allocation failed!\n"); exit(1); }


	// Save the results
	save_data(Y, landmarks, costs, N, no_dims);

    // Clean up the memory
	free(Y); Y = NULL;
	free(costs); costs = NULL;
	free(landmarks); landmarks = NULL;
}



// Function that runs the Barnes-Hut implementation of t-SNE
int main() {

    // Define some variables
	int N, D, no_dims, max_iter, rand_seed;
	double perplexity, theta, *data;

    // Read the parameters and the dataset
	if(load_data<double>(&data, &N, &D, &no_dims, &max_iter, &theta, &perplexity, &rand_seed)) {
         run_tSNE_andSave<double>(data, N, D, no_dims, max_iter, theta, perplexity, rand_seed);
        free(data); data = NULL;
    }
}
