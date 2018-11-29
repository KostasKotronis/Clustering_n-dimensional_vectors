all: cluster

cluster: clustering.o cluster.o clusterFunctions.o data.o initialization.o assignment.o update.o evaluation.o euclidean.o cosine.o generalFunctions.o hashTable.o
	g++ clustering.o cluster.o clusterFunctions.o data.o initialization.o assignment.o update.o evaluation.o euclidean.o cosine.o generalFunctions.o hashTable.o -o cluster

clustering.o: clustering.cpp
	g++ -c clustering.cpp

cluster.o: cluster.cpp
	g++ -c cluster.cpp

data.o: data.cpp
	g++ -c data.cpp

clusterFunctions.o: clusterFunctions.cpp
	g++ -c clusterFunctions.cpp

initialization.o: initialization.cpp
	g++ -c initialization.cpp

assignment.o: assignment.cpp
	g++ -c assignment.cpp

update.o: update.cpp
	g++ -c update.cpp

evaluation.o: evaluation.cpp
	g++ -c evaluation.cpp

euclidean.o: euclidean.cpp
	g++ -c euclidean.cpp

cosine.o: cosine.cpp
	g++ -c cosine.cpp

generalFunctions.o: generalFunctions.cpp
	g++ -c generalFunctions.cpp

hashTable.o: hashTable.cpp
	g++ -c hashTable.cpp

clean:
	rm -rf *o cluster
