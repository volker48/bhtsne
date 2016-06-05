all:
	mkdir -p out
	rm -f out/bh_tsne
	rm -f out/libtsne.so
	g++ -O2 tsne.cpp -o out/bh_tsne -fopenmp -flto
	g++ -O2 -fPIC -shared tsne.cpp -o out/libtsne.so -fopenmp -flto

clean:
	rm -f out/*

test:
	cd testdata/d2 && ../../out/bh_tsne
