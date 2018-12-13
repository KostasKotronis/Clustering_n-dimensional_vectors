# Clustering n-dimensional vectors

Implementation of algorithms for clustering n-dimensional vectors in C++ and validation of consistency within clusters with Sihouette's method. 
  
For calculating distances between vectors, **euclidean** and **cosine** distances are supported. 
  
In the **initialization stage**, the initial centroids of clusters can be selected either **randomly** or by using **K-Means++ algorithm**.
  
The **assignment** of vectors to the appropriate cluster, can be done by: 
- **Lloyd algorithm**
- **Range search with LSH**
- **Range search with Hypercube** 

The **update** of every cluster's centroid, can be done by:
- **K-Means**
- **Partitioning Around Medoids (PAM)** improved like Lloyd's

### Input file:
  A .csv file that every row corresponds to a vector. First column contains the id, and the rest n columns contain the coordinates of the vector.
  
  *vector_id1 &nbsp;&nbsp; c11 &nbsp;&nbsp; c12 &nbsp;&nbsp; ... &nbsp;&nbsp; c1n*
  
  *vector_id2 &nbsp;&nbsp; c21 &nbsp;&nbsp; c22 &nbsp;&nbsp; ... &nbsp;&nbsp; c2n*
  
  *...*
  
### Output file:
  A file that contains the results in the following format:
  
  *Algorithm: IxAxU*
  
  *Metric: euclidean or cosine*
  
  *CLUSTER-1 {size: <int>, centroid: <item_id> or coordinates of centroid(if <update method> is k-Means) }*
  
  *CLUSTER-2 {size: <int>, centroid: <item_id> or coordinates of centroid(if <update method> is k-Means) }*
  
  *...*
  
  *clustering_time: <double> //in seconds*
  
  *Silhouette: [s1, ..., sk, stotal]*
  
  

### Instructions to compile:
```make``` &nbsp;&nbsp; to compile

```make clean``` &nbsp;&nbsp; to delete executable and .o files.

### Instructions to execute:
```./cluster –i <input file> –c <configuration file> -o <output file>```
###### Additional arguements:
```-d <metric> ``` &nbsp;&nbsp; metric ∈ { euclidean (default), cosine }

```-I <initialization method> ``` &nbsp;&nbsp; initialization method ∈ { random (default), kmeans++ }

```-A <assignment method> ``` &nbsp;&nbsp; assignment method ∈ { lloyd (default), lsh, cube }

```-U <update method> ``` &nbsp;&nbsp; update method ∈ { kmeans (default), pam }

```-complete``` &nbsp;&nbsp; to add at the end of output file which vectors (id) every cluster contains.

```-all``` &nbsp;&nbsp; to run every combination of initialization, assignment and update methods (12).
