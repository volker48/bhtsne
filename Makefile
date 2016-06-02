all:
	mkdir -p out
	g++ -O2 sptree.cpp tsne.cpp -o out/bh_tsne

clean:
	rm -f out/*

test:
	echo "all good :) "
