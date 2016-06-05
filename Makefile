all: tsne_bin tsne_lib


tsne_bin: tsne_core.cpp sptree.h sptree.cpp tsne.h vptree.h tsne_bin.cpp
	mkdir -p out
	rm -f out/bh_tsne
	g++ -O2 -flto -ffast-math tsne_bin.cpp -o out/bh_tsne -fopenmp



tsne_lib: tsne_core.cpp sptree.h sptree.cpp tsne.h vptree.h tsne_lib.cpp
	mkdir -p out
	rm -f out/libtsne.so
	g++ -O2 -flto -ffast-math -fPIC -shared tsne_lib.cpp -o out/libtsne.so -fopenmp -Wall

clean:
	rm -f out/*

test:
	cd testdata/d2 && ../../out/bh_tsne
