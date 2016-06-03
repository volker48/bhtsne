all:
	mkdir -p out
	rm -f out/bh_tsne
	g++ -O2 tsne.cpp -o out/bh_tsne

clean:
	rm -f out/*

test:
	# cat testdata/eye2.txt | ./bhtsne.py -d 2 -p 0.1 -r 0
	true
