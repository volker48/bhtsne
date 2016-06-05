all:
	mkdir -p out
	rm -f out/bh_tsne
	g++ -O2 tsne.cpp -o out/bh_tsne -fopenmp -g

clean:
	rm -f out/*

test:
	cd testdata/d2 && ../../out/bh_tsne
